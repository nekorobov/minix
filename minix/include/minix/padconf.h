#ifndef __MINIX_PADCONF_H
#define __MINIX_PADCONF_H

/* Define the start address of the padconf registers and the size of the block.
 * The base must be page aligned, so we round down and the kernel adds the
 * offset. The size must be a multiple of ARM_PAGE_SIZE, so we round up to 4KB.
 */
#define PADCONF_AM335X_REGISTERS_BASE 0x44E10000
#define PADCONF_AM335X_REGISTERS_OFFSET 0x0000
#define PADCONF_AM335X_REGISTERS_SIZE 0x1000 /* OFFSET + highest reg, rounded up */
#define PADCONF_DM37XX_REGISTERS_BASE 0x48002000
#define PADCONF_DM37XX_REGISTERS_OFFSET 0x0030
#define PADCONF_DM37XX_REGISTERS_SIZE 0x1000 /* OFFSET + highest reg, rounded up */

#define PADCONF_MUXMODE(X)  (X & 0x7)	/* mode 1 til 7 [2:0] */
#define PADCONF_PULL_MODE(X)  ((X & 0x3) << 3)	/* 2 bits[4:3] */
#define PADCONF_PULL_MODE_PD_DIS PADCONF_PULL_MODE(0)	/* pull down disabled */
#define PADCONF_PULL_MODE_PD_EN PADCONF_PULL_MODE(1)	/* pull down enabled */
#define PADCONF_PULL_MODE_PU_DIS PADCONF_PULL_MODE(2)	/* pull up disabled */
#define PADCONF_PULL_MODE_PU_EN PADCONF_PULL_MODE(3)	/* pull up enabled */
#define PADCONF_INPUT_ENABLE(X)  ((X & 0x1) << 8)	/* 1 bits[8] */
#define PADCONF_OFF_MODE(X)  ((X & 0xFE) << 9)	/* 5 bits[13:9] */

