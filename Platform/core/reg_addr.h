#ifndef __REG_ADDR_H
#define __REG_ADDR_H
#define CORE_REVISION								0x8000
#define CORE_STEPPING								0x8001
#define CORE_KICK_START								0x8003
#define CORE_WATCH_DOG								0x8004
#define CORE_LPM_CTRL								0x8005
#define CORE_MISC_CTRL								0x8006
#define CORE_OTP_READ_DATA							0x8007
#define CORE_UART_TX_DATA							0x8008
#define CORE_UART_B_TX_DATA							0x8009
#define CORE_DMA_START								0x800a
#define CORE_SBC_CLEAR								0x800b
#define CORE_WATCH_DOG2								0x800c
#define CORE_SD_REG_WR								0x800d
#define CORE_PC_ADDRESS								0x800e
#define CORE_CMD1									0x8010
#define CORE_SYSTEM_CTRL							0x8011
#define CORE_MSBC_CTRL0								0x8012
#define CORE_MSBC_CTRL1								0x8013
#define CORE_MSBC_CTRL2								0x8014
#define CORE_QSPI_CTRL1								0x8015
#define CORE_CLK38M4_CFG							0x8016
#define CORE_BITSEND_DATA_WR						0x8017
#define CORE_SYS_EVENT_CLEAN						0x8018
#define CORE_LPM_CTRL2								0x8019
#define CORE_CLKOFF2								0x801b
#define CORE_LPM_RD_MUX								0x801c
#define CORE_IO_RST									0x801d
#define CORE_UCODE_ADDR_NX							0x8022
#define CORE_UCODE_ADDR_HI							0x8023
#define CORE_UCODE_ADDR_LO							0x8024
#define CORE_UCODE_DATA								0x8025
#define CORE_SCS_WDATA								0x8026
#define CORE_SCS_ADDR								0x802a
#define CORE_FLASH_OFFSET							0x802c
#define CORE_FLASH_READ_CMD							0x802f
#define CORE_I2S_CTRL								0x8030
#define CORE_I2S_CLKDIV								0x8031
#define CORE_I2S_RX_SADDR							0x8032
#define CORE_I2S_TX_SADDR							0x8034
#define CORE_I2S_LEN								0x8036
#define CORE_ADCD_DCOFF								0x8038
#define CORE_ADCD_DELAY								0x803a
#define CORE_ADCD_CTRL								0x803b
#define CORE_ADCD_SADDR								0x803c
#define CORE_ADCD_EADDR								0x803e
#define CORE_RTHALFSLOT								0x8040
#define CORE_CLOCK_SELECT							0x8042
#define CORE_CLOCK_CTRL								0x8043
#define CORE_EFUSE_ADDRESS							0x8044
#define CORE_SUM_CTRL								0x8045
#define CORE_EFUSE_CONTROL							0x8046
#define CORE_AUDIO_DIV_CLK_SEL						0x8047
#define CORE_LEFT_DOWN_DEST							0x8048
#define CORE_RIGHT_DOWN_DEST						0x804a
#define CORE_LPM_CTRL1								0x804c
#define CORE_CLOCK_OFF								0x8050
#define CORE_UART_BAUD_RATE							0x8052
#define CORE_UART_RX_START_ADDR						0x8054
#define CORE_UART_RX_END_ADDR						0x8056
#define CORE_UART_RX_RPTR							0x8058
#define CORE_UART_TX_START_ADDR						0x805a
#define CORE_UART_TX_END_ADDR						0x805c
#define CORE_UART_TX_WPTR							0x805e
#define CORE_UART_CONTROL							0x8060
#define CORE_GPIO_KEY								0x8061
#define CORE_SBC_SCTRL								0x8063
#define CORE_SBC_CTRL								0x8064
#define CORE_TONE_CTRL								0x8065
#define CORE_TISHI_SADDR							0x8066
#define CORE_TISHI_LEN								0x8068
#define CORE_TISHI_WPTR								0x806a
#define CORE_TISHI_CTRL								0x806c
#define CORE_LEFT_UP_DEST							0x806d
#define CORE_RIGHT_UP_DEST							0x806e
#define CORE_LEFT_STEP								0x806f
#define CORE_RIGHT_STEP								0x8070
#define CORE_UART_B_CONTROL							0x8071
#define CORE_UART_B_BAUD_RATE						0x8072
#define CORE_UARTB_RX_START_ADDR					0x8074
#define CORE_UARTB_RX_END_ADDR						0x8076
#define CORE_UARTB_RX_RPTR							0x8078
#define CORE_UARTB_TX_START_ADDR					0x807a
#define CORE_UARTB_TX_END_ADDR						0x807c
#define CORE_UARTB_TX_WPTR							0x807e
#define CORE_GPIO0_CONFIG							0x8080
#define CORE_GPIO1_GPIO31_CONFIG					0x8081
#define CORE_QSPI_CTRL								0x80a0
#define CORE_QSPI_DELAY								0x80a1
#define CORE_QSPI_TXLEN								0x80a2
#define CORE_QSPI_TXADDR							0x80a4
#define CORE_QSPI_RXADDR							0x80a6
#define CORE_QSPI_RXLEN								0x80a8
#define CORE_IICD_CTRL								0x80aa
#define CORE_IICD_SCLL								0x80ab
#define CORE_IICD_SCLH								0x80ac
#define CORE_IICD_STSU								0x80ad
#define CORE_IICD_STHD								0x80ae
#define CORE_IICD_SOSU								0x80af
#define CORE_IICD_DTSU								0x80b0
#define CORE_IICD_DTHD								0x80b1
#define CORE_IICD_TXLEN								0x80b2
#define CORE_IICD_TXADDR							0x80b4
#define CORE_IICD_RXADDR							0x80b6
#define CORE_IICD_RXLEN								0x80b8
#define CORE_NEC_STARTADDR							0x80ba
#define CORE_NEC_ENDADDR							0x80bc
#define CORE_NEC_RPTR								0x80be
#define CORE_NEC_CONTROL							0x80c0
#define CORE_DSMDIV_11M_FRAC						0x80c1
#define CORE_MIC_HPF_ALPHA							0x80c4
#define CORE_MIC_HPF_CTRL							0x80c5
#define CORE_SYS_PAUSE_CTRL0						0x80c6
#define CORE_SYS_PAUSE_CTRL1						0x80c7
#define CORE_QSPI_CTRL_XIP							0x80c8
#define CORE_QSPI_DELAY_XIP							0x80c9
#define CORE_DSMDIV_11M_FRAC						0x80c1
#define CORE_CVSD_CTRL1								0x80ca
#define CORE_BITSEND_CFG							0x80cb
#define CORE_BIST_CTRL								0x80cc
#define CORE_PWM_PCNT(x)                	 (0x80CD+x*0x5)
#define CORE_PWM_NCNT(x)                	 (0x80CF+x*0x5)
#define CORE_PWM_CTRL(x)				     (0x80D1+x*0x5)
#define CORE_CVSD_CONTROL							0x80f5
#define CORE_CVSD_SOURCE_ADDRESS					0x80f6
#define CORE_CVSD_DEST_ADDRESS						0x80f8
#define CORE_CVSD_BUFFER_LEN						0x80fa
#define CORE_PCM_SOURCE_ADDRESS						0x80fc
#define CORE_PCM_8K_DEST_ADDRESS					0x80fe
#define CORE_DAC_SOURCE_ADDRESS						0x8100
#define CORE_DAC_LEN								0x8102
#define CORE_DAC_WPTR								0x8104
#define CORE_DAC_CTRL								0x8106
#define CORE_DAC_SEL								0x8107
#define CORE_DAC_RSV								0x8109
#define CORE_SBC_SRC_START							0x810a
#define CORE_SBC_DST_START							0x810c
#define CORE_SBC_SRC_LEN							0x810e
#define CORE_SBC_DST_LEN							0x8110
#define CORE_SBC_SWP								0x8112
#define CORE_SBC_CACHE_CFG0							0x8114
#define CORE_SBC_CACHE_CFG1							0x8115
#define CORE_SBC_CTRL2								0x8116
#define CORE_SBC_FF_DITHER0							0x8117
#define CORE_PWM_CLKEN								0x8118
#define CORE_ADDA_COEF_WDATA_0						0x8119
#define CORE_ADDA_COEF_WDATA_1						0x811a
#define CORE_ADDA_COEF_WDATA_2						0x811b
#define CORE_ADC_FILTER_CTRL						0x811c
#define CORE_ADDA_COEF_WR_CTRL						0x811d
#define CORE_DAC_SEL2								0x811e
#define CORE_DAC_DC_CALI_R							0x8120
#define CORE_DAC_DC_CALI_L							0x8122
#define CORE_PCM_48K_DEST_ADDRESS					0x8124
#define CORE_CVSD_GROUP_LENGTH						0x8126
#define CORE_SBC_FF_DITHER1							0x8128
#define CORE_SBC_FF_DITHER2							0x8129
#define CORE_GPIO_CONF1								0x812a
#define CORE_UART_CTRL								0x8132
#define CORE_UARTB_CTRL								0x8135
#define CORE_RAMP_CTRL								0x8138
#define CORE_ADC_COEF_CTRL							0x8139
#define CORE_ADC_CFG_CTRL							0x813a
#define CORE_DAC_ANC_CTRL							0x813d
#define CORE_CCM_CTRL								0x813e
#define CORE_CCM_INADDR								0x813f
#define CORE_CCM_OUTADDR							0x8141
#define CORE_CCM_AAD								0x8143
#define CORE_CCM_INPTR								0x8146
#define CORE_ADC_MIC_CTRL							0x8148
#define CORE_ADC_DC_CTRL							0x8149
#define CORE_IICD1_CTRL								0x814a
#define CORE_IICD1_SCLL								0x814b
#define CORE_IICD1_SCLH								0x814c
#define CORE_IICD1_STSU								0x814d
#define CORE_IICD1_STHD								0x814e
#define CORE_IICD1_SOSU								0x814f
#define CORE_IICD1_DTSU								0x8150
#define CORE_IICD1_DTHD								0x8151
#define CORE_IICD1_TXLEN							0x8152
#define CORE_IICD1_TXADDR							0x8154
#define CORE_IICD1_RXADDR							0x8156
#define CORE_IICD1_RXLEN							0x8158
#define CORE_I2S_MRAM_SEL							0x815a
#define CORE_DAC_VOL_L								0x815b
#define CORE_DAC_VOL_R								0x815e
#define CORE_DAC_VOL_P								0x8161
#define CORE_DAC_DITHER_POW							0x8164
#define CORE_SDMA_CTRL								0x8167
#define CORE_SDMA_SRC								0x8168
#define CORE_SDMA_SRC_START							0x816a
#define CORE_SDMA_SRC_END							0x816c
#define CORE_SDMA_DST								0x816e
#define CORE_SDMA_DST_START							0x8170
#define CORE_SDMA_DST_END							0x8172
#define CORE_SDMA_LEN								0x8174
#define CORE_EQ_CTRL								0x8176
#define CORE_EQ_SRC_SADDR							0x8177
#define CORE_EQ_DST_SADDR							0x8179
#define CORE_EQ_BUF_LEN								0x817b
#define CORE_AUDIO_CLKSEL							0x817d
#define CORE_IO_RST_EN								0x817e
#define CORE_ADC_VOL								0x817f
#define CORE_DSMDIV_11M_INT							0x8182
#define CORE_DSMDIV_12M_FRAC						0x8183
#define CORE_DSMDIV_12M_INT							0x8186
#define CORE_DSMDIV_CTRL							0x8187
#define CORE_IIS_TX_WRPTR							0x8188
#define CORE_LPM_TS_CTRL							0x81f0
#define CORE_LPM_TS_CTRL2							0x81f1
#define CORE_LPM_TS_RD_MUX							0x81f2
#define CORE_LPM_TS_RD_MUX2							0x81f3
#define CORE_ICE_CONTROL							0x8203
#define CORE_ICE_STATUS								0x8204
#define CORE_ICE_BREAK_0_LO							0x8205
#define CORE_ICE_BREAK_0_HI							0x8206
#define CORE_ICE_BREAK_1_LO							0x8207
#define CORE_ICE_BREAK_1_HI							0x8208
#define CORE_CLKN									0x8300
#define CORE_EXTMCLK								0x8304
#define CORE_MISC_STATUS							0x8308
#define CORE_BITSEND_STATUS							0x8309
#define CORE_AVG_POWER								0x830a
#define CORE_UART_STATE								0x830c
#define CORE_UART_DET_BAUD							0x830d
#define CORE_UART_TX_ITEMS							0x830e
#define CORE_UART_TX_RPTR							0x8310
#define CORE_UART_RX_ITEMS							0x8312
#define CORE_UART_RX_RDPTR							0x8314
#define CORE_ADCD_ADDR								0x8316
#define CORE_LPOCNT									0x8318
#define CORE_SBC_STATUS								0x8319
#define CORE_GPIO_IN1								0x831a
#define CORE_DEBUG_BAUD								0x831b
#define CORE_GPIO_IN								0x831c
#define CORE_I2S_WRPTR								0x8320
#define CORE_I2S_RDPTR								0x8322
#define CORE_NEC_ITEMS								0x8324
#define CORE_CCM_OUTPTR								0x8326
#define CORE_QSPI_REMAIN							0x8328
#define CORE_IICD_REMAIN							0x832a
#define CORE_ADC_DATA								0x832c
#define CORE_OTP_ADDRESS							0x832e
#define CORE_SDMA_DONE								0x832f
#define CORE_DMA_STATUS								0x8330
#define CORE_PERIPHERAL_STATUS						0x8331
#define CORE_DAC_STATUS								0x8332
//#define CORE_GPIO_HIGH_WAKEUP_ENABLE				0x8333
#define CORE_TISHI_RPTR								0x8334
#define CORE_OSC_CTRL								0x8336
#define CORE_OSC_IB_CTRL							0x8337
#define CORE_OSC_CTRL1								0x8338
#define CORE_XTAL_PRE_ON_COUNTER					0x833A
#define CORE_EXEN_PRE_ON_COUNTER					0x833B
#define CORE_WAKEUP_CTRL							0x833C
#define CORE_ISOLATION_GATE							0x833D
#define CORE_BUCK_CTRL								0x833E
#define CORE_BUCK_CTRL1								0x8340
#define CORE_BUCK_PFM_DISEN							0x8341
#define CORE_GPIO_LOW_WAKEUP_ENABLE					0x8342
#define CORE_GPIO_HIGH_WAKEUP_ENABLE				0x8347
#define CORE_CCNT_COUNTER							0x834b
#define CORE_BIST_DONE								0x834e
#define CORE_BIST_FAIL								0x8350
#define CORE_ADC_SUM_RESULT							0x8352
#define CORE_UART_B_STATE							0x8354
#define CORE_UART_B_DET_BAUD						0x8355
#define CORE_UART_B_TX_ITEMS						0x8356
#define CORE_UART_B_TX_RPTR							0x8358
#define CORE_UART_B_RX_ITEMS						0x835a
#define CORE_UART_B_RX_WPTR							0x835c
#define CORE_SCS_RDATA								0x835e
#define CORE_PCM_IN_PTR								0x8362
#define CORE_PCM_8K_OUT_PTR							0x8364
#define CORE_CVSD_IN_PTR							0x8366
#define CORE_CVSD_OUT_PTR							0x8368
#define CORE_DAC_READ_PTR							0x836a
#define CORE_SBC_SOURCE_READ_PTR					0x836c
#define CORE_SBC_DST_WRITE_PTR						0x836e
#define CORE_EXE_ADDR								0x8370
#define CORE_PWM_INT								0x8373
#define CORE_SYS_WAKEUP_INT_STATUS0					0x8374
#define CORE_SYS_WAKEUP_INT_STATUS1					0x8375
#define CORE_PCM_48K_OUT_PTR						0x8376
#define CORE_SD_REG_RDATA							0x8378
#define CORE_CHGR_EVENT								0x8379
#define CORE_VOICE_DET_STATUS						0x837b
#define CORE_VOICE_DET_WINDOW_RECORD				0x837c
#define CORE_LPM_STATUS_OTHERS						0x8380
#define CORE_ADC_AVG_DC								0x8384
#define CORE_LPM_TS_BASELINE						0x8386
#define CORE_LPM_TS_CFG_STATUS						0x8389
#define CORE_LPM_TS_FIR2_DATA						0x838d
#define CORE_IICD1_REMAIN							0x8390
#define CORE_SDMA_REMAIN							0x8392
#define CORE_SDMA_WADDR								0x8394
#define CORE_SDMA_RADDR								0x8396
#define CORE_EQ_RPTR								0x8398
#define CORE_EQ_WPTR								0x839a
#define CORE_LPM_TS_STATUS							0x839c
#define CORE_DAC_COEF_CHKSUM						0x839d
#define CORE_ADC_COEF_CHKSUM						0x839e
#define CORE_NEC_WPTR								0x839f
#define CORE_LDO_CTRL								0x83a1
#define CORE_LPM_STATUS1							0x83a5
#define CORE_EQ_COEF_CHKSUM							0x83a6
#define CORE_CVSD_COE_CHKSUM						0x83a7
#define CORE_B_BOX									0x8400
#define CORE_E_BOX									0x8500
#define CORE_L_BOX									0x8600
#define CORE_MEM_K_FOR_SHA							0x8700
#define CORE_SPI_CTRL								0x8800
#define CORE_SPI_FIFO								0x8804
#define CORE_SPI_FIFOTDATA							0x8806
#define CORE_SPI_FIFORDATA							0x8807
#define CORE_SPI_DMA								0x8808
#define CORE_SPI_IRQRS								0x880c
#define CORE_SPI_IRQMS								0x880d
#define CORE_SPI_IRQMEN								0x880e
#define CORE_SPI_DMA_TX_SADDR						0x8810
#define CORE_SPI_DMA_RX_SADDR						0x8814
#define CORE_SPI_DMA_TX_LEN							0x8818
#define CORE_SPI_DMA_RX_LEN							0x881c
#define CORE_SPI_DMA_TX_ADDR						0x8820
#define CORE_SPI_DMA_RX_ADDR						0x8824
#define CORE_SPI_FIFO_CTRL							0x8830
#define CORE_USB_CONFIG								0x8c00
#define CORE_USB_IRQ_MASK							0x8c01
#define CORE_USB_ADDR								0x80c3
#define CORE_USB_RX_SADDR							0x8c04
#define CORE_USB_RX_LEN								0x8c07
#define CORE_USB_TX_SADDR0							0x8c0a
#define CORE_USB_TX_SADDR1							0x8c0c
#define CORE_USB_TX_SADDR2							0x8c0e
#define CORE_USB_TX_SADDR3							0x8c10
#define CORE_USB_HMODE								0x8c12
#define CORE_USB_TRIG								0x8c20
#define CORE_USB_STALL								0x8c21
#define CORE_USB_TOGGLE								0x8c22
#define CORE_USB_RX_WPTR							0x8c30
#define CORE_IRQ_status								0x8c32
#define CORE_STALL_status							0x8c34
#define CORE_USB_TX_BUSY							0x8c35
#define CORE_USB_SOF								0x8c36
#define CORE_USB_CCNT								0x8c38
#define CORE_RXCNT									0x8c3a
#define CORE_DAC_ECHO_IIR_SCALE						0x8d00
#define CORE_DAC_ECHO_IIR_COEF						0x8d03
#define CORE_DAC_FB_IIR_SCALE						0x8d99
#define CORE_DAC_FB_IIR_COEF						0x8d9c
#define CORE_LPM_REG 								0x804c
#define CORE_LPM_WR                          		0x8005
#define CORE_LPM_WR2	                     		0x8019
#define CORE_ICE_WDT_MODE  							0x83a8

