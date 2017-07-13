/*
 * This file implements support for i2c on the BeagleBone and BeagleBoard-xM
 */

/* kernel headers */
#include <minix/chardriver.h>
#include <minix/clkconf.h>
#include <minix/drivers.h>
#include <minix/ds.h>
#include <minix/log.h>
#include <minix/mmio.h>
#include <minix/padconf.h>
#include <minix/sysutil.h>
#include <minix/type.h>
#include <minix/board.h>
#include <minix/spin.h>

/* device headers */
#include <minix/i2c.h>

/* system headers */
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>

/* usr headers */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* local headers */
#include "rpi_i2c.h"

/*
 * defines the set of register
 *
 * Warning: always use the 16-bit variants of read/write/set from mmio.h
 * to access these registers. The DM37XX TRM Section 17.6 warns that 32-bit
 * accesses can corrupt the register contents.
 */
typedef struct
{
	vir_bytes CTRL;
	vir_bytes ST;
	vir_bytes DLEN;
	vir_bytes SL_ADDR;
	vir_bytes FIFO;
	vir_bytes DIV;
	vir_bytes DEL;
	vir_bytes CLKT;
} rpi_i2c_regs_t;

/* generic definition an i2c bus */

typedef struct rpi_i2c_bus
{
	phys_bytes mr_base;
	phys_bytes mr_size;
	vir_bytes mapped_addr;
	rpi_i2c_regs_t *regs;
	uint32_t functional_clock;
	uint32_t module_clock;
	uint32_t bus_speed;
	int irq;
	int irq_hook_id;
	int irq_hook_kernel_id;
} rpi_i2c_bus_t;

/* Define the registers for each chip */

static rpi_i2c_regs_t bcm283x_i2c_regs = {
	.CTRL = BCM283X_CTRL,
	.ST = BCM283X_STATUS,
	.DLEN = BCM283X_DLEN,
	.SL_ADDR = BCM283X_SL_ADDR,
	.FIFO = BCM283X_FIFO,
	.DIV = BCM283X_DIV,
	.DEL = BCM283X_DEL,
	.CLKT = BCM283X_CLKT,
};

/* Define the buses available on each chip */

static rpi_i2c_bus_t bcm283x_i2c_buses[] = {
	{BCM283X_I2C0_BASE, BCM283X_I2C0_SIZE, 0, &bcm283x_i2c_regs,
		BCM283X_FUNCTIONAL_CLOCK, BCM283X_MODULE_CLOCK, 
		BUS_SPEED_400KHz, BCM283X_I2C0_IRQ, 1, 1},
	{BCM283X_I2C1_BASE, BCM283X_I2C1_SIZE, 0, &bcm283x_i2c_regs,
		BCM283X_FUNCTIONAL_CLOCK, BCM283X_MODULE_CLOCK,
		BUS_SPEED_400KHz, BCM283X_I2C0_IRQ, 2, 3}
	};

#define BCM283X_rpi_NBUSES (sizeof(bcm283x_i2c_buses) / sizeof(rpi_i2c_bus_t))

/* Globals */

static rpi_i2c_bus_t *rpi_i2c_buses;	/* all available buses for this SoC */
static rpi_i2c_bus_t *rpi_i2c_bus;	/* the bus selected at start-up */
static int rpi_i2c_nbuses;	/* number of buses supported by SoC */

/* logging - use with log_warn(), log_info(), log_debug(), log_trace() */
static struct log log = {
	.name = "i2c",
	.log_level = LEVEL_INFO,
	.log_func = default_log
};

/* Local Function Prototypes */

/* Implementation of Generic I2C Interface using Bus Specific Code */
static int rpi_i2c_process(minix_i2c_ioctl_exec_t * m);