/* padconf pin definitions */
#define CONTROL_PADCONF_SDRC_D0 (0x00000000)
#define CONTROL_PADCONF_SDRC_D2 (0x00000004)
#define CONTROL_PADCONF_SDRC_D4 (0x00000008)
#define CONTROL_PADCONF_SDRC_D6 (0x0000000C)
#define CONTROL_PADCONF_SDRC_D8 (0x00000010)
#define CONTROL_PADCONF_SDRC_D10 (0x00000014)
#define CONTROL_PADCONF_SDRC_D12 (0x00000018)
#define CONTROL_PADCONF_SDRC_D14 (0x0000001C)
#define CONTROL_PADCONF_SDRC_D16 (0x00000020)
#define CONTROL_PADCONF_SDRC_D18 (0x00000024)
#define CONTROL_PADCONF_SDRC_D20 (0x00000028)
#define CONTROL_PADCONF_SDRC_D22 (0x0000002C)
#define CONTROL_PADCONF_SDRC_D24 (0x00000030)
#define CONTROL_PADCONF_SDRC_D26 (0x00000034)
#define CONTROL_PADCONF_SDRC_D28 (0x00000038)
#define CONTROL_PADCONF_SDRC_D30 (0x0000003C)
#define CONTROL_PADCONF_SDRC_CLK (0x00000040)
#define CONTROL_PADCONF_SDRC_DQS1 (0x00000044)
#define CONTROL_PADCONF_SDRC_DQS3 (0x00000048)
#define CONTROL_PADCONF_GPMC_A2 (0x0000004C)
#define CONTROL_PADCONF_GPMC_A4 (0x00000050)
#define CONTROL_PADCONF_GPMC_A6 (0x00000054)
#define CONTROL_PADCONF_GPMC_A8 (0x00000058)
#define CONTROL_PADCONF_GPMC_A10 (0x0000005C)
#define CONTROL_PADCONF_GPMC_D1 (0x00000060)
#define CONTROL_PADCONF_GPMC_D3 (0x00000064)
#define CONTROL_PADCONF_GPMC_D5 (0x00000068)
#define CONTROL_PADCONF_GPMC_D7 (0x0000006C)
#define CONTROL_PADCONF_GPMC_D9 (0x00000070)
#define CONTROL_PADCONF_GPMC_D11 (0x00000074)
#define CONTROL_PADCONF_GPMC_D13 (0x00000078)
#define CONTROL_PADCONF_GPMC_D15 (0x0000007C)
#define CONTROL_PADCONF_GPMC_NCS1 (0x00000080)
#define CONTROL_PADCONF_GPMC_NCS3 (0x00000084)
#define CONTROL_PADCONF_GPMC_NCS5 (0x00000088)
#define CONTROL_PADCONF_GPMC_NCS7 (0x0000008C)
#define CONTROL_PADCONF_GPMC_NADV_ALE (0x00000090)
#define CONTROL_PADCONF_GPMC_NWE (0x00000094)
#define CONTROL_PADCONF_GPMC_NBE1 (0x00000098)
#define CONTROL_PADCONF_GPMC_WAIT0 (0x0000009C)
#define CONTROL_PADCONF_GPMC_WAIT2 (0x000000A0)
#define CONTROL_PADCONF_DSS_PCLK (0x000000A4)
#define CONTROL_PADCONF_DSS_VSYNC (0x000000A8)
#define CONTROL_PADCONF_DSS_DATA0 (0x000000AC)
#define CONTROL_PADCONF_DSS_DATA2 (0x000000B0)
#define CONTROL_PADCONF_DSS_DATA4 (0x000000B4)
#define CONTROL_PADCONF_DSS_DATA6 (0x000000B8)
#define CONTROL_PADCONF_DSS_DATA8 (0x000000BC)
#define CONTROL_PADCONF_DSS_DATA10 (0x000000C0)
#define CONTROL_PADCONF_DSS_DATA12 (0x000000C4)
#define CONTROL_PADCONF_DSS_DATA14 (0x000000C8)
#define CONTROL_PADCONF_DSS_DATA16 (0x000000CC)
#define CONTROL_PADCONF_DSS_DATA18 (0x000000D0)
#define CONTROL_PADCONF_DSS_DATA20 (0x000000D4)
#define CONTROL_PADCONF_DSS_DATA22 (0x000000D8)
#define CONTROL_PADCONF_CAM_HS (0x000000DC)
#define CONTROL_PADCONF_CAM_XCLKA (0x000000E0)
#define CONTROL_PADCONF_CAM_FLD (0x000000E4)
#define CONTROL_PADCONF_CAM_D1 (0x000000E8)
#define CONTROL_PADCONF_CAM_D3 (0x000000EC)
#define CONTROL_PADCONF_CAM_D5 (0x000000F0)
#define CONTROL_PADCONF_CAM_D7 (0x000000F4)
#define CONTROL_PADCONF_CAM_D9 (0x000000F8)
#define CONTROL_PADCONF_CAM_D11 (0x000000FC)
#define CONTROL_PADCONF_CAM_WEN (0x00000100)
#define CONTROL_PADCONF_CSI2_DX0 (0x00000104)
#define CONTROL_PADCONF_CSI2_DX1 (0x00000108)
#define CONTROL_PADCONF_MCBSP2_FSX (0x0000010C)
#define CONTROL_PADCONF_MCBSP2_DR (0x00000110)
#define CONTROL_PADCONF_MMC1_CLK (0x00000114)
#define CONTROL_PADCONF_MMC1_DAT0 (0x00000118)
#define CONTROL_PADCONF_MMC1_DAT2 (0x0000011C)
#define CONTROL_PADCONF_MMC2_CLK (0x00000128)
#define CONTROL_PADCONF_MMC2_DAT0 (0x0000012C)
#define CONTROL_PADCONF_MMC2_DAT2 (0x00000130)
#define CONTROL_PADCONF_MMC2_DAT4 (0x00000134)
#define CONTROL_PADCONF_MMC2_DAT6 (0x00000138)
#define CONTROL_PADCONF_MCBSP3_DX (0x0000013C)
#define CONTROL_PADCONF_MCBSP3_CLKX (0x00000140)
#define CONTROL_PADCONF_UART2_CTS (0x00000144)
#define CONTROL_PADCONF_UART2_TX (0x00000148)
#define CONTROL_PADCONF_UART1_TX (0x0000014C)
#define CONTROL_PADCONF_UART1_CTS (0x00000150)
#define CONTROL_PADCONF_MCBSP4_CLKX (0x00000154)
#define CONTROL_PADCONF_MCBSP4_DX (0x00000158)
#define CONTROL_PADCONF_MCBSP1_CLKR (0x0000015C)
#define CONTROL_PADCONF_MCBSP1_DX (0x00000160)
#define CONTROL_PADCONF_MCBSP_CLKS (0x00000164)
#define CONTROL_PADCONF_MCBSP1_CLKX (0x00000168)
#define CONTROL_PADCONF_UART3_RTS_SD (0x0000016C)
#define CONTROL_PADCONF_UART3_TX_IRTX (0x00000170)
#define CONTROL_PADCONF_HSUSB0_STP (0x00000174)
#define CONTROL_PADCONF_HSUSB0_NXT (0x00000178)
#define CONTROL_PADCONF_HSUSB0_DATA1 (0x0000017C)
#define CONTROL_PADCONF_HSUSB0_DATA3 (0x00000180)
#define CONTROL_PADCONF_HSUSB0_DATA5 (0x00000184)
#define CONTROL_PADCONF_HSUSB0_DATA7 (0x00000188)
#define CONTROL_PADCONF_I2C1_SDA (0x0000018C)
#define CONTROL_PADCONF_I2C2_SDA (0x00000190)
#define CONTROL_PADCONF_I2C3_SDA (0x00000194)
#define CONTROL_PADCONF_MCSPI1_CLK (0x00000198)
#define CONTROL_PADCONF_MCSPI1_SOMI (0x0000019C)
#define CONTROL_PADCONF_MCSPI1_CS1 (0x000001A0)
#define CONTROL_PADCONF_MCSPI1_CS3 (0x000001A4)
#define CONTROL_PADCONF_MCSPI2_SIMO (0x000001A8)
#define CONTROL_PADCONF_MCSPI2_CS0 (0x000001AC)
#define CONTROL_PADCONF_SYS_NIRQ (0x000001B0)
#define CONTROL_PADCONF_SAD2D_MCAD0 (0x000001B4)
#define CONTROL_PADCONF_SAD2D_MCAD2 (0x000001B8)
#define CONTROL_PADCONF_SAD2D_MCAD4 (0x000001BC)
#define CONTROL_PADCONF_SAD2D_MCAD6 (0x000001C0)
#define CONTROL_PADCONF_SAD2D_MCAD8 (0x000001C4)
#define CONTROL_PADCONF_SAD2D_MCAD10 (0x000001C8)
#define CONTROL_PADCONF_SAD2D_MCAD12 (0x000001CC)
#define CONTROL_PADCONF_SAD2D_MCAD14 (0x000001D0)
#define CONTROL_PADCONF_SAD2D_MCAD16 (0x000001D4)
#define CONTROL_PADCONF_SAD2D_MCAD18 (0x000001D8)
#define CONTROL_PADCONF_SAD2D_MCAD20 (0x000001DC)
#define CONTROL_PADCONF_SAD2D_MCAD22 (0x000001E0)
#define CONTROL_PADCONF_SAD2D_MCAD24 (0x000001E4)
#define CONTROL_PADCONF_SAD2D_MCAD26 (0x000001E8)
#define CONTROL_PADCONF_SAD2D_MCAD28 (0x000001EC)
#define CONTROL_PADCONF_SAD2D_MCAD30 (0x000001F0)
#define CONTROL_PADCONF_SAD2D_MCAD32 (0x000001F4)
#define CONTROL_PADCONF_SAD2D_MCAD34 (0x000001F8)
#define CONTROL_PADCONF_SAD2D_MCAD36 (0x000001FC)
#define CONTROL_PADCONF_SAD2D_NRESPWRON (0x00000200)
#define CONTROL_PADCONF_SAD2D_ARMNIRQ (0x00000204)
#define CONTROL_PADCONF_SAD2D_SPINT (0x00000208)
#define CONTROL_PADCONF_SAD2D_DMAREQ0 (0x0000020C)
#define CONTROL_PADCONF_SAD2D_DMAREQ2 (0x00000210)
#define CONTROL_PADCONF_SAD2D_NTRST (0x00000214)
#define CONTROL_PADCONF_SAD2D_TDO (0x00000218)
#define CONTROL_PADCONF_SAD2D_TCK (0x0000021C)
#define CONTROL_PADCONF_SAD2D_MSTDBY (0x00000220)
#define CONTROL_PADCONF_SAD2D_IDLEACK (0x00000224)
#define CONTROL_PADCONF_SAD2D_SWRITE (0x00000228)
#define CONTROL_PADCONF_SAD2D_SREAD (0x0000022C)
#define CONTROL_PADCONF_SAD2D_SBUSFLAG (0x00000230)
#define CONTROL_PADCONF_SDRC_CKE1 (0x00000234)
#define CONTROL_PADCONF_SDRC_BA0 (0x00000570)
#define CONTROL_PADCONF_SDRC_A0 (0x00000574)
#define CONTROL_PADCONF_SDRC_A2 (0x00000578)
#define CONTROL_PADCONF_SDRC_A4 (0x0000057C)
#define CONTROL_PADCONF_SDRC_A6 (0x00000580)
#define CONTROL_PADCONF_SDRC_A8 (0x00000584)
#define CONTROL_PADCONF_SDRC_A10 (0x00000588)
#define CONTROL_PADCONF_SDRC_A12 (0x0000058C)
#define CONTROL_PADCONF_SDRC_A14 (0x00000590)
#define CONTROL_PADCONF_SDRC_NCS1 (0x00000594)
#define CONTROL_PADCONF_SDRC_NRAS (0x00000598)
#define CONTROL_PADCONF_SDRC_NWE (0x0000059C)
#define CONTROL_PADCONF_SDRC_DM1 (0x000005A0)
#define CONTROL_PADCONF_SDRC_DM3 (0x000005A4)
#define CONTROL_PADCONF_ETK_CLK (0x000005A8)
#define CONTROL_PADCONF_ETK_D0 (0x000005AC)
#define CONTROL_PADCONF_ETK_D2 (0x000005B0)
#define CONTROL_PADCONF_ETK_D4 (0x000005B4)
#define CONTROL_PADCONF_ETK_D6 (0x000005B8)
#define CONTROL_PADCONF_ETK_D8 (0x000005BC)
#define CONTROL_PADCONF_ETK_D10 (0x000005C0)
#define CONTROL_PADCONF_ETK_D12 (0x000005C4)
#define CONTROL_PADCONF_ETK_D14 (0x000005C8)