#define DMA_CTRL									0x1f038
#define DMA_TASK_TABLE_ADDR							0x1f03c
#define DMA_TASK_VAL								0x1f040
#define DMA_TASK_STATUS								0x1f048
#define DMA_TASK_INT_STATUS							0x1f04c
#define DMA_TASK_INT_CLEAR							0x1f044

#define	CORE_QSPI1_TX_CTRL							0x1f068
#define	CORE_QSPI1_RX_CTRL							0x1f06c
#define	CORE_QSPI1_CTRL								0x1f070
#define	CORE_QSPI1_CMD_CTRL							0x1f074
#define	CORE_QSPI1_START							0x1f078
#define	CORE_QSPI1_STATUS							0x1f07c
#define	CORE_QSPI2_TX_CTRL							0x1f080
#define	CORE_QSPI2_RX_CTRL							0x1f084
#define	CORE_QSPI2_CTRL								0x1f088
#define	CORE_QSPI2_CMD_CTRL							0x1f08c
#define	CORE_QSPI2_START							0x1f090
#define	CORE_QSPI2_STATUS							0x1f094

#define CORE_GPIO_IEN_L								0x1f0a0
#define CORE_GPIO_IEN_H								0x1f0a4
#define CORE_GPIO_ICR_L								0x1f0a8
#define CORE_GPIO_ICR_H								0x1f0ac
#define CORE_GPIO_FTS_L								0x1f0b0
#define CORE_GPIO_FTS_H								0x1f0b4
#define CORE_GPIO_RTS_L								0x1f0b8
#define CORE_GPIO_RTS_H								0x1f0bc
#define CORE_GPIO_LTS_L								0x1f0c0
#define CORE_GPIO_LTS_H								0x1f0c4
#define CORE_GPIO_ISR_L								0x1f0c8
#define CORE_GPIO_ISR_H								0x1f0cc