/* Bus Specific Code */
static void rpi_i2c_flush(void);
static uint32_t rpi_i2c_poll(uint32_t mask);
static int rpi_i2c_bus_is_free(void);
static int rpi_i2c_soft_reset(void);
static void rpi_i2c_bus_init(void);
static void rpi_i2c_padconf(int i2c_bus_id);
static void rpi_i2c_clkconf(int i2c_bus_id);
static void rpi_i2c_intr_enable(void);
static uint32_t rpi_i2c_read_status(void);
static void rpi_i2c_write_status(uint32_t mask);
static int rpi_i2c_read(i2c_addr_t addr, uint8_t * buf, size_t buflen,
    int dostop);
static int rpi_i2c_write(i2c_addr_t addr, const uint8_t * buf, size_t buflen,
    int dostop);

/*
 * Performs the action in minix_i2c_ioctl_exec_t.
 */
static int
rpi_i2c_process(minix_i2c_ioctl_exec_t * ioctl_exec)
{
	int r;

	/*
	 * Zero data bytes transfers are not allowed. The controller treats
	 * I2C_CNT register value of 0x0 as 65536. This is true for both the
	 * am335x and dm37xx. Full details in the TRM on the I2C_CNT page.
	 */
	if (ioctl_exec->iie_buflen == 0) {
		return EINVAL;
	}

	rpi_i2c_flush();	/* clear any garbage in the fifo */

	/* Check bus busy flag before using the bus */
	r = rpi_i2c_bus_is_free();
	if (r == 0) {
		log_warn(&log, "Bus is busy\n");
		return EBUSY;
	}

	if (ioctl_exec->iie_cmdlen > 0) {
		r = rpi_i2c_write(ioctl_exec->iie_addr, ioctl_exec->iie_cmd,
		    ioctl_exec->iie_cmdlen,
		    !(I2C_OP_READ_P(ioctl_exec->iie_op)));
		if (r != OK) {
			rpi_i2c_soft_reset();
			rpi_i2c_bus_init();
			return r;
		}
	}

	if (I2C_OP_READ_P(ioctl_exec->iie_op)) {
		r = rpi_i2c_read(ioctl_exec->iie_addr, ioctl_exec->iie_buf,
		    ioctl_exec->iie_buflen, I2C_OP_STOP_P(ioctl_exec->iie_op));
	} else {
		r = rpi_i2c_write(ioctl_exec->iie_addr, ioctl_exec->iie_buf,
		    ioctl_exec->iie_buflen, I2C_OP_STOP_P(ioctl_exec->iie_op));
	}

	if (r != OK) {
		rpi_i2c_soft_reset();
		rpi_i2c_bus_init();
		return r;
	}

	return OK;
}

/*
 * Drain the incoming FIFO.
 *
 * Usually called to clear any garbage that may be in the buffer before
 * doing a read.
 */
static void
rpi_i2c_flush(void)
{
	int tries;
	int status;

	for (tries = 0; tries < 1000; tries++) {
		status = rpi_i2c_poll(BCM283X_STATUS_RXD);
		if ((status & BCM283X_STATUS_RXD) != 0) {	/* bytes available for reading */

			/* consume the byte and throw it away */
			(void) read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->FIFO);

		} else {
			break;	/* buffer drained */
		}
	}
}

/*
 * Poll the status register checking the bits set in 'mask'.
 * Returns the status if any bits set or 0x0000 when the timeout is reached.
 */
static uint32_t
rpi_i2c_poll(uint32_t mask)
{
	spin_t spin;
	uint32_t status;

	/* poll for up to 1 s */
	spin_init(&spin, 1000000);
	do {
		status = rpi_i2c_read_status();
		if ((status & mask) != 0) {	/* any bits in mask set */
			return status;
		}

	} while (spin_check(&spin));

	return status;		/* timeout reached, abort */
}

/*
 * Poll Bus Busy Flag until the bus becomes free (return 1) or the timeout
 * expires (return 0).
 */