/* conf pin descriptions (am335x) */
#define CONTROL_CONF_GPMC_AD0 (0x00000800)
#define CONTROL_CONF_GPMC_AD1 (0x00000804)
#define CONTROL_CONF_GPMC_AD2 (0x00000808)
#define CONTROL_CONF_GPMC_AD3 (0x0000080C)
#define CONTROL_CONF_GPMC_AD4 (0x00000810)
#define CONTROL_CONF_GPMC_AD5 (0x00000814)
#define CONTROL_CONF_GPMC_AD6 (0x00000818)
#define CONTROL_CONF_GPMC_AD7 (0x0000081C)
#define CONTROL_CONF_GPMC_AD8 (0x00000820)
#define CONTROL_CONF_GPMC_AD9 (0x00000824)
#define CONTROL_CONF_GPMC_AD10 (0x00000828)
#define CONTROL_CONF_GPMC_AD11 (0x0000082C)
#define CONTROL_CONF_GPMC_AD12 (0x00000830)
#define CONTROL_CONF_GPMC_AD13 (0x00000834)
#define CONTROL_CONF_GPMC_AD14 (0x00000838)
#define CONTROL_CONF_GPMC_AD15 (0x0000083C)
#define CONTROL_CONF_GPMC_A0 (0x00000840)
#define CONTROL_CONF_GPMC_A1 (0x00000844)
#define CONTROL_CONF_GPMC_A2 (0x00000848)
#define CONTROL_CONF_GPMC_A3 (0x0000084C)
#define CONTROL_CONF_GPMC_A4 (0x00000850)
#define CONTROL_CONF_GPMC_A5 (0x00000854)
#define CONTROL_CONF_GPMC_A6 (0x00000858)
#define CONTROL_CONF_GPMC_A7 (0x0000085C)
#define CONTROL_CONF_GPMC_A8 (0x00000860)
#define CONTROL_CONF_GPMC_A9 (0x00000864)
#define CONTROL_CONF_GPMC_A10 (0x00000868)
#define CONTROL_CONF_GPMC_A11 (0x0000086C)
#define CONTROL_CONF_GPMC_WAIT0 (0x00000870)
#define CONTROL_CONF_GPMC_WPN (0x00000874)
#define CONTROL_CONF_GPMC_BEN1 (0x00000878)
#define CONTROL_CONF_GPMC_CSN0 (0x0000087C)
#define CONTROL_CONF_GPMC_CSN1 (0x00000880)
#define CONTROL_CONF_GPMC_CSN2 (0x00000884)
#define CONTROL_CONF_GPMC_CSN3 (0x00000888)
#define CONTROL_CONF_GPMC_CLK (0x0000088C)
#define CONTROL_CONF_GPMC_ADVN_ALE (0x00000890)
#define CONTROL_CONF_GPMC_OEN_REN (0x00000894)
#define CONTROL_CONF_GPMC_WEN (0x00000898)
#define CONTROL_CONF_GPMC_BEN0_CLE (0x0000089C)
#define CONTROL_CONF_LCD_DATA0 (0x000008A0)
#define CONTROL_CONF_LCD_DATA1 (0x000008A4)
#define CONTROL_CONF_LCD_DATA2 (0x000008A8)
#define CONTROL_CONF_LCD_DATA3 (0x000008AC)
#define CONTROL_CONF_LCD_DATA4 (0x000008B0)
#define CONTROL_CONF_LCD_DATA5 (0x000008B4)
#define CONTROL_CONF_LCD_DATA6 (0x000008B8)
#define CONTROL_CONF_LCD_DATA7 (0x000008BC)
#define CONTROL_CONF_LCD_DATA8 (0x000008C0)
#define CONTROL_CONF_LCD_DATA9 (0x000008C4)
#define CONTROL_CONF_LCD_DATA10 (0x000008C8)
#define CONTROL_CONF_LCD_DATA11 (0x000008CC)
#define CONTROL_CONF_LCD_DATA12 (0x000008D0)
#define CONTROL_CONF_LCD_DATA13 (0x000008D4)
#define CONTROL_CONF_LCD_DATA14 (0x000008D8)
#define CONTROL_CONF_LCD_DATA15 (0x000008DC)
#define CONTROL_CONF_LCD_VSYNC (0x000008E0)
#define CONTROL_CONF_LCD_HSYNC (0x000008E4)
#define CONTROL_CONF_LCD_PCLK (0x000008E8)
#define CONTROL_CONF_LCD_AC_BIAS_EN (0x000008EC)
#define CONTROL_CONF_MMC0_DAT3 (0x000008F0)
#define CONTROL_CONF_MMC0_DAT2 (0x000008F4)
#define CONTROL_CONF_MMC0_DAT1 (0x000008F8)
#define CONTROL_CONF_MMC0_DAT0 (0x000008FC)
#define CONTROL_CONF_MMC0_CLK (0x00000900)
#define CONTROL_CONF_MMC0_CMD (0x00000904)
#define CONTROL_CONF_MII1_COL (0x00000908)
#define CONTROL_CONF_MII1_CRS (0x0000090C)
#define CONTROL_CONF_MII1_RX_ER (0x00000910)
#define CONTROL_CONF_MII1_TX_EN (0x00000914)
#define CONTROL_CONF_MII1_RX_DV (0x00000918)
#define CONTROL_CONF_MII1_TXD3 (0x0000091C)
#define CONTROL_CONF_MII1_TXD2 (0x00000920)
#define CONTROL_CONF_MII1_TXD1 (0x00000924)
#define CONTROL_CONF_MII1_TXD0 (0x00000928)
#define CONTROL_CONF_MII1_TX_CLK (0x0000092C)
#define CONTROL_CONF_MII1_RX_CLK (0x00000930)
#define CONTROL_CONF_MII1_RXD3 (0x00000934)
#define CONTROL_CONF_MII1_RXD2 (0x00000938)
#define CONTROL_CONF_MII1_RXD1 (0x0000093C)
#define CONTROL_CONF_MII1_RXD0 (0x00000940)
#define CONTROL_CONF_RMII1_REF_CLK (0x00000944)
#define CONTROL_CONF_MDIO (0x00000948)
#define CONTROL_CONF_MDC (0x0000094C)
#define CONTROL_CONF_SPI0_SCLK (0x00000950)
#define CONTROL_CONF_SPI0_D0 (0x00000954)
#define CONTROL_CONF_SPI0_D1 (0x00000958)
#define CONTROL_CONF_SPI0_CS0 (0x0000095C)
#define CONTROL_CONF_SPI0_CS1 (0x00000960)
#define CONTROL_CONF_ECAP0_IN_PWM0_OUT (0x00000964)
#define CONTROL_CONF_UART0_CTSN (0x00000968)
#define CONTROL_CONF_UART0_RTSN (0x0000096C)
#define CONTROL_CONF_UART0_RXD (0x00000970)
#define CONTROL_CONF_UART0_TXD (0x00000974)
#define CONTROL_CONF_UART1_CTSN (0x00000978)
#define CONTROL_CONF_UART1_RTSN (0x0000097C)
#define CONTROL_CONF_UART1_RXD (0x00000980)
#define CONTROL_CONF_UART1_TXD (0x00000984)
#define CONTROL_CONF_I2C0_SDA (0x00000988)
#define CONTROL_CONF_I2C0_SCL (0x0000098C)
#define CONTROL_CONF_MCASP0_ACLKX (0x00000990)
#define CONTROL_CONF_MCASP0_FSX (0x00000994)
#define CONTROL_CONF_MCASP0_AXR0 (0x00000998)
#define CONTROL_CONF_MCASP0_AHCLKR (0x0000099C)
#define CONTROL_CONF_MCASP0_ACLKR (0x000009A0)
#define CONTROL_CONF_MCASP0_FSR (0x000009A4)
#define CONTROL_CONF_MCASP0_AXR1 (0x000009A8)
#define CONTROL_CONF_MCASP0_AHCLKX (0x000009AC)
#define CONTROL_CONF_XDMA_EVENT_INTR0 (0x000009B0)
#define CONTROL_CONF_XDMA_EVENT_INTR1 (0x000009B4)
#define CONTROL_CONF_WARMRSTN (0x000009B8)
#define CONTROL_CONF_NNMI (0x000009C0)
#define CONTROL_CONF_TMS (0x000009D0)
#define CONTROL_CONF_TDI (0x000009D4)
#define CONTROL_CONF_TDO (0x000009D8)
#define CONTROL_CONF_TCK (0x000009DC)
#define CONTROL_CONF_TRSTN (0x000009E0)
#define CONTROL_CONF_EMU0 (0x000009E4)
#define CONTROL_CONF_EMU1 (0x000009E8)
#define CONTROL_CONF_RTC_PWRONRSTN (0x000009F8)
#define CONTROL_CONF_PMIC_POWER_EN (0x000009FC)
#define CONTROL_CONF_EXT_WAKEUP (0x00000A00)
#define CONTROL_CONF_RTC_KALDO_ENN (0x00000A04)
#define CONTROL_CONF_USB0_DRVVBUS (0x00000A1C)
#define CONTROL_CONF_USB1_DRVVBUS (0x00000A34)