#define CORE_CLKPLL_CFG6  							0X8a09
#define CORE_RF_IDO_EN0 							0x8a0a
#define CORE_RF_IDO_EN1 							0x8a0b
#define CORE_RF_IDO_CFG5 							0x8a11
#define CORE_RF_IDO_CFG6 							0x8a12
#define CORE_SYN_EN 								0x8a16
#define CORE_RX_EN0									0x8a17
#define CORE_GPADC_CTRL 							0x8a97

#define CORE_codec_ldo_en0                          0x8b00
#define CORE_codec_ldo_cfg0                         0x8b01
#define CORE_codec_ldo_cfg1                         0x8b02
#define CORE_codec_ldo_cfg2                         0x8b04
#define CORE_codec_vmid_en0                         0x8b05
#define CORE_codec_vmid_cfg0                        0x8b06
#define CORE_codec_vmid_cfg1                        0x8b07
#define CORE_codec_vmid_cfg2                        0x8b08
#define CORE_codec_vmid_cfg3                        0x8b09
#define CORE_codec_vmid_ind                         0x8b0a
#define CORE_au_adc_en0                             0x8b0b
#define CORE_au_adc_en1                             0x8b0c
#define CORE_au_adc_cfg0                            0x8b0d
#define CORE_au_adc_cfg1                            0x8b0e
#define CORE_au_adc_cfg2                            0x8b0f
#define CORE_au_adc_cfg3                            0x8b10
#define CORE_au_adc_cfg4                            0x8b11
#define CORE_au_adc_cfg5                            0x8b12
#define CORE_au_adc_cfg6                            0x8b13
#define CORE_au_dac_en0                             0x8b14
#define CORE_au_dac_en1                             0x8b15
#define CORE_au_dac_en2                             0x8b16
#define CORE_au_dac_en3                             0x8b17
#define CORE_au_dac_cfg0                            0x8b18
#define CORE_au_dac_cfg1                            0x8b19
#define CORE_au_dac_cfg2                            0x8b1a
#define CORE_au_dac_cfg3                            0x8b1b
/***********************************lpm_write*************************************************/
#define LPMREG_SEL_CTRL             (0x01)
#define LPMREG_SEL_CTRL2            (0x02)
#define LPMREG_SEL_GPIOLOW_WAKE     (0x04)
#define LPMREG_SEL_GPIOHIGH_WAKE    (0x08)
#define LPMREG_SEL_COUNTER          (0x10)
#define LPMREG_SEL_CTRL3            (0x20)
#define LPMREG_SEL_BUCK_CFG         (0x20)
#define LPMREG_SEL_CHARGE_CTRL      (0x40)
#define LPMREG_SEL_XTAL32K_CTRL     (0x80)