static int
rpi_i2c_bus_is_free(void)
{
	spin_t spin;
	uint16_t status;

	/* wait for up to 1 second for the bus to become free */
	spin_init(&spin, 1000000);
	do {

		status = rpi_i2c_read_status();
		if ((status & BCM283X_STATUS_TA) == 0) {
			return 1;	/* bus is free */
		}

	} while (spin_check(&spin));

	return 0;		/* timeout expired */
}

static void
rpi_i2c_clkconf(int i2c_bus_id)
{
	clkconf_init();

	switch (i2c_bus_id) {
	case 0:
		clkconf_set(CM_WKUP_I2C0_CLKCTRL, BIT(1), 0xffffffff);
		break;
	case 1:
		clkconf_set(CM_PER_I2C1_CLKCTRL, BIT(1), 0xffffffff);
		break;
	default:
		log_warn(&log, "Invalid i2c_bus_id\n");
		break;
	}

	clkconf_release();
}

static void
rpi_i2c_padconf(int i2c_bus_id)
{
	int r;
	u32_t pinopts;

	/* use the options suggested in starterware driver */
	pinopts =
	    CONTROL_CONF_SLEWCTRL | CONTROL_CONF_RXACTIVE |
	    CONTROL_CONF_PUTYPESEL;
	switch (i2c_bus_id) {
	case 0:
		pinopts |= CONTROL_CONF_MUXMODE(0);
	
		r = sys_padconf(CONTROL_CONF_I2C0_SDA, 0xffffffff,
		    pinopts);
		if (r != OK) {
			log_warn(&log, "padconf failed (r=%d)\n", r);
		}
	
		r = sys_padconf(CONTROL_CONF_I2C0_SCL, 0xffffffff,
		    pinopts);
		if (r != OK) {
			log_warn(&log, "padconf failed (r=%d)\n", r);
		}
	
		log_debug(&log, "pinopts=0x%x\n", pinopts);
		break;
	
	case 1:
		pinopts |= CONTROL_CONF_MUXMODE(2);
	
		r = sys_padconf(CONTROL_CONF_SPI0_CS0, 0xffffffff,
		    pinopts);
		if (r != OK) {
			log_warn(&log, "padconf failed (r=%d)\n", r);
		}
	
		r = sys_padconf(CONTROL_CONF_SPI0_D1, 0xffffffff,
		    pinopts);
		if (r != OK) {
			log_warn(&log, "padconf failed (r=%d)\n", r);
		}
	
		log_debug(&log, "pinopts=0x%x\n", pinopts);
		break;
	
	default:
		log_warn(&log, "Invalid i2c_bus_id\n");
		break;
	}
}

static int
rpi_i2c_soft_reset(void)
{
	/* Disable to do soft reset */
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, 0);
	micro_delay(50000);

	/* Have to temporarily enable I2C to read RDONE */
	set32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, BCM283X_CTRL_I2C_EN | BCM283X_CTRL_CFIFO, 
		BCM283X_CTRL_I2C_EN | BCM283X_CTRL_CFIFO);
	micro_delay(50000);

	return OK;
}

static void
rpi_i2c_intr_enable(void)
{
	int r;
	uint16_t intmask;
	static int policy_set = 0;
	static int enabled = 0;

	if (!policy_set) {
		r = sys_irqsetpolicy(rpi_i2c_bus->irq, 0,
		    &rpi_i2c_bus->irq_hook_kernel_id);
		if (r == OK) {
			policy_set = 1;
		} else {
			log_warn(&log, "Couldn't set irq policy\n");
		}
	}

	if (policy_set && !enabled) {
		r = sys_irqenable(&rpi_i2c_bus->irq_hook_kernel_id);
		if (r == OK) {
			enabled = 1;
		} else {
			log_warn(&log, "Couldn't enable irq %d (hooked)\n",
			    rpi_i2c_bus->irq);
		}
	}

	/* According to NetBSD driver and u-boot, these are needed even
	 * if just using polling (i.e. non-interrupt driver programming).
	 */
	intmask = 0;
	intmask |= BCM283X_CTRL_INTR;
	intmask |= BCM283X_CTRL_INTT;
	intmask |= BCM283X_CTRL_INTD;

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, intmask);
}