#define CONTROL_CONF_SLEWCTRL (1<<6)
#define CONTROL_CONF_RXACTIVE (1<<5)
#define CONTROL_CONF_PUTYPESEL (1<<4)
#define CONTROL_CONF_PUDEN (1<<3)
#define CONTROL_CONF_MUXMODE(X) (X&0x7)

/* Raspberry Pi specific registers */
#define PADCONF_RPI2_REGISTERS_BASE 0x3F200000
#define PADCONF_RPI2_REGISTERS_OFFSET 0x0000
#define PADCONF_RPI2_REGISTERS_SIZE 0x1000

#define GPFSEL09	(0x00000000)
#define GPFSEL1019	(0x00000004)
#define GPFSEL2029	(0x00000008)
#define GPFSEL3039	(0x0000000C)
#define GPFSEL4049	(0x00000010)
#define GPFSEL5053	(0x00000014)

#define CONTROL_BCM_CONF_I2C0_SDA 	(0x00E00000)
#define CONTROL_BCM_CONF_I2C0_SCL 	(0x07000000)
#define CONTROL_BCM_CONF_I2C1_SDA 	(0x00000100)
#define CONTROL_BCM_CONF_I2C1_SCL 	(0x00000800)
#define CONTROL_BCM_CONF_PCM_CLK	(0x04000000) 
#define CONTROL_BCM_CONF_PCM_FS		(0x20000000) 
#define CONTROL_BCM_CONF_PCM_DIN	(0x00000004) 
#define CONTROL_BCM_CONF_PCM_DOUT	(0x00000020) 
#define CONTROL_BCM_CONF_PWM0		(0x00000100)
#define CONTROL_BCM_CONF_PWM1		(0x00000800)
#define CONTROL_BCM_CONF_PWM0_JACK	(0x00000004)
#define CONTROL_BCM_CONF_PWM1_JACK	(0x00000020)
#define CONTROL_BCM_CONF_SPI0_MOSI 	(0x00000004)
#define CONTROL_BCM_CONF_SPI0_MISO 	(0x10000000)
#define CONTROL_BCM_CONF_SPI0_SCLK 	(0x00000020)
#define CONTROL_BCM_CONF_SPI0_CE0 	(0x02000000)
#define CONTROL_BCM_CONF_SPI0_CE1 	(0x00400000)
#define CONTROL_BCM_CONF_SPI1_MOSI 	(0x00000003)
#define CONTROL_BCM_CONF_SPI1_MISO 	(0x0C000000)
#define CONTROL_BCM_CONF_SPI1_SCLK 	(0x00000018)
#define CONTROL_BCM_CONF_SPI1_CE0 	(0x01800000)
#define CONTROL_BCM_CONF_SPI1_CE1 	(0x00300000)
#define CONTROL_BCM_CONF_SPI1_CE2 	(0x00060000)
#define CONTROL_BCM_CONF_SPI2_MOSI 	(0x00000018)
#define CONTROL_BCM_CONF_SPI2_MISO 	(0x00000003)
#define CONTROL_BCM_CONF_SPI2_SCLK 	(0x000000C0)
#define CONTROL_BCM_CONF_SPI2_CE0 	(0x00000600)
#define CONTROL_BCM_CONF_SPI2_CE1 	(0x00003000)
#define CONTROL_BCM_CONF_SPI2_CE2 	(0x00018000)

#endif /* __MINIX_PADCONF_H */