/***********************************lpm_write2*************************************************/
#define LPMREG2_SEL_ENTER_SCAN_MODE             (0x01)
#define LPMREG2_SEL_CHARGE_LOW_HIGH_WAKEUP_REG  (0x02)
#define LPMREG2_SEL_LDO_CONTROL                 (0x04)
#define LPMREG2_SEL_GPIO2_TOUCH_WAKE            (0x08)
#define LPMREG2_SEL_ICE_WDT_LONG_RST_MODE       (0x80)


/***********************************UART*************************************************/
#define CORE_UART_CONTROL_BASE(x)      		((CORE_UART_CONTROL + (x)*0x11))
#define CORE_UART_BAUD_BASE(x)      		((CORE_UART_BAUD_RATE + (x)*0x20))
#define CORE_UART_TxItems_BASE(x)      		((CORE_UART_TX_ITEMS + (x)*0x48))
#define CORE_UART_RxItems_BASE(x)      		((CORE_UART_RX_ITEMS + (x)*0x48))
#define CORE_UART_CTRL_BASE(x)      		((CORE_UART_CTRL + (x)*0x03))
#define CORE_UART_STATE_BASE(x)      		((CORE_UART_STATE + (x)*0x48))

/***********************************IIC************************************************/
#define CORE_IICD_CTRL_BASE(x)				((CORE_IICD_CTRL + (x)*0xA0))
#define CORE_IICD_TXLEN_BASE(x)				((CORE_IICD_TXLEN +(x)*0xA0))
#define CORE_IICD_TXADDR_BASE(x)			((CORE_IICD_TXADDR +(x)*0xA0))
#define CORE_IICD_RXADDR_BASE(x)			((CORE_IICD_RXADDR + (x)*0xA0))
#define CORE_IICD_RXLEN_BASE(x)				((CORE_IICD_RXLEN + (x)*0xA0))