static void
rpi_i2c_bus_init(void)
{

	/* Ensure i2c module is disabled before setting prescalar & bus speed */
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, 0);
	micro_delay(50000);

	/* Set prescalar to obtain 12 MHz i2c module clock */
	uint32_t divider = (rpi_i2c_bus->functional_clock / rpi_i2c_bus->module_clock) - 1;
	if (divider > BCM283X_I2C_CDIV_MAX ||
		divider < BCM283X_I2C_CDIV_MIN) {
		log_warn(&log, "Incorrect divider\n");
	}

	if (divider & 0x01)
		divider++;

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->DIV, divider);

	/* Set own I2C address */
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->SL_ADDR, I2C_OWN_ADDRESS);

	/* Bring the i2c module out of reset */
	set32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, BCM283X_CTRL_I2C_EN, BCM283X_CTRL_I2C_EN);
	micro_delay(50000);

	/*
	 * Enable interrupts
	 */
	rpi_i2c_intr_enable();
}

static uint32_t
rpi_i2c_read_status(void)
{
	uint16_t status = read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->ST);
	return status;
}

static void
rpi_i2c_write_status(uint32_t mask)
{
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->ST, mask);
}

static int
rpi_i2c_read(i2c_addr_t addr, uint8_t * buf, size_t buflen, int dostop)
{
	int r, i;
	uint32_t ctrl;
	uint32_t pollmask;
	uint32_t errmask;

	/* Set address of slave device */
	addr &= MAX_I2C_SA_MASK;	/* sanitize address (10-bit max) */

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->DLEN, 1);

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->FIFO, addr & 0xff);

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->SL_ADDR, (addr >> XSA) | SL_ADDR_MASK);

	ctrl = read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL);
	ctrl |= BCM283X_CTRL_ST;
	ctrl &= (~BCM283X_CTRL_READ);
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, ctrl);

	pollmask |= BCM283X_STATUS_DONE;
	errmask  |= BCM283X_STATUS_CLKT;
	errmask  |= BCM283X_STATUS_ERR;

	r = rpi_i2c_poll(pollmask | errmask);
	if ((r & errmask) != 0) {
		/* only debug log level because i2cscan trigers this */
		log_debug(&log, "Read Error! Status=%x\n", r);
		return EIO;
	} else if ((r & pollmask) == 0) {
		log_warn(&log, "No RRDY Interrupt. Status=%x\n", r);
		log_warn(&log,
		    "Likely cause: bad pinmux or no devices on bus\n");
		return EBUSY;
	}
	
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->DLEN, buflen);

	ctrl = read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL);
	ctrl |= BCM283X_CTRL_ST;
	ctrl |= BCM283X_CTRL_READ;
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, ctrl);

	pollmask |= BCM283X_STATUS_RXD;
	pollmask |= BCM283X_STATUS_RXF;

	for (i = 0; i < buflen; i++) {
		/* Data to read? */
		r = rpi_i2c_poll(pollmask | errmask);
		if ((r & errmask) != 0) {
			/* only debug log level because i2cscan trigers this */
			log_debug(&log, "Read Error! Status=%x\n", r);
			return EIO;
		} else if ((r & pollmask) == 0) {
			log_warn(&log, "No RRDY Interrupt. Status=%x\n", r);
			log_warn(&log,
			    "Likely cause: bad pinmux or no devices on bus\n");
			return EBUSY;
		}

		/* read a byte */
		buf[i] = read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->FIFO) & 0xff;

		/* clear the read ready flag */
		rpi_i2c_write_status(BCM283X_STATUS_DONE | errmask);
	}

	set32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, BCM283X_CTRL_CFIFO, BCM283X_CTRL_CFIFO);

	return 0;
}

static int
rpi_i2c_write(i2c_addr_t addr, const uint8_t * buf, size_t buflen, int dostop)
{
	int r, i;
	uint32_t pollmask;
	uint32_t errmask;
	uint32_t ctrl;

	/* Set address of slave device */
	addr &= MAX_I2C_SA_MASK;	/* sanitize address (10-bit max) */

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->DLEN, buflen + 1);

	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->FIFO, addr & 0xff);

	pollmask |= BCM283X_STATUS_DONE;
	pollmask |= BCM283X_STATUS_TXE;
	pollmask |= BCM283X_STATUS_TXD;
	errmask  |= BCM283X_STATUS_CLKT;
	errmask  |= BCM283X_STATUS_ERR;

	ctrl = read32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL);
	ctrl |= BCM283X_CTRL_ST;
	ctrl &= (~BCM283X_CTRL_READ);
	write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, ctrl);

	for (i = 0; i < buflen; i++) {

		/* Ready to write? */
		r = rpi_i2c_poll(pollmask | errmask);
		if ((r & errmask) != 0) {
			log_warn(&log, "Write Error! Status=%x\n", r);
			return EIO;
		} else if ((r & pollmask) == 0) {
			log_warn(&log, "Not ready for write? Status=%x\n", r);
			return EBUSY;
		}

		write32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->FIFO, buf[i]);

		/* clear the write ready flag */
		rpi_i2c_write_status(BCM283X_STATUS_DONE | errmask);
	}

	set32(rpi_i2c_bus->mapped_addr + rpi_i2c_bus->regs->CTRL, BCM283X_CTRL_CFIFO, BCM283X_CTRL_CFIFO);

	return 0;
}

int
rpi_interface_setup(int (**process) (minix_i2c_ioctl_exec_t * ioctl_exec),
    int i2c_bus_id)
{
	int r;
	int i2c_rev, major, minor;
	struct minix_mem_range mr;
	struct machine machine;
	sys_getmachine(&machine);

	/* Fill in the function pointer */

	*process = rpi_i2c_process;

	/* Select the correct i2c definition for this SoC */

	if (BOARD_IS_RPI_2_B(machine.board_id) || BOARD_IS_RPI_3_B(machine.board_id)){
		rpi_i2c_buses = bcm283x_i2c_buses;
		rpi_i2c_nbuses = BCM283X_rpi_NBUSES;
	} else {
		return EINVAL;
	}

	if (i2c_bus_id < 0 || i2c_bus_id >= rpi_i2c_nbuses) {
		return EINVAL;
	}

	/* select the bus to operate on */
	rpi_i2c_bus = &rpi_i2c_buses[i2c_bus_id];

	/* Configure Pins */
	rpi_i2c_padconf(i2c_bus_id);

	/*
	 * Map I2C Registers
	 */

	/* Configure memory access */
	mr.mr_base = rpi_i2c_bus->mr_base;	/* start addr */
	mr.mr_limit = mr.mr_base + rpi_i2c_bus->mr_size;	/* end addr */

	/* ask for privileges to access the I2C memory range */
	if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != OK) {
		panic("Unable to obtain i2c memory range privileges");
	}

	/* map the memory into this process */
	rpi_i2c_bus->mapped_addr = (vir_bytes) vm_map_phys(SELF,
	    (void *) rpi_i2c_bus->mr_base, rpi_i2c_bus->mr_size);

	if (rpi_i2c_bus->mapped_addr == (vir_bytes) MAP_FAILED) {
		panic("Unable to map i2c registers");
	}

	/* Enable Clocks */
	rpi_i2c_clkconf(i2c_bus_id);

	/* Perform a soft reset of the I2C module to ensure a fresh start */
	r = rpi_i2c_soft_reset();
	if (r != OK) {
		/* module didn't come back up :( */
		return r;
	}

	/* Bring up I2C module */
	rpi_i2c_bus_init();

	return OK;
}