/***********************************WDT************************************************/
#define CORE_WDT_COUNTER_BASE(x)				((CORE_WATCH_DOG + (x)*0x08))

/***********************************GPIO*************************************************/
#define reg_deal_gpiox(gpiox)				(1 << (gpiox & 0x1f))

#define CORE_GPIO_CONFIG_BASE(gpiopin_check)      		((CORE_GPIO0_CONFIG + (gpiopin_check > 31)*0xaa) + (gpiopin_check & 0x1f))
#define CORE_GPIO_IN_BASE(gpiopin_check)      			((CORE_GPIO_IN - (gpiopin_check > 31)*0x02))
#define CORE_GPIO_INTER_BASE(gpio_addr, gpiopin_check)      	(((gpio_addr) + (gpiopin_check > 31)*0x04))
#define CORE_GPIO_WAKEUP_LOW_BASE(gpio_addr, gpiopin_check)     	(((gpio_addr) + (gpiopin_check > 31)*0x0a))
#define CORE_GPIO_WAKEUP_HIGH_BASE(gpio_addr, gpiopin_check)     	(((gpio_addr) + (gpiopin_check > 31)*0x05))

/***********************************QSPI*************************************************/
#define CORE_QSPI_CONFIG_BASE(x, y)      		(((x) + (y)*0x18))




#endif

