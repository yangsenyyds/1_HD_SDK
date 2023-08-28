#ifndef __REG_STRUCT_H
#define __REG_STRUCT_H

#define _IO	volatile
#define _O	volatile
#define _I	volatile


typedef struct
{
    _O unsigned char kick_start					: 1;	//W 0.0		//Write to restart PC to 1
    _IO unsigned char kick_start_reserved		: 7;	//RW 0.0		//not used
} __attribute__((packed)) KICK_STARTRegDef;		//CORE_KICK_START	0x8003

typedef struct
{
    _O unsigned char lpm_ctrl0					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into 8336-8339
    _O unsigned char lpm_ctrl1					: 1;	//W 10.0		//Write 1 to write 804c-804f contents into 833a-833d
    _O unsigned char lpm_gpio_wk_low			: 1;	//W 0.0		//Write 1 to write 804c-804f contents into 8342-8345(gpio low wakeup)
    _O unsigned char lpm_gpio_wk_high			: 1;	//W 0.0		//Write 1 to write 804c-804f contents into 8347-834a(gpio high wakeup)
    _O unsigned char lpm_counter				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into lpm counter and enter LPM mode
    _O unsigned char lpm_ctrl2					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into 833e-8341
    _O unsigned char lpm_chgr_ctrl				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into lpm charger control
    _O unsigned char lpm_xtal_32k_ctrl			: 1;	//W 0.0		//Write 1 to write 804c-804f contents into lpm xtal 32k control
} __attribute__((packed)) LPM_CTRLRegDef;		//CORE_LPM_CTRL	0x8005

typedef struct
{
    _O unsigned char icache_init				: 1;	//W 0.0		//Write 1 to init cm0 icache
    _O unsigned char icache_match_sts			: 1;	//W 0.0		//Write 1 to set cm0 icache into match state
    _O unsigned char mcu_encryption_en			: 1;	//W 0.0		//Write 1 to set cm0 code encryption key
    _O unsigned char lock_otp					: 1;	//W 0.0		//Write 1 to lock otp security data block
    _IO unsigned char misc_ctrl_reserved		: 4;	//RW 0.0		//not used
} __attribute__((packed)) MISC_CTRLRegDef;		//CORE_MISC_CTRL	0x8006

typedef struct
{
    _O unsigned char otp_dma_start				: 1;	//W 0.0		//Write 1 to start otp dma
    _O unsigned char spi_dma_start				: 1;	//W 0.0		//Write 1 to start spi dma
    _O unsigned char iic_dma_start				: 1;	//W 0.0		//Write 1 to start iic dma
    _O unsigned char qspi_dma_start				: 1;	//W 0.0		//Write 1 to start qspi dma
    _O unsigned char iic1_dma_start				: 1;	//W 0.0		//Write 1 to start iic dma
    _O unsigned char soft_dma_start				: 1;	//W 0.0		//Write 1 to start soft dma
    _O unsigned char dma_start_reserved			: 1;	//W 0.0		//not used
    _O unsigned char lpo_cali_start				: 1;	//W 0.0		//Write 1 to start lpo calibration counter
} __attribute__((packed)) DMA_STARTRegDef;		//CORE_DMA_START	0x800a

typedef struct
{
    _O unsigned char sbc_msbc_x_wp_clr			: 1;	//W 0.0		//msbc x wp clear
    _O unsigned char sbc_src_addr_clr			: 1;	//W 0.0		//source address clear
    _O unsigned char sbc_dst_addr_clr			: 1;	//W 0.0		//destination address clear
    _O unsigned char sbc_v_offset_clr			: 1;	//W 0.0		//v offset clear
    _O unsigned char sbc_sat_record_clr			: 1;	//W 0.0		//sat record clear
    _O unsigned char sbc_error_clr				: 1;	//W 0.0		//error clear
    _O unsigned char sbc_v_matrix_clr			: 1;	//W 0.0		//v matrix claar
    _O unsigned char sbc_farrow_filter_clr		: 1;	//W 0.0		//farrow filter clear
} __attribute__((packed)) SBC_CLEARRegDef;		//CORE_SBC_CLEAR	0x800b

typedef struct
{
    _O unsigned char sd_reg_wr					: 1;	//W 0.0		//sdio register write strobe
    _IO unsigned char sd_reg_wr_reserved		: 7;	//RW 0.0		//not used
} __attribute__((packed)) SD_REG_WRRegDef;		//CORE_SD_REG_WR	0x800d

typedef struct
{
    _O unsigned char cmd1_sft_rst				: 1;	//W 0.0		//sft_rst :Reset System
    _O unsigned char cmd1_mcu_rst				: 1;	//W 0.0		//mcu_rst: Reset MCU
    _IO unsigned char cmd1_reserved				: 6;	//RW 0.0		//not used
} __attribute__((packed)) CMD1RegDef;		//CORE_CMD1	0x8010

typedef struct
{
    _IO unsigned char sys_encryption_en			: 1;	//RW 0.0		//1: enable encryption;
    _IO unsigned char wdt_int_en				: 1;	//RW 0.0		//Watchdog interrupt mode enable
    _IO unsigned char ret_ram_cs_mode			: 1;	//RW 0.0		//retention ram cs mode, 1 is hardware auto control
    _O unsigned char vbat_change_flag_clr		: 1;	//W 0.0		//vbat change flag clear
    _IO unsigned char ret_ram_clk_en			: 1;	//RW 0.0		//retention ram clock enable
    _IO unsigned char nec_in_sel				: 1;	//RW 0.0		//0:ir select from gpio
    _IO unsigned char qspi_mram_async			: 1;	//RW 1.0		//qspi mram async mode
    _IO unsigned char dma_xor_en				: 1;	//RW 0.0		//dma xor aes result enable
} __attribute__((packed)) SYSTEM_CTRLRegDef;		//CORE_SYSTEM_CTRL	0x8011

typedef struct
{
    _O unsigned char msbc_encode_sample_rate	: 2;	//WO 0.0		//msbc encode sampling frequency
    _O unsigned char msbc_enc_block_num			: 5;	//WO 0.0		//msbc encode number of blocks
    _O unsigned char msbc_encode_alloc_method	: 1;	//WO 0.0		//msbc encode allocation method
} __attribute__((packed)) MSBC_CTRL1RegDef;		//CORE_MSBC_CTRL1	0x8013

typedef struct
{
    _O unsigned char msbc_enc_block_num_sel		: 2;	//WO 0.0		//msbc encode number of blocks select
    _O unsigned char msbc_enc_subband_num		: 4;	//WO 0.0		//msbc encode number of subbands
    _O unsigned char msbc_enc_channel_num		: 2;	//WO 0.0		//msbc encode number of channels
} __attribute__((packed)) MSBC_CTRL2RegDef;		//CORE_MSBC_CTRL2	0x8014

typedef struct
{
    _IO unsigned char qspi_sample_edge_sel		: 1;	//RW 0.0		//1:qspi?????????0??qspi???????
    _IO unsigned char qspi_dma_insert_en		: 1;	//RW 0.0		//1:dma task insert enable, 0:dma task wait xip task done
    _IO unsigned char qspi_dma_config_en		: 1;	//RW 0.0		//1:use dma configuration,0:configuration select by dma and xip
    _IO unsigned char qspi_sck_gate_en			: 1;	//RW 0.0		//1:sck gate enable,0:sck gate disable
    _IO unsigned char qspi_rdata_delay			: 2;	//RW 0.0		//qspi read data delay: 0: 16 mcu clk cycles 1: 32 mcu clk cycles 2: 48 mcu clk cycles 3: 64 mcu clk cycles
    _IO unsigned char qspi_clock_sel			: 2;	//RW 0.0		//spi clock select: 0: 24M 1: 48M 2: 64M 3: 96M
} __attribute__((packed)) QSPI_CTRL1RegDef;		//CORE_QSPI_CTRL1	0x8015

typedef struct
{
    _IO unsigned char clk38m4_high_duty			: 2;	//RW 0.0		//38.4m??????????
    _IO unsigned char clk38m4_low_duty			: 2;	//RW 0.0		//38.4m??????????
    _IO unsigned char clk38m4_dpll_38m4_en		: 1;	//RW 0.0		//dpll 38m4 enable
    _IO unsigned char clk38m4_cfg_reserved		: 3;	//RW 0.0		//not used
} __attribute__((packed)) CLK38M4_CFGRegDef;		//CORE_CLK38M4_CFG	0x8016

typedef struct
{
    _O unsigned char bitsend_data_wr			: 1;	//WO 0.0		//nan
    _IO unsigned char bitsend_data_wr_reserved	: 7;	//RW 0.0		//not used
} __attribute__((packed)) BITSEND_DATA_WRRegDef;		//CORE_BITSEND_DATA_WR	0x8017

typedef struct
{
    _IO unsigned char wdt_err_clr				: 1;	//RW 0.0		//watch dog  error clean
    _IO unsigned char wdt2_err_clr				: 1;	//RW 0.0		//watch dog 2 error clean
    _IO unsigned char chgr_rst_clr				: 1;	//RW 0.0		//charger reset clean
    _IO unsigned char sys_event_clean_reserved	: 5;	//RW 0.0		//not used
} __attribute__((packed)) SYS_EVENT_CLEANRegDef;		//CORE_SYS_EVENT_CLEAN	0x8018

typedef struct
{
    _O unsigned char scan_mode_enable			: 1;	//W 0.0		//Write 1 to write 804c-804f contents into deaddead then into scan mode
    _O unsigned char chgr_wk_ctrl				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into charger event (low wake and high wake)
    _O unsigned char vad_ctrl					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into voice detect control
    _O unsigned char misc_ctrl					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into misc control and gpio wake control for GPIO[39:32]
    _O unsigned char adc_ctrl					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into ADC control low 32 bits
    _O unsigned char adc_ctrl1					: 1;	//W 0.0		//Write 1 to write 804c-804f contents into ADC control high 6 bits
    _IO unsigned char lpm_ctrl2_reserved		: 2;	//RW 0.0		//not used
} __attribute__((packed)) LPM_CTRL2RegDef;		//CORE_LPM_CTRL2	0x8019

typedef struct
{
    _IO unsigned char eq_clkoff					: 1;	//RW 0.0		//nan
    _IO unsigned char mic_clkoff				: 1;	//RW 0.0		//nan
    _IO unsigned char clkoff2_reserved			: 6;	//RW 0.0		//not used
} __attribute__((packed)) CLKOFF2RegDef;		//CORE_CLKOFF2	0x801b

typedef struct
{
    _IO unsigned char lpm_reg_rd_mux			: 6;	//RW 0.0		//lpm other status registers read mux
    _IO unsigned char lpm_rd_mux_reserved		: 2;	//RW 0.0		//not used
} __attribute__((packed)) LPM_RD_MUXRegDef;		//CORE_LPM_RD_MUX	0x801c

typedef struct
{
    _IO unsigned char ucode_addr_nx				: 3;	//RW 0.0		//Host_addr[14:12]: ucode program address highest byte
    _IO unsigned char ucode_addr_nx_reserved	: 5;	//RW 0.0		//not used
} __attribute__((packed)) UCODE_ADDR_NXRegDef;		//CORE_UCODE_ADDR_NX	0x8022

typedef struct
{
    _IO unsigned char ucode_host_addr			: 6;	//RW 0.0		//Host_addr[11:6]: ucode address both write and read are in bytes address [13:8]
    _IO unsigned char ucode_boot_host_read_en	: 1;	//RW 0.0		//Boot_host_read_enable: boot rom host read enable
    _IO unsigned char ucode_host_en				: 1;	//RW 0.0		//Ucode_host_enable: Writes to ucode data is only valid with this bit set, host to reset this bit before operation starts. ucode address & data = all 0??s when bit is set.
} __attribute__((packed)) UCODE_ADDR_HIRegDef;		//CORE_UCODE_ADDR_HI	0x8023

typedef struct
{
    _IO unsigned char ucode_addr_lo_reserved	: 2;	//RW 0.0		//not used
    _IO unsigned char ucode_addr_lo				: 6;	//RW 0.0		//Host_addr [5:0]: The value of this register is the bit [7:0] of byte address during read. Set bit [5:0] of word address as ucode_addr_lo[7:2] during write, and bit-0 and bit-1 are not cared.
} __attribute__((packed)) UCODE_ADDR_LORegDef;		//CORE_UCODE_ADDR_LO	0x8024

typedef struct
{
    _IO unsigned char i2s_en					: 1;	//RW 0.0		//I2S eanble
    _IO unsigned char i2s_rx_en					: 1;	//RW 0.0		//I2S rx eanble
    _IO unsigned char i2s_mode_sel				: 2;	//RW 0.0		//mode select: 00: standard i2s mode; 01: pcm mode; 10: left mode; 11: right mode
    _IO unsigned char i2s_bit_mode_sel			: 2;	//RW 0.0		//bit mode select, 00: 16 bit mode; 01: 24 bit mode; 10: 32 bit mode; 11: 64 bit mode;
    _IO unsigned char i2s_24bit_mode			: 1;	//RW 0.0		//24 bit mode
    _IO unsigned char i2s_master_mode			: 1;	//RW 0.0		//1:master mode
} __attribute__((packed)) I2S_CTRLRegDef;		//CORE_I2S_CTRL	0x8030

typedef struct
{
    _IO unsigned char adcd_delay				: 7;	//RW 0.0		//adc channel switch delay counter, based on 3M
    _IO unsigned char adcd_vof_blk_en			: 1;	//RW 0.0		//voice filter block enable
} __attribute__((packed)) ADCD_DELAYRegDef;		//CORE_ADCD_DELAY	0x803a

typedef struct
{
    _IO unsigned char adcd_rssi_avg_time		: 2;	//RW 0.0		//average time for rssi
    _IO unsigned char adcd_rssi_clr				: 1;	//RW 0.0		//clear rssi
    _IO unsigned char adcd_rssi_en				: 1;	//RW 0.0		//rssi enable
    _IO unsigned char adcd_cic_ds_sel			: 1;	//RW 0.0		//cic_ds_sel: 0: down sample 32; 1: down sample 16
    _IO unsigned char adcd_data_sel				: 1;	//RW 0.0		//1:raw adc data, 0:voice data
    _IO unsigned char adcd_mram_sel				: 1;	//RW 0.0		//mram selection, 1: mram; 0: schedule ram/xram
    _IO unsigned char adcd_dma_en				: 1;	//RW 0.0		//adc DMA enable
} __attribute__((packed)) ADCD_CTRLRegDef;		//CORE_ADCD_CTRL	0x803b

typedef struct
{
    _IO unsigned char sys_clock_src				: 2;	//RW 0.0		//system clock source. 0:crystal, 1: dpll, 2:rc, 3:lpoclk
    _IO unsigned char sys_clock_div				: 3;	//RW 0.0		//system clock division. 0-7, 2 to 256
    _IO unsigned char cp_clock_sel				: 1;	//RW 0.0		//dpll clock 96m selection  (cp clock select. 0:rc, 1:dpll)
    _IO unsigned char pd_sel					: 1;	//RW 0.0		//1: Powerdown DPLL/XTAL when crystal is off
    _IO unsigned char keyscan_clock_sel			: 1;	//RW 0.0		//keyscan clock select. 0:lpo, 1:rcclk divided by 256
} __attribute__((packed)) CLOCK_SELECTRegDef;		//CORE_CLOCK_SELECT	0x8042

typedef struct
{
    _IO unsigned char uart_clock_sel			: 1;	//RW 0.0		//UART clock select. 0: crystal 1: DPLL 48Mhz
    _IO unsigned char mcu_clk_sel				: 2;	//RW 0.0		//mcu_clk_sel 0: 48M 1: 64M 2: 96M 3: 192M
    _IO unsigned char res						: 2;
    _IO unsigned char whiten_off				: 1;	//RW 0.0		//1: whiten off
    _IO unsigned char test_mode					: 2;	//RW 0.0		//1: radio test mode, 2: ADC test mode
} __attribute__((packed)) CLOCK_CTRLRegDef;		//CORE_CLOCK_CTRL	0x8043

typedef struct
{
    _IO unsigned char sum_ctrl_reserved			: 1;	//RW 0.0		//not used
    _IO unsigned char modem_test_mux			: 3;	//RW 0.0		//adc test mux
    _IO unsigned char sum_ctrl					: 4;	//RW 0.0		//sum control
} __attribute__((packed)) SUM_CTRLRegDef;		//CORE_SUM_CTRL	0x8045

typedef struct
{
    _IO unsigned char efuse_addr_en				: 1;	//RW 0.0		//efuse address enable
    _IO unsigned char efuse_read_mode_en		: 1;	//RW 0.0		//efuse read mode enable
    _IO unsigned char efuse_prog_mode_en		: 1;	//RW 0.0		//efuse program mode enable
    _IO unsigned char efuse_key_xor_en			: 1;	//RW 0.0		//key xor enable
    _IO unsigned char efuse_dma_delay			: 2;	//RW 0.0		//OTP dma delay
    _IO unsigned char efuse_control_reserved	: 2;	//RW 0.0		//not used
} __attribute__((packed)) EFUSE_CONTROLRegDef;		//CORE_EFUSE_CONTROL	0x8046

typedef struct
{
    _IO unsigned char auth_clk_en				: 1;	//RW 0.0		//auth_clk_en
    _IO unsigned char mcu_clk256m_en			: 1;	//RW 0.0		//mcu clk256m enable
    _IO unsigned char i2s_divclk_sel			: 2;	//RW 0.0		//i2s divide clock select, 0: divide 1; 1: divide 2; 2: divide 3;
    _IO unsigned char dac_divclk_sel			: 2;	//RW 0.0		//dac divide clock select, 0: divide 1; 1: divide 2; 2: divide 3;
    _IO unsigned char adc_divclk_sel			: 2;	//RW 0.0		//adc divide clock select, 0: divide 1; 1: divide 2; 2: divide 3;
} __attribute__((packed)) AUDIO_DIV_CLK_SELRegDef;		//CORE_AUDIO_DIV_CLK_SEL	0x8047

typedef struct
{
    _IO unsigned short auth_rom_off				: 1;	//RW 0.0		//1: turn off auth rom's clock
    _IO unsigned short iotp_off					: 1;	//RW 0.0		//1: turn off iotp clock
    _IO unsigned short debug_uart_off			: 1;	//RW 0.0		//1: turn off debug UART's clock
    _IO unsigned short itrace_off				: 1;	//RW 0.0		//1: turn off itrace's clock
    _IO unsigned short qspi_off					: 1;	//RW 0.0		//1: turn off qspi's clock
    _IO unsigned short dac_off					: 1;	//RW 0.0		//1: turn off dac's clock
    _IO unsigned short vof_off					: 1;	//RW 0.0		//1: turn off voice filter's clock
    _IO unsigned short iic_off					: 1;	//RW 0.0		//1: turn off IIC's clock
    _IO unsigned short mcu_off					: 1;	//RW 0.0		//1: turn off cm0's clock
    _IO unsigned short mram_off					: 1;	//RW 0.0		//1: turn off MRAM's clock
    _IO unsigned short usb_off					: 1;	//RW 0.0		//1: turn off USB's clock
    _IO unsigned short sbc_off					: 1;	//RW 0.0		//1: turn off sbc's clock
    _IO unsigned short spi_off					: 1;	//RW 0.0		//1: turn off SPI's clock
    _IO unsigned short sdio_off					: 1;	//RW 0.0		//1: turn off SDIO's clock
    _IO unsigned short i2s_off					: 1;	//RW 0.0		//1: turn off I2S clock
    _IO unsigned short uart_off					: 1;	//RW 0.0		//1: turn off UART's clock
} __attribute__((packed)) CLOCK_OFFRegDef;		//CORE_CLOCK_OFF	0x8050

typedef struct
{
    _IO unsigned short uart_baud_rate			: 15;	//RW 0.0		//UART baud rate divisor by uart clock
    _IO unsigned short uart_mram_sel				: 1;	//RW 0.0		//UART DMA mram selection
} __attribute__((packed)) UART_BAUD_RATERegDef;		//CORE_UART_BAUD_RATE	0x8052

typedef struct
{
    _IO unsigned char uart_en					: 1;	//RW 0.0		//Enable UART
    _IO unsigned char uart_parity_sel			: 1;	//RW 0.0		//1: odd parity; 0: even parity
    _IO unsigned char uart_data_len				: 1;	//RW 0.0		//UART Data length 1: 9 bits; 0: 8 bits
    _IO unsigned char uart_stop_bit				: 1;	//RW 0.0		//1: stop bit is 2, 0:stop bit is 1
    _IO unsigned char uart_flow_ctrl_en			: 1;	//RW 0.0		//1: enable RTS/CTS flow control
    _IO unsigned char uart_7816_en				: 1;	//RW 0.0		//1: enable 7816 smart card protocol
    _IO unsigned char uart_half_duplex_en		: 1;	//RW 0.0		//1: enable half duplex single line UART
    _IO unsigned char uart_baud_mode_sel		: 1;	//RW 0.0		//1: Use baud settings 0:auto detect baud rate
} __attribute__((packed)) UART_CONTROLRegDef;		//CORE_UART_CONTROL	0x8060

typedef struct
{
    _IO unsigned char sbc_sctrl_reserved		: 2;	//RW 0.0		//not used
    _IO unsigned char cache_ctrl				: 2;	//RW 0.0		//nan
    _IO unsigned char sbc_ff_48k_en				: 1;	//RW 0.0		//enable farrow filter for sbc in 48k->48k mode
    _IO unsigned char msbc_dec_en				: 1;	//RW 0.0		//enable msbc decode
    _IO unsigned char sbc_eq_en					: 1;	//RW 0.0		//enable EQ, for SBC decoder when sampling frequency is 44.1k/48k
    _IO unsigned char sum_en					: 1;	//RW 0.0		//enable ADC sum
} __attribute__((packed)) SBC_SCTRLRegDef;		//CORE_SBC_SCTRL	0x8063

typedef struct
{
    _IO unsigned char sbc_block_en				: 1;	//RW 0.0		//sbc block enable
    _IO unsigned char sbc_max_bitneed_mode		: 1;	//RW 0.0		//sbc max bitneed mode
    _IO unsigned char sbc_src_bit16				: 1;	//RW 0.0		//sbc source buffer bit 16
    _IO unsigned char sbc_dst_bit16				: 1;	//RW 0.0		//sbc destination buffer bit 16
    _IO unsigned char msbc_gain_ctrl			: 3;	//RW 0.0		//msbc gain control
    _IO unsigned char msbc_en					: 1;	//RW 0.0		//enable msbc encode
} __attribute__((packed)) SBC_CTRLRegDef;		//CORE_SBC_CTRL	0x8064

typedef struct
{
    _IO unsigned char tone_ctrl_reserved		: 4;	//RW 0.0		//not used
    _IO unsigned char tone_ff_dither			: 2;	//RW 0.0		//farrow filter dither[25:24]
    _IO unsigned char tone_sbc_ff_en			: 1;	//RW 0.0		//enable farrow filter for sbc, default in 44.1->48k mode when sampling frequency is 44.1k, If sampling frequency is 48k, 'sbc_ff_48k_en' needs to be set to '1' to enable farrow filter
    _IO unsigned char tone_no_toggle			: 1;	//RW 0.0		//1: no_toggle, Do not send leading alternating 1??s & 0??s prior to real packet
} __attribute__((packed)) TONE_CTRLRegDef;		//CORE_TONE_CTRL	0x8065

typedef struct
{
    _IO unsigned char tishi_en					: 1;	//RW 0.0		//tishi_en, 1: enable tishi sound play, dac_dma will read tishi memory
    _IO unsigned char tishi_trunc				: 1;	//RW 0.0		//tishi_trunc_lo, to control tishi sound volume when playing tishi sound. 0: N = 2, 1: N = 4, 2: N=8, 3: N=16 play_sound = main_sound/M + tishi_sound/N
    _IO unsigned char main_trunc				: 2;	//RW 0.0		//main_trunc, to control main sound volume when playing tishi sound. 0: M=2, 1: M=4, 2: M=8, 3: M=16 play_sound = main_sound/M + tishi_sound/N
    _IO unsigned char tishi_cont_mode			: 1;	//RW 0.0		//tishi_cont_mode,  1: continous mode, if tishi_en is 1, playing tishi buffer without concern about wptr, if tishi_en is 0,  stop immediately.  0: if tishi_en is 1, playing tishi buffer when rptr != wptr, if tishi_en is 0, stop when rptr == wptr, so software may set tishi_en=0 when setting the last wptr, recommanded mode!
    _IO unsigned char tishi_mram_sel			: 1;	//RW 0.0		//tishi_mram_sel, 1: mram, 0: not mram
    _IO unsigned char tishi_l_en				: 1;	//RW 0.0		//tishi_l_en, L add tishi enable
    _IO unsigned char tishi_r_en				: 1;	//RW 0.0		//tishi_r_en, R add tishi enable
} __attribute__((packed)) TISHI_CTRLRegDef;		//CORE_TISHI_CTRL	0x806c

typedef struct
{
    _IO unsigned char left_dowm_step			: 4;	//RW 0.0		//left down step, 0~15 represents 1~16
    _IO unsigned char left_up_step				: 4;	//RW 0.0		//left up step, 0~15 represents 1~16
} __attribute__((packed)) LEFT_STEPRegDef;		//CORE_LEFT_STEP	0x806f

typedef struct
{
    _IO unsigned char right_down_step			: 4;	//RW 0.0		//right down step, 0~15 represents 1~16
    _IO unsigned char right_up_step				: 4;	//RW 0.0		//right up step, 0~15 represents 1~16
} __attribute__((packed)) RIGHT_STEPRegDef;		//CORE_RIGHT_STEP	0x8070

typedef struct
{
    _IO unsigned char uartb_en					: 1;	//RW 0.0		//Enable UART
    _IO unsigned char uartb_parity_sel			: 1;	//RW 0.0		//1: odd parity; 0: even parity
    _IO unsigned char uartb_data_len			: 1;	//RW 0.0		//UART Data length 1: 9 bits; 0: 8 bits
    _IO unsigned char uartb_stop_bit			: 1;	//RW 0.0		//1: stop bit is 2, 0:stop bit is 1
    _IO unsigned char uartb_flow_ctrl_en		: 1;	//RW 0.0		//1: enable RTS/CTS flow control
    _IO unsigned char uartb_7816_en				: 1;	//RW 0.0		//1: enable 7816 smart card protocol
    _IO unsigned char uartb_half_duplex_en		: 1;	//RW 0.0		//1: enable half duplex single line UART
    _IO unsigned char uartb_baud_mode_sel		: 1;	//RW 0.0		//1: Use baud settings 0:auto detect baud rate
} __attribute__((packed)) UART_B_CONTROLRegDef;		//CORE_UART_B_CONTROL	0x8071

typedef struct
{
    _IO unsigned short uartb_baud_rate			: 15;	//RW 0.0		//UART baud rate divisor by uart clock
    _IO unsigned short uartb_mram_sel			: 1;	//RW 0.0		//UART DMA mram selection
} __attribute__((packed)) UART_B_BAUD_RATERegDef;		//CORE_UART_B_BAUD_RATE	0x8072

typedef struct
{
    _IO unsigned char gpio0_mux_sel				: 6;	//RW 0.0		//bit5:0 is mutiplex selection, see right table for detail
    _IO unsigned char gpio0_mode_sel			: 2;	//RW 0.0		//1:pullup, 2:pulldown, 3:analog function
} __attribute__((packed)) GPIO0_CONFIGRegDef;		//CORE_GPIO0_CONFIG	0x8080

typedef struct
{
    _IO unsigned char qspi_mode_sel				: 2;	//RW 0.0		//0:standard SPI, 1:dual SPI, 2:quad SPI
    _IO unsigned char qspi_auto_inc				: 1;	//RW 0.0		//1:auto increment address pointer
    _IO unsigned char qspi_xip_en				: 1;	//RW 0.0		//1:enable XIP function
    _IO unsigned char qspi_cont_mode_en			: 1;	//RW 0.0		//1:enable coninuous read mode M byte
    _IO unsigned char qspi_cont_read_mode		: 1;	//RW 0.0		//1:continuous read mode
    _IO unsigned char qspi_mram_sel				: 1;	//RW 0.0		//mram select
    _IO unsigned char qspi_clk_revert			: 1;	//RW 0.0		//1:qspi clock is reverted
} __attribute__((packed)) QSPI_CTRLRegDef;		//CORE_QSPI_CTRL	0x80a0

typedef struct
{
    _IO unsigned char qspi_dummy_bit_cnt		: 4;	//RW 0.0		//dummy bits count
    _IO unsigned char qspi_multi_bit_cnt		: 4;	//RW 0.0		//multi bit mode address bytes count
} __attribute__((packed)) QSPI_DELAYRegDef;		//CORE_QSPI_DELAY	0x80a1

typedef struct
{
    _IO unsigned char iicd_auto_inc				: 1;	//RW 0.0		//automatic dma address increment after each read/write
    _IO unsigned char iicd_restart				: 1;	//RW 0.0		//restart between write and read
    _IO unsigned char iicd_mram_sel				: 1;	//RW 0.0		//mram selection
    _IO unsigned char iicd_intr_en				: 1;	//RW 0.0		//iicd interrupt enable
    _IO unsigned char iicd_intr_clr				: 1;	//RW 0.0		//iicd interrupt clear
    _IO unsigned char iicd_ctrl_reserved		: 3;	//RW 0.0		//not used
} __attribute__((packed)) IICD_CTRLRegDef;		//CORE_IICD_CTRL	0x80aa

typedef struct
{
    _IO unsigned char nec_en 					: 1;	//RW 0.0		//nec detect enable
    _IO unsigned char nec_clkdiv_num			: 7;	//RW 0.0		//nec detect clk divide number
} __attribute__((packed)) NEC_CONTROLRegDef;		//CORE_nec control	0x80c0

typedef struct
{
    _IO unsigned char mic_hpf_ctrl_reserved1	: 4;	//RW 0.0		//not used
    _IO unsigned char mic_hpf_en				: 1;	//RW 0.0		//mic hpf enable
    _IO unsigned char mic_hpf_clr				: 1;	//RW 0.0		//mic hpf clear
    _IO unsigned char mic_hpf_ctrl_reserved2	: 2;	//RW 0.0		//not used
} __attribute__((packed)) MIC_HPF_CTRLRegDef;		//CORE_MIC_HPF_CTRL	0x80c5

typedef struct
{
    _IO unsigned char sys_pause_en				: 1;	//RW 0.0		//system pause enable
    _IO unsigned char sys_spause				: 1;	//RW 0.0		//system pause start, should set 1-> 0
    _IO unsigned char sys_pause_ctrl0_reserved	: 2;	//RW 0.0		//not used
    _IO unsigned char sys_wk_int_en1			: 4;	//RW 0.0		//system wakeup interrupt enable 11:8
} __attribute__((packed)) SYS_PAUSE_CTRL0RegDef;		//CORE_SYS_PAUSE_CTRL0	0x80c6

typedef struct
{
    _IO unsigned char qspi_xip_mode_sel			: 2;	//RW 0.0		//0:standard SPI, 1:dual SPI, 2:quad SPI
    _IO unsigned char qspi_xip_auto_inc			: 1;	//RW 0.0		//1:auto increment address pointer
    _IO unsigned char qspi_xip_xip_en			: 1;	//RW 0.0		//1:enable XIP function
    _IO unsigned char qspi_xip_cont_mode_en		: 1;	//RW 0.0		//1:enable coninuous read mode M byte
    _IO unsigned char qspi_xip_cont_read_mode	: 1;	//RW 0.0		//1:continuous read mode
    _IO unsigned char qspi_xip_mram_sel			: 1;	//RW 0.0		//mram selection
    _IO unsigned char qspi_xip_clk_revert		: 1;	//RW 0.0		//1:qspi clock is reverted
} __attribute__((packed)) QSPI_CTRL_XIPRegDef;		//CORE_QSPI_CTRL_XIP	0x80c8

typedef struct
{
    _IO unsigned char qspi_xip_dummy_bit_cnt	: 4;	//RW 0.0		//dummy bits count
    _IO unsigned char qspi_xip_multi_bit_cnt	: 4;	//RW 0.0		//multi bit mode address bytes count
} __attribute__((packed)) QSPI_DELAY_XIPRegDef;		//CORE_QSPI_DELAY_XIP	0x80c9

typedef struct
{
    _IO unsigned char cvsd_pcmin_mram_sel		: 1;	//RW 0.0		//cvsd encode pcm data in mram select
    _IO unsigned char cvsd_8kpcmout_mram_sel	: 1;	//RW 0.0		//cvsd decode 8k pcm data out mram select
    _IO unsigned char cvsd_cvsdin_mram_sel		: 1;	//RW 0.0		//cvsd decode cvsd data in mram select
    _IO unsigned char cvsd_cvsdout_mram_sel		: 1;	//RW 0.0		//cvsd encode cvsd data out mram select
    _IO unsigned char cvsd_lpf_scaler_coeff		: 1;	//RW 0.0		//cvsd encode LPF scaler coefficients, 0: 2times,1: 4 times
    _IO unsigned char cvsd_lpf_scaler_dir		: 1;	//RW 0.0		//cvsd encode LPF scaler direction,1: zoom in; 0:zoom out
    _IO unsigned char cvsd_ctrl1_reserved		: 1;	//RW 0.0		//not used
    _IO unsigned char cvsd_lpf_scaler_en		: 1;	//RW 0.0		//cvsd encode LPF scaler enable
} __attribute__((packed)) CVSD_CTRL1RegDef;		//CORE_CVSD_CTRL1	0x80ca

typedef struct
{
    _IO unsigned char bist_en					: 1;	//RW 0.0		//bist enable
    _IO unsigned char bist_reset				: 1;	//RW 0.0		//bist resetn
    _IO unsigned char bist_holdl				: 1;	//RW 0.0		//bist holdL
    _IO unsigned char bist_debugz				: 1;	//RW 0.0		//bist debugz
    _IO unsigned char calib_lpo_cnt				: 4;	//RW 0.0		//lpo clock count for calibration 0-15 for 1-32768
} __attribute__((packed)) BIST_CTRLRegDef;		//CORE_BIST_CTRL	0x80cc

typedef struct
{
    _IO unsigned char pwm01_clk_src_sel			: 3;	//RW 0.0		//PWM 0&1 clock source selection, 0-7 for divided by 1-128
    _IO unsigned char pwm0_syn_to_pwm1			: 1;	//RW 0.0		//0: PWM0/1 work independent 1: PWM0 output is synchronized to PWM1
    _IO unsigned char pwm0_init					: 1;	//RW 0.0		//reserved
    _IO unsigned char pwm0_pwm_en				: 1;	//RW 0.0		//PWM enable when pwm0_syn_to_pwm1=0
    _IO unsigned char pwm0_time_mode_en			: 1;	//RW 0.0		//timer mode enable
    _IO unsigned char pwm0_auto_load_en			: 1;	//RW 0.0		//config 0->1 to load the  pwm0_pcnt/ncnt into PWM when pwm0_syn_to_pwm1=0
} __attribute__((packed)) PWM0_CONTROLRegDef;		//CORE_PWM0_CONTROL	0x80d1

typedef struct
{
    _IO unsigned char pwm1_reserved				: 3;	//RW 0.0		//nan
    _IO unsigned char pwm01_out_inv				: 1;	//RW 0.0		//Only used when pwm0_syn_to_pwm1 =1, PWM0/1 out inverse
    _IO unsigned char pwm1_init					: 1;	//RW 0.0		//Only used when pwm0_syn_to_pwm1 =1,  0: PWM0 out high first 1: PWM1 out high first
    _IO unsigned char pwm1_pwm_en				: 1;	//RW 0.0		//PWM enable When pwm0_syn_to_pwm1 =1, PWM0/1 enable
    _IO unsigned char pwm1_time_mode_en			: 1;	//RW 0.0		//timer mode enable
    _IO unsigned char pwm1_auto_load_en			: 1;	//RW 0.0		//auto load enable for timer mode pwm0_syn_to_pwm1 =0 config 0->1 to load the  pwm1_pcnt/ncnt into PWM pwm0_syn_to_pwm1 =1 config 0->1 to load the  pwm0/pwm1_pcnt/ncnt into PWM
} __attribute__((packed)) PWM1_CONTROLRegDef;		//CORE_PWM1_CONTROL	0x80d6

typedef struct
{
    _IO unsigned char cvsd_grp_en				: 1;	//RW 0.0		//cvsd group enable
    _IO unsigned char cvsd_filter_clr			: 1;	//RW 0.0		//cvsd filter delay chain clean enable
    _IO unsigned char cvsd_ff_64to48k_en		: 1;	//RW 0.0		//farrow filter block enable in 64->48k mode
    _IO unsigned char cvsd_clip					: 1;	//RW 0.0		//cvsd clip
    _IO unsigned char cvsd_dec_8k_mode_en		: 1;	//RW 0.0		//cvsd decode 8k mode enable
    _IO unsigned char cvsd_dec_48k_mode_en		: 1;	//RW 0.0		//cvsd decode 48k mode enable
    _IO unsigned char cvsd_48kpcmout_mram_sel	: 1;	//RW 0.0		//cvsd decode 48k pcm data out mram select
    _IO unsigned char cvsd_dma_en				: 1;	//RW 0.0		//cvsd dma enable
} __attribute__((packed)) CVSD_CONTROLRegDef;		//CORE_CVSD_CONTROL	0x80f5

typedef struct
{
    _IO unsigned char dac_en					: 1;	//RW 0.0		//1=enable dac
    _IO unsigned char dac_mram_sel				: 1;	//RW 0.0		//1=source buffer is mram
    _IO unsigned char dac_conti_mode			: 1;	//RW 0.0		//1=continuous mode, wptr is not used
    _IO unsigned char dac_mute_lr				: 1;	//RW 0.0		//mute l&r channel
    _IO unsigned char dac_filter_en				: 1;	//RW 0.0		//filter_en, 0: filter in reset state, 1: filter in work state
    _IO unsigned char dac_sdm_en				: 1;	//RW 0.0		//sdm_en, 0: sdm in reset state, 1: sdm in work state
    _IO unsigned char dac_invert_wave			: 1;	//RW 0.0		//invert wave
    _IO unsigned char dac_bypass				: 1;	//RW 0.0		//bypass dac
} __attribute__((packed)) DAC_CTRLRegDef;		//CORE_DAC_CTRL	0x8106

typedef struct
{
    _IO unsigned short dac_dma_mono_mode			: 1;	//RW 0.0		//dac dma mono mode sel. 0: each sample output L and R, 1: only L
    _IO unsigned short dac_test_channel_to_gpio	: 1;	//RW 0.0		//test_right_to_gpio, sigma-delta channel sel,  0: left channel   <=> gpio_group0;  right channel  <=> gpio_group1 1: right channel <=> gpio_group0;  left channel    <=> gpio_group1
    _IO unsigned short dac_clk_sel				: 2;	//RW 0.0		//clk_sel, 0: mclk, 1: mclk/2, 2: mclk/4, 3: rsvd
    _IO unsigned short dac_dither_en				: 1;	//RW 0.0		//dither_en: DSM dither enable
    _IO unsigned short dac_dat_wid_mode			: 1;	//RW 0.0		//dat_wid_mode: 0: input data width is 16bits, 1: input data width is 24bits
    _IO unsigned short dac_dsm_outdis_mode		: 1;	//RW 0.0		//dac_dsm_outdis_mode: 0: normal; 1: auto mute the DSM input data when the continuous N points of the PCM * VOL data values are 0. The N is set by dac_zero_num_sel
    _IO unsigned short dac_invert_anaclk			: 1;	//RW 0.0		//invert output analog clock
    _IO unsigned short dac_zero_num_sel			: 3;	//RW 0.0		//dac_zero_num_sel: 0~7:  2^(9+dac_zero_num_sel)
    _IO unsigned short dac_scramble_en			: 1;	//RW 0.0		//scramble_en: DWA enable
    _IO unsigned short dac_scramble_mode			: 2;	//RW 0.0		//scramble_mode: 0: normal DWA mode; 1: Random DWA mode; 2: butterfly mode
    _IO unsigned short dac_vol_fade_dis_l		: 1;	//RW 0.0		//vol_fade_dis_l: 0: enable volume fadein/fadeout functionn when change the volume value. The volume will change gradully to the vol or 0 with the step of vol_step 1: disable volume fadein/fadeout. The volume will change immediately to the vol or 0.
    _IO unsigned short dac_vol_fade_dis_r		: 1;	//RW 0.0		//vol_fade_dis_r:  0: enable volume fadein/fadeout functionn when change the volume value. The volume will change gradully to the vol or 0 with the step of vol_step 1: disable volume fadein/fadeout. The volume will change immediately to the vol or 0.
} __attribute__((packed)) DAC_SELRegDef;		//CORE_DAC_SEL	0x8107

typedef struct
{
    _IO unsigned char sbc_cache_addr			: 1;	//RW 0.0		//sbc_cache_addr[16]
    _IO unsigned char sbc_cache_load			: 1;	//RW 0.0		//sbc_cache_load    , set 1->0 to load the cache data from 'sbc_cache_addr'
    _IO unsigned char sbc_cache_update			: 1;	//RW 0.0		//sbc_cache_update, set 1->0 to save the cache data to 'sbc_cache_addr'
    _IO unsigned char sbc_cache_en				: 1;	//RW 0.0		//sbc_cache_en, enable sbc engine auto load and update for each frame
    _IO unsigned char sbc_ch0_op_en				: 1;	//RW 0.0		//channel 0 output enable when sbc decode. 0: enable; 1: disable
    _IO unsigned char sbc_ch1_op_en				: 1;	//RW 0.0		//channel 1 output enable when sbc decode. 0: enable; 1: disable
    _IO unsigned char sbc_ch_mux_en				: 1;	//RW 0.0		//channel 0/1 mux enable
    _IO unsigned char sbc_cvsd_mux_en			: 1;	//RW 0.0		//cvsd filter enable, it will reuse sbc resource. 1: for cvsd; 0: for sbc/msbc
} __attribute__((packed)) SBC_CTRL2RegDef;		//CORE_SBC_CTRL2	0x8116

typedef struct
{
    _IO unsigned char pwm0_clken				: 1;	//RW 0.0		//PWM0
    _IO unsigned char pwm1_clken				: 1;	//RW 0.0		//PWM1
    _IO unsigned char pwm2_clken				: 1;	//RW 0.0		//PWM2
    _IO unsigned char pwm3_clken				: 1;	//RW 0.0		//PWM3
    _IO unsigned char pwm4_clken				: 1;	//RW 0.0		//PWM4
    _IO unsigned char pwm5_clken				: 1;	//RW 0.0		//PWM5
    _IO unsigned char pwm6_clken				: 1;	//RW 0.0		//PWM6
    _IO unsigned char pwm7_clken				: 1;	//RW 0.0		//PWM7
} __attribute__((packed)) PWM_CLKENRegDef;		//CORE_PWM_CLKEN	0x8118

typedef struct
{
    _IO unsigned char adc_comp_scale			: 4;	//RW 0.0		//adc_comp_scale: 0~7:  4~ -3
    _IO unsigned char adc_hbf_scale				: 4;	//RW 0.0		//adc_hbf_scale: 0~7: 4~ -3
} __attribute__((packed)) ADC_FILTER_CTRLRegDef;		//CORE_ADC_FILTER_CTRL	0x811c

typedef struct
{
    _IO unsigned char adda_coef_wr_ctrl_reserved: 2;	//RW 0.0		//reserved
    _IO unsigned char dac_coef_wr_en			: 1;	//RW 0.0		//dac_ram_coef_en, same as above
    _IO unsigned char dac_coef_strobe			: 1;	//RW 0.0		//dac_ram_coef_strobe, same as above
    _IO unsigned char vof_coef_wr_en			: 1;	//RW 0.0		//vof_wr_en, same as above
    _IO unsigned char vof_coef_strobe			: 1;	//RW 0.0		//vof_wr_strobe, same as above
    _IO unsigned char cvsd_coef_wr_en			: 1;	//RW 0.0		//cvsd_coef_wr_en, same as above
    _IO unsigned char cvsd_coef_strobe			: 1;	//RW 0.0		//cvsd_coef_wr_strobe, same as above
} __attribute__((packed)) ADDA_COEF_WR_CTRLRegDef;		//CORE_ADDA_COEF_WR_CTRL	0x811d

typedef struct
{
    _IO unsigned short dac_din_l_sel				: 2;	//RW 0.0		//din_l_sel 00: left channel 01: right channel 10: (left + right)/2 11: -(left + right)/2
    _IO unsigned short dac_clk_en_l				: 1;	//RW 0.0		//clk_en_l, 1: enable left channel clk, 0: disable left channel clk
    _IO unsigned short dac_test_gpio_to_audio	: 1;	//RW 0.0		//test_gpio_to_audio, to test analog dac,  6-bit sigma-delta and clock input from gpio
    _IO unsigned short dac_din_r_sel				: 2;	//RW 0.0		//din_r_sel 00: left channel 01: right channel 10: (left + right)/2 11: -(left + right)/2
    _IO unsigned short dac_clk_en_r				: 1;	//RW 0.0		//clk_en_r, 1: enable right channel clk, 0: disable right channel clk
    _IO unsigned short dac_test_audio_to_gpio	: 1;	//RW 0.0		//test_audio_to_gpio, 1: 6-bit sigma delta and clock output to gpio
    _IO unsigned short dac_sel2_reserved2		: 2;	//RW 0.0		//rsv
    _IO unsigned short dac_ana_clk_en			: 1;	//RW 0.0		//ana_clk_en, 1: enable sdm clk to analog, should set 1 when work
    _IO unsigned short dac_signbits_swap			: 1;	//RW 0.0		//swap dwa_dout_sign[1:0] to match ANA
    _I unsigned short dac_sel2_reserved			: 4;	//RO 0.0		//reserved
} __attribute__((packed)) DAC_SEL2RegDef;		//CORE_DAC_SEL2	0x811e

typedef struct
{
    _IO unsigned char left_down_en				: 1;	//RW 0.0		//left_down_en, 1: enable, 0: disable
    _IO unsigned char left_up_en				: 1;	//RW 0.0		//left_up_en, 1: enable, 0: disable
    _IO unsigned char right_down_en				: 1;	//RW 0.0		//right_down_en, 1: enable, 0: disable
    _IO unsigned char right_up_en				: 1;	//RW 0.0		//right_up_en, 1: enable, 0: disable
    _IO unsigned char ramp_down_finish_clr		: 1;	//RW 0.0		//ramp_down_finish_clr, 1: clear finish flag
    _IO unsigned char ramp_up_finish_clr		: 1;	//RW 0.0		//ramp_up_finish_clr, 1: clear finish flag
    _IO unsigned char ramp_tishi_trunc_hi		: 1;	//RW 0.0		//tishi_trunc_hi
    _I unsigned char ramp_ctrl_reserved			: 1;	//RO 0.0		//reserved
} __attribute__((packed)) RAMP_CTRLRegDef;		//CORE_RAMP_CTRL	0x8138

typedef struct
{
    _IO unsigned char dac_outside_mic_add_en_l	: 1;	//RW 0.0		//outside_mic_add_en_l 0: ignore outside mic data 1: dac add outside mic data
    _IO unsigned char dac_outside_mic_add_en_r	: 1;	//RW 0.0		//outside_mic_add_en_r 0: ignore outside mic data 1: dac add outside mic data
    _IO unsigned char dac_inside_mic_add_en_l	: 1;	//RW 0.0		//inside_mic_add_en_l 0: ignore inside mic data 1: dac add inside mic data
    _IO unsigned char dac_inside_mic_add_en_r	: 1;	//RW 0.0		//inside_mic_add_en_r 0: ignore inside mic data 1: dac add inside mic data
    _IO unsigned char dac_cic_order2_en_l		: 1;	//RW 0.0		//cic_order2_en_l 0: cic use default 3 orders 1: cic use 2 orders to reduce dac output delay
    _IO unsigned char dac_cic_order2_en_r		: 1;	//RW 0.0		//cic_order2_en_r 0: cic use default 3 orders 1: cic use 2 orders to reduce dac output delay
    _IO unsigned char dac_fb_ch_sel				: 1;	//RW 0.0		//fb_ch_sel 0: dac feed back source use left channel 1: dac feed back source use right channel
    _IO unsigned char dac_anc_block_en			: 1;	//RW 0.0		//anc_block_en 0: disable anc module 1: enable anc module
} __attribute__((packed)) DAC_ANC_CTRLRegDef;		//CORE_DAC_ANC_CTRL	0x813d

typedef struct
{
    _IO unsigned char ccm_dma_kick				: 1;	//RW 0.0		//ccm dma kick
    _IO unsigned char ccm_mode_en				: 1;	//RW 0.0		//ccm mode enable
    _IO unsigned char ccm_trx_mode				: 1;	//RW 0.0		//1:rx mode;0:tx mode
    _IO unsigned char ccm_reserved				: 1;	//RW 0.0		//nan
    _IO unsigned char ccm_mic_len				: 2;	//RW 0.0		//mic len 0: 4 bytes; 1: 8 bytes; 2: 12 bytes; 3: 16 bytes;
    _IO unsigned char ccm_ctrl_reserved			: 2;	//RW 0.0		//not used
} __attribute__((packed)) CCM_CTRLRegDef;		//CORE_CCM_CTRL	0x813e

typedef struct
{
    _IO unsigned char adc_micclk_sel			: 2;	//RW 0.0		//micclk_sel: 0??from PLL; 1: PLL div2; 2: PLL div4; 3: from GPIO
    _IO unsigned char adc_micclk_inv			: 1;	//RW 0.0		//micclk_inv: inverse the micclk to analog/PDM
    _IO unsigned char adc_anaclk_en				: 1;	//RW 0.0		//anaclk_en: enable the micclk to analog
    _IO unsigned char adc_micdat_mode			: 1;	//RW 0.0		//micdat_mode: 0: Signed mode; 1: Unsigned mode
    _IO unsigned char adc_pdmclk_en				: 1;	//RW 0.0		//pdmclk_en: enable the micclk to pdm
    _IO unsigned char adc_pdmdat_sel			: 1;	//RW 0.0		//pdmdat_sel: 0: latch data on the rising clock edge; 1: latch data on the falling clock edge
    _IO unsigned char adc_mic_src				: 1;	//RW 0.0		//mic_src: 0: micdata from ANA MIC, 1 micdata from PDM
} __attribute__((packed)) ADC_MIC_CTRLRegDef;		//CORE_ADC_MIC_CTRL	0x8148

typedef struct
{
    _IO unsigned char adc_dc_dly_time			: 2;	//RW 0.0		//dc_dly_time: 0~3: 32/64/128/256 smps;
    _IO unsigned char adc_dc_avg_time			: 2;	//RW 0.0		//dc_avg_time: 0~3: 128/256/512/1024 smps
    _IO unsigned char adc_reserved				: 3;	//nan nan		//nan
    _IO unsigned char adc_dc_en					: 1;	//RW 0.0		//dc_en: enable the dc calc
} __attribute__((packed)) ADC_DC_CTRLRegDef;		//CORE_ADC_DC_CTRL	0x8149

typedef struct
{
    _IO unsigned char iicd1_auto_inc			: 1;	//RW 0.0		//automatic dma address increment after each read/write
    _IO unsigned char iicd1_restart				: 1;	//RW 0.0		//restart between write and read
    _IO unsigned char iicd1_mram_sel			: 1;	//RW 0.0		//mram selection
    _IO unsigned char iicd1_intr_en				: 1;	//RW 0.0		//iicd interrupt enable
    _IO unsigned char iicd1_intr_clr			: 1;	//RW 0.0		//iicd interrupt clear
    _IO unsigned char iicd1_ctrl_reserved		: 3;	//RW 0.0		//reserved
} __attribute__((packed)) IICD1_CTRLRegDef;		//CORE_IICD1_CTRL	0x814a

typedef struct
{
    _IO unsigned char i2s_mram_sel				: 1;	//RW 0.0		//i2s mram sel
    _IO unsigned char pcm_one_channel			: 1;	//RW 0.0		//pcm one channel mode enable
    _IO unsigned char mclk_div2_en				: 1;	//RW 0.0		//mclk divide 2 enable
    _IO unsigned char fifo_mode					: 1;	//RW 0.0		//0: IIS master work in circular BUF mode 1: IIS master work in FIFO mode, transmit 0 when iis_rdptr == iis_tx_wrptr
    _IO unsigned char qspi_intr_clr				: 1;	//RW 0.0		//qspi interrupt clear
    _IO unsigned char qspi_intr_en				: 1;	//RW 0.0		//qspi interrupt enable
    _IO unsigned char audio_test_mux			: 2;	//RW 0.0		//audio test mode mux
} __attribute__((packed)) I2S_MRAM_SELRegDef;		//CORE_I2S_MRAM_SEL	0x815a

typedef struct
{
    _IO unsigned int dac_vol_step_l			: 8;	//RW 0.0		//dac vol step of Channel L
    _IO unsigned int dac_vol_target_val_l		: 14;	//RW 0.0		//dac vol target value of Channel L, Scale in U(14,12)
    _IO unsigned int dac_vol_l_reserved		: 2;	//RW 0.0		//reserved
} __attribute__((packed)) DAC_VOL_LRegDef;		//CORE_DAC_VOL_L	0x815b

typedef struct
{
    _IO unsigned int dac_vol_step_r			: 8;	//RW 0.0		//dac vol step of Channel r
    _IO unsigned int dac_vol_target_val_r		: 14;	//RW 0.0		//dac vol target value of Channel R,, Scale in U(14,12)
    _IO unsigned int dac_vol_r_reserved		: 2;	//RW 0.0		//reserved
} __attribute__((packed)) DAC_VOL_RRegDef;		//CORE_DAC_VOL_R	0x815e

typedef struct
{
    _IO unsigned int dac_vol_step_p			: 8;	//RW 0.0		//dac vol step of Channel Prompt
    _IO unsigned int dac_voll_target_val_p		: 14;	//RW 0.0		//dac vol target value of Channel Prompt,, Scale in U(14,12)
    _IO unsigned int dac_vol_p_reserved		: 2;	//RW 0.0		//reserved
} __attribute__((packed)) DAC_VOL_PRegDef;		//CORE_DAC_VOL_P	0x8161

typedef struct
{
    _IO unsigned int dac_dither_power			: 23;	//RW 0.0		//dither power (signed)
    _IO unsigned int dac_dither_pow_reserved	: 1;	//RW 0.0		//reserved
} __attribute__((packed)) DAC_DITHER_POWRegDef;		//CORE_DAC_DITHER_POW	0x8164

typedef struct
{
    _IO unsigned char clear_int					: 1;	//RW 0.0		//nan
    _IO unsigned char sdma_src_mram_sel			: 1;	//RW 0.0		//nan
    _IO unsigned char sdma_dst_mram_sel			: 1;	//RW 0.0		//nan
    _IO unsigned char sdma_ctrl_reserve			: 5;	//RW 0.0		//nan
} __attribute__((packed)) SDMA_CTRLRegDef;		//CORE_SDMA_CTRL	0x8167

typedef struct
{
    _IO unsigned char eq_blken					: 1;	//RW 0.0		//EQ DMA block enable
    _IO unsigned char eq_start					: 1;	//RW 0.0		//EQ DMA start signal
    _IO unsigned char nrof_channels				: 1;	//RW 0.0		//Number of channels
    _IO unsigned char wid_sel					: 1;	//RW 0.0		//WIDTH select for EQ; 0: 16bit; 1: 24bit
    _IO unsigned char src_mram_sel				: 1;	//RW 0.0		//MRAM select for EQ DMA source address
    _IO unsigned char dst_mram_sel				: 1;	//RW 0.0		//MRAM select for EQ DMA destination address
    _IO unsigned char reserved					: 2;	//RW 0.0		//nan
} __attribute__((packed)) EQ_CTRLRegDef;		//CORE_EQ_CTRL	0x8176

typedef struct
{
    _IO unsigned short i2s_clksel				: 3;	//RW 0.0		//0??6.144M; 1: 12.288M  2: 11.2896M;  3: 4.096M  4: From GPIO
    _IO unsigned short dac_clksel				: 3;	//RW 0.0		//0??6.144M; 1: 12.288M  2: 11.2896M;  3: 4.096M  4: From GPIO
    _IO unsigned short mic_clksel				: 3;	//RW 0.0		//0??6.144M; 1: 12.288M  2: 11.2896M;  3: 4.096M  4: From GPIO
    _IO unsigned short audioclk_ana_sel			: 1;	//RW 0.0		//1: audio clk  select analog output clock; 0: audio clk select digital divider clock
    _IO unsigned short dsmclk_ana_sel			: 1;	//RW 0.0		//1: sdm divider input clk  select analog output clock; 0: dsm divider input  clk select digital divider clock
    _IO unsigned short io_rst_src_en				: 5;	//RW 0.0		//11: touch0 long press reset enable 12: touch1 long press reset enable 13: gpio11 long press reset enable 14: gpio33 long press reset enable 15: gpio35 long press reset enable
} __attribute__((packed)) AUDIO_CLKSELDef;		//CORE_AUDIO_CLKSEL	0x817d

typedef struct
{
    //_IO unsigned int adc_vol_step				:8;	//RW 0.0		//ADC vol step of Channel Prompt
    _IO unsigned int adc_voll_target_val		: 14;	//RW 0.0		//ADC vol target value of Channel Prompt,, Scale in U(14,12)
    _IO unsigned int adc_mute_en				: 1;	//RW 0.0		//mute ADC channel
    _IO unsigned int adc_vol_fade_dis			: 1;	//RW 0.0		//vol_fade_dis:  0: enable volume fadein/fadeout functionn when change the volume value. The volume will change gradully to the vol or 0 with the step of vol_step 1: disable volume fadein/fadeout. The volume will change immediately to the vol or 0.
} __attribute__((packed)) ADC_VOLRegDef;		//CORE_ADC_VOL	0x817f

typedef struct
{
    _IO unsigned char dsmdiv_11m_ctrl			: 4;	//RW 0.0		//bit[0] : enable:    enable dsmdiv_11m bit[1]:  dither_enable: enable dither bit[2]: div2:  0: output div1:  1: output div2 bit[3]: cfg_load: config 0->1 to load the dsmdiv_11m_init and dsmdiv_11m_frace into dsmdiv
    _IO unsigned char dsmdiv_12m_ctrl			: 4;	//RW 0.0		//bit[0] : enable:    enable dsmdiv_12m bit[1]:  dither_enable: enable dither bit[2]: div2:  0: output div1:  1: output div2 bit[3]: cfg_load: config 0->1 to load the dsmdiv_12m_init and dsmdiv_12m_frace into dsmdiv
} __attribute__((packed)) DSMDIV_CTRLRegDef;		//CORE_DSMDIV_CTRL	0x8187

typedef struct
{
    _O unsigned char ts_analog_ctrl				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor Analog CTRL
    _O unsigned char ts_fir_ctrl				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor FIR CTRL
    _O unsigned char ts_cal_ctrl1				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CAL CTRL1
    _O unsigned char ts_cal_ctrl2				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CAL CTRL2
    _O unsigned char ts_cal_ctrl3				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CAL CTRL3
    _O unsigned char ts_cal_ctrl4				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CAL CTRL4
    _O unsigned char ts_cmp_ctrl1				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CMP CTRL1
    _O unsigned char ts_cmp_ctrl2				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor CMP CTRL2
} __attribute__((packed)) LPM_TS_CTRLRegDef;		//CORE_LPM_TS_CTRL	0x81f0

typedef struct
{
    _O unsigned char ts_bl_ctrl1				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor BL_CTRL1
    _O unsigned char ts_bl_ctrl2				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor BL_CTRL2
    _O unsigned char ts_bl_ctrl3				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor BL_CTRL3
    _O unsigned char ts_bl_ctrl4				: 1;	//W 0.0		//Write 1 to write 804c-804f contents into Touch sensor BL_CTRL4
    _IO unsigned char lpm_ts_ctrl2_reserved		: 4;	//RW 0.0		//not used
} __attribute__((packed)) LPM_TS_CTRL2RegDef;		//CORE_LPM_TS_CTRL2	0x81f1

typedef struct
{
    _O unsigned char lpm_ts_rd_mux2				: 5;	//W 0.0		//lpm ts cfg status registers read mux bit0: CAL_CFG     bit1: BL_CTRL1 bit2: BL_CTRL2    bit3: BL_CTRL3 bit4: BL_CTRL4
    _IO unsigned char lpm_ts_rd_mux2_reserved	: 3;	//RW 0.0		//not used
} __attribute__((packed)) LPM_TS_RD_MUX2RegDef;		//CORE_LPM_TS_RD_MUX2	0x81f3

typedef struct
{
    _IO unsigned char ice_cmd					: 3;	//RW 0.0		//001: Force CPU stop 010: Step one instruction 011: Resume running 100: Special function, test only
    _IO unsigned char ice_cpu_stop				: 1;	//RW 0.0		//1: CPU stops when PC is >=breakpoint 0 and <= breakpoint 1
    _IO unsigned char ice_bkp0_sel				: 2;	//RW 0.0		//00: ICE breakpoint 0 is disabled 01: ICE breakpoint 0 is code address breakpoint 10: ICE breakpoint 0 is memory read access breakpoint 11: ICE breakpoint 0 is memory write access breakpoint
    _IO unsigned char ice_bkp1_sel				: 2;	//RW 0.0		//00: ICE breakpoint 1 is disabled 01: ICE breakpoint 1 is code address breakpoint 10: ICE breakpoint 1 is memory read access breakpoint 11: ICE breakpoint 1 is memory write access breakpoint
} __attribute__((packed)) ICE_CONTROLRegDef;		//CORE_ICE_CONTROL	0x8203

typedef struct
{
    _I unsigned char ice_cpu_halted				: 1;	//R 0.0		//1: CPU is Halt
    _I unsigned char ice_step					: 1;	//R 0.0		//1: Step
    _I unsigned char ice_flag_sel_val			: 1;	//R 0.0		//Flag value of selected flag(ICE register select 0x8209)
    _IO unsigned char ice_status_reserved		: 5;	//RW 0.0		//not used
} __attribute__((packed)) ICE_STATUSRegDef;		//CORE_ICE_STATUS	0x8204

typedef struct
{
    _I unsigned int clkn_bluetooth				: 28;	//R 0.0		//clockN of bluetooth
    _IO unsigned int clkn_reserved				: 4;	//RW 0.0		//not used
} __attribute__((packed)) CLKNRegDef;		//CORE_CLKN	0x8300

typedef struct
{
    _I unsigned int extmclk_bluetooth			: 28;	//R 0.0		//clock External of bluetooth
    _IO unsigned int extmclk_reserved			: 4;	//RW 0.0		//not used
} __attribute__((packed)) EXTMCLKRegDef;		//CORE_EXTMCLK	0x8304

typedef struct
{
    _I unsigned char crystal_stable				: 1;	//R 0.0		//crystal is stable
    _I unsigned char aes_key_load				: 1;	//R 0.0		//aes key is loaded
    _I unsigned char cpu_kick					: 1;	//R 0.0		//CPU is kicked
    _I unsigned char cm0_debug_ready			: 1;	//R 0.0		//cm0 debug data is ready
    _I unsigned char sbc_decoder_busy			: 1;	//R 0.0		//1=sbc decoder is busy
    _I unsigned char uart_baud_rate_bit13		: 1;	//R 0.0		//Auto detected baud rate bit13 for uart
    _I unsigned char uartb_baud_rate_bit13		: 1;	//R 0.0		//Auto detected baud rate bit13 for uartb
    _IO unsigned char status_reserved			: 1;	//RW 0.0		//not used
} __attribute__((packed)) MISC_STATUSRegDef;		//CORE_MISC_STATUS	0x8308

typedef struct
{
    _I unsigned char bitsend_status				: 2;	//R 0.0		//bitsend status
    _I unsigned char isogate_en					: 1;	//R 0.0		//isogate enable
    _I unsigned char ice_wk_high_en				: 1;	//R 0.0		//ice wake up high enable
    _I unsigned char ice_wk_low_en				: 1;	//R 0.0		//ice wake up low enable
    _I unsigned char mic_done					: 1;	//R 0.0		//mic calculation done
    _I unsigned char ccm_dma_done				: 1;	//R 0.0		//ccm dma done
    _I unsigned char ccm_mic_chk_pass			: 1;	//R 0.0		//ccm mic check pass
} __attribute__((packed)) BITSEND_STATUSRegDef;		//CORE_BITSEND_STATUS	0x8309

typedef struct
{
    _I unsigned char uart_tx_fifo_full			: 1;	//R 0.0		//TX FIFO full
    _I unsigned char uart_tx_fifo_empty			: 1;	//R 0.0		//TX FIFO empty
    _I unsigned char uart_tx_busy				: 1;	//R 0.0		//TX busy
    _I unsigned char uart_rx_fifo_empty			: 1;	//R 0.0		//RX FIFO empty
    _I unsigned char uart_auto_det_baudrate		: 4;	//R 0.0		//Auto detected baud rate
} __attribute__((packed)) UART_STATERegDef;		//CORE_UART_STATE	0x830c

typedef struct
{
    _I unsigned char sbc_sat_record				: 6;	//R 0.0		//sbc sat record
    _I unsigned char sbc_error					: 2;	//R 0.0		//sbc error
} __attribute__((packed)) SBC_STATUSRegDef;		//CORE_SBC_STATUS	0x8319

typedef struct
{
    _I unsigned char gpio_in1_data				: 4;	//R 0.0		//Gpio[39:32] input data
    _I unsigned char gpio_in1_reserved			: 4;	//R 0.0		//not used
} __attribute__((packed)) GPIO_IN1RegDef;		//CORE_GPIO_IN1	0x831a

typedef struct
{
    _I unsigned short otp_addr 					: 9;	//R 0.0		//otp address
    _I unsigned short vbat_change_flag 			: 1;	//R 0.0		//vbat change flag
    _I unsigned short vbat_ov_flag				: 1;	//R 0.0		//vbat cross flag
    _I unsigned short ice_mode					: 1;	//R 0.0		//nan
    _I unsigned short wd_en						: 2;	//R 0.0		//nan
    _I unsigned short io_rst_en					: 1;	//R 0.0		//nan
    _I unsigned short sdma_done					: 1;	//R 0.0		//soft dma done
} __attribute__((packed)) OTP_ADDRESSRegDef;		//CORE_OTP_ADDRESS	0x832e

typedef struct
{
    _I unsigned char iic1_crc_ok				: 1;	//R 0.0		//iic1 dma crc is OK
    _I unsigned char iic1_done					: 1;	//R 0.0		//iic1 dma is done
    _I unsigned char qspi_crc_ok				: 1;	//R 0.0		//qspi dma crc is OK
    _I unsigned char qspi_done					: 1;	//R 0.0		//qspi dma is done
    _I unsigned char iic_crc_ok					: 1;	//R 0.0		//iic dma crc is OK
    _I unsigned char iic_done					: 1;	//R 0.0		//iic dma is done
    _I unsigned char spi_done					: 1;	//R 0.0		//spi dma is done
    _I unsigned char qspi_writing				: 1;	//R 0.0		//qspi dma is writing
} __attribute__((packed)) DMA_STATUSRegDef;		//CORE_DMA_STATUS	0x8330

typedef struct
{
    _I unsigned char iic1_ack					: 1;	//R 0.0		//iic1 ACK is got
    _I unsigned char clk_cali_done				: 1;	//R 0.0		//clock calibration is done
    _I unsigned char cvsd_grp_done				: 1;	//R 0.0		//cvsd group done
    _I unsigned char iic_ack					: 1;	//R 0.0		//iic ACK is got
    _I unsigned char lpm_busy					: 1;	//R 0.0		//lpm reg access busy
    _I unsigned char qspi_conti_mode			: 1;	//R 0.0		//qspi is in continous mode
    _I unsigned char i2s_busy					: 1;	//R 0.0		//i2s busy
    _I unsigned char cvsd_filt_clr_done			: 1;	//R 0.0		//cvsd filter clear done
} __attribute__((packed)) PERIPHERAL_STATUSRegDef;		//CORE_PERIPHERAL_STATUS	0x8331

typedef struct
{
    _I unsigned char dac_dma_fsm				: 4;	//RO 0.0		//dac_dma  state machine 0: idle, 1: rdlow, 2: rdhigh, 3: wait8k, 4: waitlow,  5: waithigh, 6: rampup, 7: rampdown, 8:waitpcm When normal working, it is wait8k at most of the time!
    _I unsigned char dac_down_finish			: 1;	//RO 0.0		//down_finish, 1: both left channel and right channel have finished ramp function
    _I unsigned char dac_up_finish				: 1;	//RO 0.0		//up_finish, 1: both left channel and right channel have finished ramp function
    _I unsigned char dac_audio_dac_rptr			: 1;	//RO 0.0		//0: audio_dac_rptr <= start_addr + len/2 + 1; 1: audio_dac_rptr > start_addr + len/2 + 1
    _I unsigned char dac_tishi_rptr_			: 1;	//RO 0.0		//0: tishi_rptr <= tishi_saddr + tishi_len/2 + 1; 1: tishi_rptr > tishi_saddr + tishi_len/2 + 1;
} __attribute__((packed)) DAC_STATUSRegDef;		//CORE_DAC_STATUS	0x8332

typedef struct
{
    _I unsigned char osc_xtal_en_reg_on			: 1;	//R 0.0		//enable controls for active mode
    _I unsigned char osc_xtal_en_on				: 1;	//R 0.0		//nan
    _I unsigned char osc_rc_en_on				: 1;	//R 0.0		//nan
    _I unsigned char pmu_bbldo_en_on			: 1;	//R 0.0		//nan
    _I unsigned char osc_xtal_en_reg			: 1;	//R 0.0		//enable control for lpm mode
    _I unsigned char osc_xtal_en				: 1;	//R 0.0		//enable control for lpm mode
    _I unsigned char osc_rc_en					: 1;	//R 0.0		//enable control for lpm mode
    _I unsigned char pmu_bbldo_en				: 1;	//R 0.0		//enable control for lpm mode
} __attribute__((packed)) OSC_CTRLRegDef;		//CORE_OSC_CTRL	0x8336

typedef struct
{
    _I unsigned char osc_xtal_ib_ctrl			: 4;	//R 0.0		//crystal bias control
    _I unsigned char osc_xtal_ib_obuf			: 2;	//R 0.0		//crystal output buffer control
    _I unsigned char osc_lpm_ib_ctrl			: 2;	//R 0.0		//lpm clock selection
} __attribute__((packed)) OSC_IB_CTRLRegDef;		//CORE_OSC_IB_CTRL	0x8337

typedef struct
{
    _I unsigned short osc_xtal_ctrim				: 5;	//R 0.0		//crystal capacitor adjust
    _I unsigned short osc_rc_ctrim				: 5;	//R 0.0		//rc oscilator adjust
    _I unsigned short enable_ss_wu				: 1;	//R 0.0		//dummy bit
    _I unsigned short pmu_wklogic_en_hold		: 1;	//R 0.0		//hold the chipen, =0 shutdown mode
    _I unsigned short pmu_bbldo_vctrim			: 4;	//R 0.0		//bb ldo voltage control
} __attribute__((packed)) OSC_CTRL1RegDef;		//CORE_OSC_CTRL1	0x8338

typedef struct
{
    _I unsigned char digldo_pre_on_cnt			: 3;	//R 0.0		//digital LDO is power on when counter is less than this value
    _I unsigned char pad_latch					: 1;	//R 0.0		//1=latch all gpio's previous state
    _I unsigned char enable_exen				: 1;	//R 0.0		//1=exen output is enabled when wakeup/sleep
    _I unsigned char enable_comparator			: 1;	//R 0.0		//1=comparator wakeup enable
    _I unsigned char fast_wake					: 1;	//R 0.0		//1=GPIO/comparator wake up after exen counter 0=Set larger value of exen/xtal counter at GPIO wakeup
    _I unsigned char cold_wake					: 1;	//R 0.0		//1=clear memory/load OTP/load eeprom at wakeup
} __attribute__((packed)) WAKEUP_CTRLRegDef;		//CORE_WAKEUP_CTRL	0x833C

typedef struct
{
    _I unsigned char misc_sig_retmem_en12		: 5;	//R 0.0		//nan
    _IO unsigned char isolation_gate_reserved	: 1;	//RW 0.0		//not used
    _I unsigned char isolation_cs_en			: 1;	//R 0.0		//CS enable
    _I unsigned char isolation_ret_mode			: 1;	//R 0.0		//retention mode
} __attribute__((packed)) ISOLATION_GATERegDef;		//CORE_ISOLATION_GATE	0x833D

typedef struct
{
    _I unsigned short buck_bypass_th_sel			: 4;	//R 0.0		//nan
    _I unsigned short buck_freq_cal				: 4;	//R 0.0		//nan
    _I unsigned short buck_fb_sel				: 5;	//R 0.0		//nan
    _I unsigned short buck_pfm_pls_sel			: 3;	//R 0.0		//nan
} __attribute__((packed)) BUCK_CTRLRegDef;		//CORE_BUCK_CTRL	0x833E

typedef struct
{
    _I unsigned char buck_icut_comp_en			: 1;	//R 0.0		//nan
    _I unsigned char buck_freq_sel				: 1;	//R 0.0		//nan
    _I unsigned char buck_oc_disen				: 1;	//R 0.0		//nan
    _I unsigned char buck_icut_zcd_en			: 1;	//R 0.0		//nan
    _I unsigned char buck_dt_ctrl				: 1;	//R 0.0		//nan
    _I unsigned char buck_zcd_en				: 1;	//R 0.0		//nan
    _I unsigned char buck_sense_en				: 1;	//R 0.0		//nan
    _I unsigned char buck_byp_en				: 1;	//R 0.0		//nan
} __attribute__((packed)) BUCK_CTRL1RegDef;		//CORE_BUCK_CTRL1	0x8340

typedef struct
{
    _I unsigned char buck_pfm_disen				: 1;	//R 0.0		//nan
    _I unsigned char buck_pfm_disen_reserved	: 7;	//R 0.0		//not used
} __attribute__((packed)) BUCK_PFM_DISENRegDef;		//CORE_BUCK_PFM_DISEN	0x8341

typedef struct
{
    _I unsigned short bist_done					: 15;	//R 0.0		//bist is done
    _IO unsigned short bist_done_reserved		: 1;	//RW 0.0		//not used
} __attribute__((packed)) BIST_DONERegDef;		//CORE_BIST_DONE	0x834e

typedef struct
{
    _I unsigned short bist_fail					: 15;	//R 0.0		//bist fail indication
    _IO unsigned short bist_fail_reserved		: 1;	//RW 0.0		//not used
} __attribute__((packed)) BIST_FAILRegDef;		//CORE_BIST_FAIL	0x8350

typedef struct
{
    _I unsigned char uartb_tx_fifo_full			: 1;	//R 0.0		//TX FIFO full
    _I unsigned char uartb_tx_fifo_empty		: 1;	//R 0.0		//TX FIFO empty
    _I unsigned char uartb_tx_busy				: 1;	//R 0.0		//TX busy
    _I unsigned char uartb_rx_fifo_empty		: 1;	//R 0.0		//RX FIFO empty
    _I unsigned char uartb_auto_det_barate		: 4;	//R 0.0		//Auto detected baud rate
} __attribute__((packed)) UART_B_STATERegDef;		//CORE_UART_B_STATE	0x8354

typedef struct
{
    _I unsigned char sys_wakeup_int_status1		: 4;	//R 0.0		//system wakeup interrupt status 11:8
    _IO unsigned char sys_wakeup_int_status1_reserved: 4;	//RW 0.0		//not used
} __attribute__((packed)) SYS_WAKEUP_INT_STATUS1RegDef;		//CORE_SYS_WAKEUP_INT_STATUS1	0x8375

typedef struct
{
    _I unsigned short chgr_event					: 10;	//R 0.0		//[9]: ad_lpm_chgr_in_det_aon,   //wakeup interrupt source, 4cyc de-glitch, reset valid, uart rx pin [8]: ad_lpm_chgr_pgood,        //wakeup interrupt source, 4cyc de-glitch, reset valid [7]: ad_lpm_chgr_uvlo_ok_aon,    //wakeup interrupt source, 4cyc de-glitch, reset valid [6]: ad_lpm_chgr_state_ind,    //wakeup interrupt source, 4cyc de-glitch, reset valid [5]: ad_lpm_chgr_cc_ov_cv,     //wakeup interrupt source, 4cyc de-glitch, reset masked [4]: ad_lpm_chgr_dppm_ov_cv,   //wakeup interrupt source, 4cyc de-glitch, reset masked [3]: ad_lpm_chgr_state_vbat_lv,//wakeup interrupt source, 4cyc de-glitch, reset masked [2]: ad_lpm_chgr_dppm_ov_cc,   //wakeup interrupt source, 4cyc de-glitch, reset masked [1]: ad_lpm_chgr_state_rch_en, //wakeup interrupt source, 4cyc de-glitch, reset masked [0]: ad_lpm_chgr_state_ichg   //wakeup interrupt source, 4cyc de-glitch, reset masked
    _IO unsigned short chgr_event_reserved		: 6;	//RW 0.0		//not used
} __attribute__((packed)) CHGR_EVENTRegDef;		//CORE_CHGR_EVENT	0x8379

typedef struct
{
    _I unsigned char lpm_ts_status				: 6;	//R 0.0		//nan
    _IO unsigned char lpm_ts_status_reserved	: 2;	//RW 0.0		//nan
} __attribute__((packed)) LPM_TS_STATUSRegDef;		//CORE_LPM_TS_STATUS	0x839c

typedef struct
{
    _IO unsigned char da_ldo_dig_core_vsel : 3;	//RW 0.0		//nan
    _IO unsigned char da_ldo_io_doze_en	: 1;    //RW 0.0		//nan
    _IO unsigned char da_ldo_io_ocp_en : 1;	    //RW 0.0		//nan
    _IO unsigned char da_ldo_io_vsel : 1;	    //RW 0.0		//nan
    _IO unsigned char da_ldo_io_en : 1;	        //RW 0.0		//nan
    _IO unsigned char reserved : 1;	            //RW 0.0		//nan
}__attribute__((packed)) LPM_DA_LDORegDef;      //CORE_LPM_STATUS1	0x83a4

typedef struct
{
    _I unsigned char touch_wkhi					: 2;	//R 0.0		//nan
    _I unsigned char res						: 2;	//R 0.0		//nan
    _I unsigned char touch_wklo					: 2;	//R 0.0		//nan
    _I unsigned char res1					: 2;	//R 0.0		//nan
} __attribute__((packed)) LPM_STATUS1RegDef;		//CORE_LPM_STATUS1	0x83a5

typedef struct
{
    _IO unsigned int spi_en					: 1;	//RW 0.0		//
    _IO unsigned int spi_hw_start_en			: 1;	//RW 0.0		//
    _IO unsigned int spi_hw_start_sel			: 1;	//RW 0.0		//
    _O unsigned int spi_start					: 1;	//W 0.0		//
    _IO unsigned int cpol						: 1;	//RW 0.0		//
    _IO unsigned int cpha						: 1;	//RW 0.0		//
    _IO unsigned int tx_rx_seq					: 1;	//RW 0.0
    _IO unsigned int rx_adj_en					: 1;	//RW 0.0
    _IO unsigned int ncs_dly					: 3;	//RW 0.0
    _IO unsigned int fifo_sw_en				: 1;	//RW 0.0
    _IO unsigned int trx_dly					: 4;	//RW 0.0
    _IO unsigned int clk_div					: 4;	//RW 0.0
    _IO unsigned int rx_adj_clk				: 4;	//RW 0.0
    _IO unsigned int slave_mode				: 1;	//RW 0.0
    _IO unsigned int y_to_rgb_mode				: 1;	//RW 0.0
    _IO unsigned int lsb_first_mode			: 1;	//RW 0.0
    _IO unsigned int mram_sel					: 1;	//RW 0.0		//mram select
    _I unsigned int spi_ctrl_reserved			: 4;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_CTRLRegDef;		//CORE_SPI_CTRL	0x8800

typedef struct
{
    _O unsigned int tx_fifo_rd					: 1;	//W 0.0
    _I unsigned int tx_fifo_empty				: 1;	//RO 0.0		//nan
    _I unsigned int tx_fifo_full				: 1;	//RO 0.0		//nan
    _IO unsigned int sw_fifo_rst				: 1;	//RW 0.0
    _I unsigned int spi_fifo_reserved			: 4;	//RO 0.0		//reserved
    _O unsigned int rx_fifo_rd					: 1;	//W 0.0
    _I unsigned int rx_fifo_empty				: 1;	//RO 0.0		//nan
    _I unsigned int rx_fifo_full				: 1;	//RO 0.0		//nan
    _IO unsigned int fifo_auto_rst				: 1;	//RW 0.0
    _I unsigned int spi_fifo_reserved1			: 4;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_FIFORegDef;		//CORE_SPI_FIFO	0x8804

typedef struct
{
    _IO unsigned int tx_fifo_data				: 8;	//RW 0.0
} __attribute__((packed)) SPI_FIFOTDATARegDef;		//CORE_SPI_FIFO	0x8806

typedef struct
{
    _IO unsigned int rx_fifo_data				: 8;	//RW 0.0
} __attribute__((packed)) SPI_FIFORDATARegDef;		//CORE_SPI_FIFO	0x8807

typedef struct
{
    _IO unsigned int dma_en					: 1;	//RW 0.0		//nan
    _IO unsigned int dma_start_auto			: 1;	//RW 0.0
    _I unsigned int spi_dma_reserved			: 1;	//RO 0.0		//reserved
    _O unsigned int dma_start_man				: 1;	//W 0.0
    _IO unsigned int tx_fixed_addr				: 1;	//RW 0.0
    _IO unsigned int rx_fixed_addr				: 1;	//RW 0.0
    _IO unsigned int dma_pingpang_en			: 1;	//RW 0.0
    _I unsigned int spi_dma_reserved1			: 9;	//RO 0.0		//reserved
    _I unsigned int dma_status					: 8;	//RO 0.0		//debug only
    _I unsigned int spi_status					: 8;	//RO 0.0		//debug only
} __attribute__((packed)) SPI_DMARegDef;		//CORE_SPI_DMA	0x8808

typedef struct
{
    _IO unsigned int dma_done_ris				: 1;	//RW 0.0		//dma_done raw interrupt status 1: one spi transceive task finish
    _IO unsigned int tx_fifo_lo_ris			: 1;	//RW 0.0		//nan
    _IO unsigned int tx_fifo_ovfl_ris			: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_hi_ris			: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_ovfl_ris			: 1;	//RW 0.0		//nan
    _IO unsigned int start_over_quick_ris		: 1;	//RW 0.0		//SPI
    _I unsigned int spi_irq_reserved			: 2;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_IRQRSRegDef;		//CORE_SPI_IRQ0	0x880c

typedef struct
{
    _IO unsigned int dma_done_mis				: 1;	//RW 0.0		//dma_done masked interrupt status, only assert when dma_done_en '1'
    _IO unsigned int tx_fifo_lo_mis			: 1;	//RW 0.0		//nan
    _IO unsigned int tx_fifo_ovfl_mis			: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_hi_mis			: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_ovfl_mis			: 1;	//RW 0.0		//nan
    _IO unsigned int start_over_quick_mis		: 1;	//RW 0.0		//nan
    _I unsigned int spi_irq_reserved1			: 2;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_IRQMSRegDef;		//CORE_SPI_IRQ	0x880d

typedef struct
{
    _IO unsigned int dma_done_en				: 1;	//RW 0.0		//nan
    _IO unsigned int tx_fifo_lo_en				: 1;	//RW 0.0		//nan
    _IO unsigned int tx_fifo_ovfl_en			: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_hi_en				: 1;	//RW 0.0		//nan
    _IO unsigned int rx_fifo_ovfl_en			: 1;	//RW 0.0		//nan
    _IO unsigned int start_over_quick_en		: 1;	//RW 0.0		//nan
    _I unsigned int spi_irq_reserved2			: 2;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_IRQMENRegDef;		//CORE_SPI_IRQ	0x880e

typedef struct
{
    _IO unsigned int tx_start_addr				: 20;	//RW 0.0
    _I unsigned int spi_dma_tx_saddr_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_TX_SADDRRegDef;		//CORE_SPI_DMA_TX_SADDR	0x8810

typedef struct
{
    _IO unsigned int rx_start_addr				: 20;	//RW 0.0
    _I unsigned int spi_dma_rx_saddr_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_RX_SADDRRegDef;		//CORE_SPI_DMA_RX_SADDR	0x8814

typedef struct
{
    _IO unsigned int tx_len					: 20;	//RW 0.0		//
    _I unsigned int spi_dma_tx_len_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_TX_LENRegDef;		//CORE_SPI_DMA_TX_LEN	0x8818

typedef struct
{
    _IO unsigned int rx_len					: 20;	//RW 0.0		//
    _I unsigned int spi_dma_rx_len_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_RX_LENRegDef;		//CORE_SPI_DMA_RX_LEN	0x881c

typedef struct
{
    _I unsigned int tx_addr					: 20;	//RO 0.0
    _I unsigned int spi_dma_tx_addr_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_TX_ADDRRegDef;		//CORE_SPI_DMA_TX_ADDR	0x8820

typedef struct
{
    _I unsigned int rx_addr					: 20;	//RO 0.0
    _I unsigned int spi_dma_rx_addr_reserved	: 12;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_DMA_RX_ADDRRegDef;		//CORE_SPI_DMA_RX_ADDR	0x8824

typedef struct
{
    _IO unsigned int tx_fifo_waterlevel		: 5;	//RW 0.0
    _I unsigned int spi_fifo_ctrl_reserved		: 3;	//RO 0.0		//reserved
    _IO unsigned int rx_fifo_waterlevel		: 5;	//RW 0.0
    _I unsigned int spi_fifo_ctrl_reserved1	: 3;	//RO 0.0		//reserved
    _I unsigned int tx_fifo_items				: 5;	//RO 0.0		//nan
    _I unsigned int spi_fifo_ctrl_reserved2	: 3;	//RO 0.0		//reserved
    _I unsigned int rx_fifo_items				: 5;	//RO 0.0		//nan
    _I unsigned int spi_fifo_ctrl_reserved3	: 3;	//RO 0.0		//reserved
} __attribute__((packed)) SPI_FIFO_CTRLRegDef;		//CORE_SPI_FIFO_CTRL	0x8830

typedef struct
{
    _IO unsigned char use_iso_enp2_out			: 1;	//RW 0.0		//1:enable ISO for endpoint 2 OUT
    _IO unsigned char use_iso_enp2_in			: 1;	//RW 0.0		//1:enable ISO for endpoint 2 IN
    _IO unsigned char usb_pad_control			: 2;	//RW 0.0		//USB pad bias control
    _IO unsigned char usb_func_en				: 1;	//RW 0.0		//enable usb function
    _IO unsigned char use_speed_sel				: 1;	//RW 0.0		//1:full speed 12M, 0:low speed 1.5M
    _IO unsigned char usb_resume_device			: 1;	//RW 0.0		//resume device
    _IO unsigned char usb_enp_wu_en				: 1;	//RW 0.0		//endpoint wakeup enable
} __attribute__((packed)) USB_CONFIGRegDef;		//CORE_USB_CONFIG	0x8c00

typedef struct
{
    _IO unsigned char usb_addr					: 7;	//RW 0.0		//usb address
    _IO unsigned char usb_disrec_pac			: 1;	//RW 0.0		//disable broadcast(address 0) packet receive
} __attribute__((packed)) USB_ADDRRegDef;		//CORE_USB_ADDR	0x80c1

typedef struct
{
    _IO unsigned short usb_irq_mask				: 11;	//RW 0.0		//set 1 to mask correspoinding IRQ. See irq status for bit detail
    _IO unsigned short usb_drv_usb_trans			: 2;	//RW 0.0		//DRV to usb transceiver
    _IO unsigned short usb_irq_mask_reserved		: 3;	//RW 0.0		//nan
} __attribute__((packed)) USB_IRQ_MASKRegDef;		//CORE_USB_IRQ_MASK	0x8c02

typedef struct
{
    _IO unsigned char usb_host_me				: 1;	//RW 0.0		//host mode
    _IO unsigned char usb_rest_device			: 1;	//RW 0.0		//REST device
    _IO unsigned char usb_dma_en				: 1;	//RW 0.0		//dma enable
    _IO unsigned char usb_mram_sel				: 1;	//RW 0.0		//mram select
    _IO unsigned char usb_sof_en				: 1;	//RW 0.0		//sof enable
    _IO unsigned char usb_sof_window			: 3;	//RW 0.0		//sof window
} __attribute__((packed)) USB_HMODERegDef;		//CORE_USB_HMODE	0x8c12

typedef struct
{
    _O unsigned char usb_fifo_sendd_enp0		: 1;	//W 0.0		//send endpoint 0 data in fifo
    _O unsigned char usb_fifo_sendd_enp1		: 1;	//W 0.0		//send endpoint 1 data in fifo
    _O unsigned char usb_fifo_sendd_enp2		: 1;	//W 0.0		//send endpoint 2 data in fifo
    _O unsigned char usb_fifo_sendd_enp3		: 1;	//W 0.0		//send endpoint 3 data in fifo
    _O unsigned char usb_enp0_reply_0len_pac	: 1;	//W 0.0		//endpoint 0 reply zero length packet to host
    _O unsigned char usb_enp1_reply_0len_pac	: 1;	//W 0.0		//endpoint 1 reply zero length packet to host
    _O unsigned char usb_enp2_reply_0len_pac	: 1;	//W 0.0		//endpoint 2 reply zero length packet to host
    _O unsigned char usb_enp3_reply_0len_pac	: 1;	//W 0.0		//endpoint 3 reply zero length packet to host
} __attribute__((packed)) USB_TRIGRegDef;		//CORE_USB_TRIG	0x8c20

typedef struct
{
    _O unsigned char usb_enp0_stall				: 1;	//W 0.0		//set endpoint 0 to STALL
    _O unsigned char usb_enp1_in_stall			: 1;	//W 0.0		//set endpoint 1 IN to STALL
    _O unsigned char usb_enp1_out_stall			: 1;	//W 0.0		//set endpoint 1 OUT to STALL
    _O unsigned char usb_enp2_in_stall			: 1;	//W 0.0		//set endpoint 2 IN to STALL
    _O unsigned char usb_enp2_out_stall			: 1;	//W 0.0		//set endpoint 2 OUT to STALL
    _O unsigned char usb_enp3_in_stall			: 1;	//W 0.0		//set endpoint 3 IN to STALL
    _O unsigned char usb_enp3_out_stall			: 1;	//W 0.0		//set endpoint 3 OUT to STALL
    _O unsigned char usb_shost_me				: 1;	//W 0.0		//host mode start
} __attribute__((packed)) USB_STALLRegDef;		//CORE_USB_STALL	0x8c21

typedef struct
{
    _O unsigned char usb_enp0_clr				: 1;	//W 0.0		//clear endpoint 0 to data0
    _O unsigned char usb_enp1_in_clr			: 1;	//W 0.0		//clear endpoint 1 IN to data0
    _O unsigned char usb_enp1_out_clr			: 1;	//W 0.0		//clear endpoint 1 OUT to data0
    _O unsigned char usb_enp2_in_clr			: 1;	//W 0.0		//clear endpoint 2 IN to data0
    _O unsigned char usb_enp2_out_clr			: 1;	//W 0.0		//clear endpoint 2 OUT to data0
    _O unsigned char usb_enp3_in_clr			: 1;	//W 0.0		//clear endpoint 3 IN to data0
    _O unsigned char usb_enp3_out_clr			: 1;	//W 0.0		//clear endpoint 3 OUT to data0
    _O unsigned char usb_reset					: 1;	//W 0.0		//RESET
} __attribute__((packed)) USB_TOGGLERegDef;		//CORE_USB_TOGGLE	0x8c22

typedef struct
{
    _I unsigned short setup_packet				: 1;	//RO 0.0		//Got SETUP packet
    _I unsigned short suspend_state				: 1;	//RO 0.0		//suspend state
    _I unsigned short nak_state					: 1;	//RO 0.0		//NAK state
    _I unsigned short reset_state				: 1;	//RO 0.0		//RESET state
    _I unsigned short ack_state					: 1;	//RO 0.0		//ACK state
    _I unsigned short rx_ready_state				: 1;	//RO 0.0		//rx ready state
    _I unsigned short rx_remain3_assert			: 1;	//RO 0.0		//if rx_remain<=3, assert
    _I unsigned short tx_done0					: 1;	//RO 0.0		//tx_done0
    _I unsigned short tx_done1					: 1;	//RO 0.0		//tx_done1
    _I unsigned short tx_done2					: 1;	//RO 0.0		//tx_done2
    _I unsigned short tx_done3					: 1;	//RO 0.0		//tx_done3
    _I unsigned short irq_status_reserved		: 5;	//RO 0.0		//reserved
} __attribute__((packed)) IRQ_statusRegDef;		//CORE_IRQ_status	0x8c32

typedef struct
{
    _I unsigned char stall_enp0					: 1;	//R 0.0		//Endpoint 0 is STALL
    _I unsigned char stall_enp1_in				: 1;	//R 0.0		//Endpoint 1 IN is STALL
    _I unsigned char stall_enp1_out				: 1;	//R 0.0		//Endpoint 1 OUT is STALL
    _I unsigned char stall_enp2_in				: 1;	//R 0.0		//Endpoint 2 IN is STALL
    _I unsigned char stall_enp2_out				: 1;	//R 0.0		//Endpoint 2 OUT is STALL
    _I unsigned char stall_enp3_in				: 1;	//R 0.0		//Endpoint 3 IN is STALL
    _I unsigned char stall_enp3_out				: 1;	//R 0.0		//Endpoint 3 OUT is STALL
    _IO unsigned char stall_status_reserved		: 1;	//RW 0.0		//not used
} __attribute__((packed)) STALL_STATUSRegDef;		//CORE_STALL_STATUS	0x8c34

typedef struct
{
    _I unsigned char usb_fifo_enp0_busy			: 1;	//R 0.0		//Endpoint 0 IN fifo is busy
    _I unsigned char usb_fifo_enp1_busy			: 1;	//R 0.0		//Endpoint 1 IN fifo is busy
    _I unsigned char usb_fifo_enp2_busy			: 1;	//R 0.0		//Endpoint 2 IN fifo is busy
    _I unsigned char usb_fifo_enp3_busy			: 1;	//R 0.0		//Endpoint 3 IN fifo is busy
    _I unsigned char usb_dn_status				: 1;	//R 0.0		//USB DN status
    _I unsigned char usb_dp_status				: 1;	//R 0.0		//USB DP status
    _I unsigned char usb_rx_empty_state			: 1;	//R 0.0		//rx empty state
    _I unsigned char usb_tx_busy_reserved		: 1;	//R 0.0		//not used
} __attribute__((packed)) USB_TX_BUSYRegDef;		//CORE_USB_TX_BUSY	0x8c35

typedef struct
{
    _IO unsigned int qspi_txaddr				: 16;	//RW 0.0
    _IO unsigned int qspi_txlen					: 16;	//RW 0.0
} __attribute__((packed)) QSPI_TX_CTRL_ADDRRegDef;		//CORE_QSPI_TX_CTRL	0x1f068

typedef struct
{
    _IO unsigned int qspi_rxaddr				: 16;	//RW 0.0
    _IO unsigned int qspi_rxlen					: 16;	//RW 0.0
} __attribute__((packed)) QSPI_RX_CTRL_ADDRRegDef;		//CORE_QSPI_RX_CTRL	0x1f06c

typedef struct
{
    _IO unsigned int mode_sel					: 2;	//RW 0.0		//0:standard SPI, 2:quad SPI
    _IO unsigned int intr_clr					: 1;	//RW 0.0		//interrupt clear
    _IO unsigned int async_fifo_clr				: 1;	//RW 0.0		//async fifo clear
    _IO unsigned int din_sample_mode			: 1;	//RW 0.0		//data sample edge select
    _IO unsigned int clk_en						: 1;	//RW 0.0		//clock enable
    _IO unsigned int intr_en					: 1;	//RW 0.0		//interrupt enable
    _IO unsigned int clkp						: 1;	//RW 0.0		//1:spi clock is reverted
    _IO unsigned int wslen						: 4;	//RW 0.0		//dummy bits count
    _IO unsigned int delay						: 4;	//RW 0.0		//multi bit mode address bytes count
} __attribute__((packed)) QSPI_CTRL_ADDRRegDef;		//CORE_QSPI_CTRL	0x1f070

typedef struct
{
    _IO unsigned int qspi_cmdaddr				: 16;	//RW 0.0
    _IO unsigned int qspi_cmdlen				: 16;	//RW 0.0
} __attribute__((packed)) QSPI_CMD_CTRL_ADDRRegDef;		//CORE_QSPI_CMD_CTRL	0x1f074


typedef struct
{
    _IO unsigned char da_codec_reg_pwr_sel      : 1;    //RW 0      //audio 3 ldo(adcldo/dacldo/hpldo) avss17 voltage selected, 1: when codec power supply> 3.6V; 0: when codec power supply <3.6V
    _IO unsigned char da_adc_pwrsw_en           : 1;    //RW 0      //audio adc power switch enable, 1: used power supply outside(codec_avdd), 0: used adc ldo inside
    _IO unsigned char da_ldo_daccore_en         : 1;    //RW 0      //audio dac core ldo enalbe
    _IO unsigned char da_ldohp_dacdrv_en        : 1;    //RW 0      //audio pa ldo enalbe
    _IO unsigned char da_adc_reg_en             : 1;    //RW 0      //audio adc ldo enable
    _IO unsigned char da_mic_reg_en             : 1;    //RW 0      //pga/micbias ldo enable
} __attribute__((packed)) codec_ldo_en0RegDef;      //CORE_codec_ldo_en0    0x8b00

typedef struct
{
    _IO unsigned char rg_mic_hvreg_vc           : 2;    //RW 1      //pga/micbias ldo voltage control
    _IO unsigned char rg_adc_reg_excap_en       : 1;    //RW 0      //audio adc ldo mode selected, 1: with external capacitor, 0: capless
    _IO unsigned char rg_adc_reg_vol_sel        : 3;    //RW 2      //audio adc ldo output voltage selected
    _IO unsigned char rg_adc_reg_ibias_sel      : 2;    //RW 1      //audio adc ldo bias current selected
} __attribute__((packed)) codec_ldo_cfg0RegDef;     //CORE_codec_ldo_cfg0   0x8b01

typedef struct
{
    _IO unsigned char rg_ibsel_hpldo            : 2;    //RW 1      //audio pa ldo bias current selected
    _IO unsigned char rg_ldo_excap_en           : 1;    //RW 0      //audio dac ldo mode selected, 1: with external capacitor, 0: capless
    _IO unsigned char rg_ldo_voutsel            : 3;    //RW 2      //audio dac ldo  output voltage selected
    _IO unsigned char rg_ibsel_ldo              : 2;    //RW 0      //audio dac ldo bias current selected
} __attribute__((packed)) codec_ldo_cfg1RegDef;     //CORE_codec_ldo_cfg1   0x8b02

typedef struct
{
    _IO unsigned char rg_ldohp_byp              : 1;    //RW 0      //audio pa ldo bypass enable
    _IO unsigned char rg_ldohp_excap_en         : 1;    //RW 0      //audio pa ldo external capacitor load enable: 0 wo external cap,1 wi external cap
    _IO unsigned char rg_ldohp_voutsel          : 3;    //RW 2      //audio pa ldo output voltage selected
    _IO unsigned char rg_ldohp_ocp_sel          : 1;    //RW 0      //audio pa ldo over current portection threshold selected: 0=80mA,1= 200mA
} __attribute__((packed)) codec_ldo_cfg2RegDef;     //CORE_codec_ldo_cfg2   0x8b04

typedef struct
{
    _IO unsigned char da_vmid_discharge         : 1;    //RW 1      //audio voltage reference discharge enable
    _IO unsigned char da_vmid_en                : 1;    //RW 0      //audio voltage reference enable, 1:charge to vmid, 0:discharge to gnd
    _IO unsigned char da_vmid_bias_en           : 1;    //RW 0      //audio reference generator bias enable
    _IO unsigned char da_vmid_lvd_en            : 1;    //RW 0      //audio power supply low voltage detect enable
    _IO unsigned char da_vmid_extcap_en         : 1;    //RW 0      //audio voltage reference operation mode selected, 1: with external capacitor; 0: without external capacitor
    _IO unsigned char da_vmid_bg_en             : 1;    //RW 0      //audio bandgap enable
    _IO unsigned char res                       : 2; //RW 0      //res
} __attribute__((packed)) codec_vmid_en0RegDef;     //CORE_codec_vmid_en0   0x8b05

typedef struct
{
    _IO unsigned char rg_vmid_isel_lsb          : 5;    //RW 0      //audio voltage reference charge/discharge current selection,coarse bit lsb
    _IO unsigned char rg_vmid_lvdsel            : 3;    //RW 0      //audio power supply low voltage detect threshold selected
} __attribute__((packed)) codec_vmid_cfg0RegDef;        //CORE_codec_vmid_cfg0  0x8b06

typedef struct
{
    _IO unsigned char rg_vmid_isel_msb          : 5;    //RW 0      //audio voltage reference charge/discharge current selection,coarse bit msb
    _IO unsigned char rg_vmid_discharge_cc      : 1;    //RW 1      //audio voltage reference discharge slew rate limited enable
    _IO unsigned char rg_vmid_discharge_auto    : 1;    //RW 1      //audio voltage reference auto discharge enable
    _IO unsigned char rg_vmid_lvd_vrefsel       : 1;    //RW 0      //audio power supply low voltage detect source selected, 1: codec_avdd; 0: buck output
} __attribute__((packed)) codec_vmid_cfg1RegDef;        //CORE_codec_vmid_cfg1  0x8b07

typedef struct
{
    _IO unsigned char rg_vmid_bg_fastsetting    : 1;    //RW 0      //audio bandgap start up fastsetting enable
    _IO unsigned char  rg_vmid_bg_man_st        : 1;    //RW 0      //audio bandgap manual start up enable
    _IO unsigned char rg_vmid_vref_sel          : 1;    //RW 1      //audio voltage reference mode selected, 1:from bandgap; 0:from codec_avdd through resistor divider
    _IO unsigned char rg_vmid_iadjust           : 4;    //RW 0      //audio voltage reference charge/discharge current selection,fine
    _IO unsigned char res                       : 1; //
} __attribute__((packed)) codec_vmid_cfg2RegDef;        //CORE_codec_vmid_cfg2  0x8b08

typedef struct
{
    _IO unsigned char rg_vmid_bg_trim           : 4;    //RW 4      //audio bandgap output voltage trimming
    _IO unsigned char rg_vmid_bg_rct            : 4;    //RW 8      //audio bandgap CTAT current trimming
} __attribute__((packed)) codec_vmid_cfg3RegDef;        //CORE_codec_vmid_cfg3  0x8b09

typedef struct
{
    _I unsigned char ad_vmid_low_12             : 1;    //RO nan        //audio reference discharging low indicator
    _I unsigned char ad_vmid_zero_12            : 1;    //RO nan        //audio reference discharging to ground finished indicator
    _I unsigned char ad_vmid_ok_12              : 1;    //RO nan        //audio reference charging finished indicator
    _I unsigned char ad_vmid_lvd_12             : 1;    //RO nan        //audio power supply low voltage indicator output
} __attribute__((packed)) codec_vmid_indRegDef;     //CORE_codec_vmid_ind   0x8b0a

typedef struct
{
    _IO unsigned char da_pga_dcoc_en            : 1;    //RW 0      //audio pga dc offset calibration enable
    _IO unsigned char da_pga_gm_en              : 1;    //RW 0      //audio pga gm stage enable
    _IO unsigned char da_pga_gm_fst_en          : 1;    //RW 0      //audio pga gm stage fastsetting enable
    _IO unsigned char da_pga_cz_en              : 1;    //RW 0      //audio pga crossing zero detected enable
    _IO unsigned char da_pga_gain_boost_en      : 1;    //RW 0      //audio pga gainboost enable, 1: +6dB; 0: +0dB
    _IO unsigned char da_sdm_dem_en             : 1;    //RW 1      //audio adc dynamic element match enable
    _IO unsigned char da_sdm_en                 : 1;    //RW 0      //audio adc enable
    _IO unsigned char da_aadc_top_ibias_en      : 1;    //RW 0      //audio adc/pga bias current enable
} __attribute__((packed)) au_adc_en0RegDef;     //CORE_au_adc_en0   0x8b0b

typedef struct
{
    _IO unsigned char da_micbias_en             : 1;    //RW 0      //micbias generator enable
    _IO unsigned char da_pga_en                 : 1;    //RW 0      //audio pga enable
    _IO unsigned char da_pga_diff_en            : 1;    //RW 0      //audio pga fully differential input enable, 1: differential input, 0: single ended input
    _IO unsigned char da_pga_rbias_en           : 1;    //RW 0      //audio pga inchip resistor(connect to micbias) path enable
} __attribute__((packed)) au_adc_en1RegDef;     //CORE_au_adc_en1   0x8b0c

typedef struct
{
    _IO unsigned char rg_sdm_cmp_isel           : 2;    //RW 1      //audio adc comparator bias current selected
    _IO unsigned char rg_sdm_ota1_isel          : 2;    //RW 1      //audio adc 1st integrator opa bias current selected
    _IO unsigned char rg_sdm_ota2_isel          : 2;    //RW 1      //audio adc 2nd integrator opa bias current selected
    _IO unsigned char rg_sdm_ref_isel           : 2;    //RW 1      //audio adc reference buffer bias current selected
} __attribute__((packed)) au_adc_cfg0RegDef;        //CORE_au_adc_cfg0  0x8b0d

typedef struct
{
    _IO unsigned char rg_pga_opa2_ibias_sel     : 2;    //RW 2      //audio pga signle ended opa bias current selected
    _IO unsigned char rg_pga_opa1_ibias_sel     : 2;    //RW 2      //audio pga fully differential opa bias current selected
    _IO unsigned char rg_pga_cz_ibias_sel       : 2;    //RW 1      //audio pga crossing-zero comparator bias current selected
    _IO unsigned char rg_pga_gm_ibias_sel       : 2;    //RW 1      //audio pga gm stage bias current selected
} __attribute__((packed)) au_adc_cfg1RegDef;        //CORE_au_adc_cfg1  0x8b0e

typedef struct
{
    _IO unsigned char rg_pga_gc                 : 5;    //RW 4      //audio pga voltage gain control
    _IO unsigned char rg_auadc_clk_dly          : 3;    //RW 0      //audio adc clkin delay selected
} __attribute__((packed)) au_adc_cfg3RegDef;        //CORE_au_adc_cfg3  0x8b10

typedef struct
{
    _IO unsigned char rg_pga_rcfilt_coarse      : 2;    //RW 1      //pga capless input rc filter bandwitch coarse trimming selected
    _IO unsigned char rg_pga_dcoc_ibc           : 2;    //RW 1      //audio pga dc offset calibration bias current selected
} __attribute__((packed)) au_adc_cfg4RegDef;        //CORE_au_adc_cfg4  0x8b11

typedef struct
{
    _IO unsigned char rg_micbias_rc             : 3;    //RW 2      //micbias inchip resistor(connect to pga) control
    _IO unsigned char rg_micbias_ibias_sel      : 2;    //RW 1      //micbias bias current selected
    _IO unsigned char rg_pga_rcfilt_fine        : 3;    //RW 2      //pga capless input rc filter bandwitch fine trimming selected
} __attribute__((packed)) au_adc_cfg5RegDef;        //CORE_au_adc_cfg5  0x8b12

typedef struct
{
    _IO unsigned char rg_micbias_vc             : 6;    //RW 3a     //micbias output voltage control
    _IO unsigned char rg_micbias_mode           : 2;    //RW 0      //micbias operation mode selected
} __attribute__((packed)) au_adc_cfg6RegDef;        //CORE_au_adc_cfg4  0x8b13

typedef struct
{
    _IO unsigned char da_drv_single_end_l_en    : 1;    //RW 0      //audio pa single ended(virtual differential)output enable,left channel, 1:single ended output; 0:differential output
    _IO unsigned char da_drv_single_end_r_en    : 1;    //RW 0      //audio pa single ended(virtual differential)output enable,right channel, 1:single ended output; 0:differential output
    _IO unsigned char da_dac_ibias_en           : 1;    //RW 0      //audio dac current bias generator enable
    _IO unsigned char da_dcoc_pgapath_l_en      : 1;    //RW 0      //audio pa dc offset cancelled pga feedback path enable,left channel
    _IO unsigned char da_dcoc_pgapath_r_en      : 1;    //RW 0      //audio pa dc offset cancelled pga feedback path enable,right channel
    _IO unsigned char da_dcoc_pgapath_short     : 1;    //RW 0      //audio pga input short enable, effetive when  da_dcoc_pgapath_r/l_en=1
    _IO unsigned char da_dcoc_sarpath_en        : 1;    //RW 0      //audio pa dc offset cancelled saradc measure path enable,right channel
} __attribute__((packed)) au_dac_en0RegDef;     //CORE_au_dac_en0   0x8b14

typedef struct
{
    _IO unsigned char da_vrefgen_drv_r_en       : 1;    //RW 0      //audio pa vcm opa enable,right channel
    _IO unsigned char da_vrefgen_dac_r_en       : 1;    //RW 0      //audio dac reference generator enalbe, right channel
    _IO unsigned char da_dac_mute_r_en          : 1;    //RW 1      //audio dac mute enable,right channel(already deleted)
    _IO unsigned char da_drv_mute_r_en          : 1;    //RW 1      //audio pa mute enable,right channel
    _IO unsigned char da_drv_ostg_cc_r_en       : 1;    //RW 1      //audio pa output stage pulldown slew rate limited enable,right channel
    _IO unsigned char da_drv_ostg_r_en          : 1;    //RW 1      //audio pa output stage enable, 1: normal, 0:hpoutrn/hpoutrp pulldown to gnd,right channel
    _IO unsigned char da_dac_int_r_en           : 1;    //RW 0      //audio dac core enalbe,right channel
    _IO unsigned char da_drv_r_en               : 1;    //RW 0      //audio pa enalbe,right channel
} __attribute__((packed)) au_dac_en1RegDef;     //CORE_au_dac_en1   0x8b15

typedef struct
{
    _IO unsigned char da_vrefgen_drv_l_en       : 1;    //RW 0      //audio pa vcm opa enable,left channel
    _IO unsigned char da_vrefgen_dac_l_en       : 1;    //RW 0      //audio dac vcm/vrefp opa enable,left channel
    _IO unsigned char da_dac_mute_l_en          : 1;    //RW 1      //audio dac mute enable,left channel(already deleted)
    _IO unsigned char da_drv_mute_l_en          : 1;    //RW 1      //audio pa mute enable,left channel
    _IO unsigned char da_drv_ostg_cc_l_en       : 1;    //RW 1      //audio pa output stage pulldown slew rate limited enable,left channel
    _IO unsigned char da_drv_ostg_l_en          : 1;    //RW 1      //audio pa output stage enable, 1: normal, 0:hpoutrn/hpoutrp pulldown to gnd,left channel
    _IO unsigned char da_dac_int_l_en           : 1;    //RW 0      //audio dac core enalbe,left channel
    _IO unsigned char da_drv_l_en               : 1;    //RW 0      //audio pa enalbe,left channel
} __attribute__((packed)) au_dac_en2RegDef;     //CORE_au_dac_en2   0x8b16

typedef struct
{
    _IO unsigned char da_input_sel_l            : 2;    //RW 0      //audio pa input path selected, 01:dacin; 00: all disable, left channel
    _IO unsigned char da_input_sel_r            : 2;    //RW 0      //audio pa input path selected,  01:dacin; 00: all disable, right channel
    _IO unsigned char da_linein_l_en            : 1;    //RW 0      //audio pa linein input path(signle ended input) enable,left channel
    _IO unsigned char da_linein_r_en            : 1;    //RW 0      //audio pa linein input path(signle ended input) enable,right channel
    _IO unsigned char da_drv_r1_open            : 1;    //RW 1      //audio pa input resistor open (disconnect)
    _IO unsigned char da_drv_rf_open            : 1;    //RW 0      //audio pa feedback resistor open (disconnect)
} __attribute__((packed)) au_dac_en3RegDef;     //CORE_au_dac_en3   0x8b17

typedef struct
{
    _IO unsigned char rg_ibsel_dac              : 3;    //RW 2      //audio dac core bias current selected
    _IO unsigned char rg_ibsel_vref4u           : 3;    //RW 2      //audio dac vrefp generator bias current selected
    _IO unsigned char rg_ibsel_vref2u           : 2;    //RW 0      //audio dac/pa vcm generator bias current selected
} __attribute__((packed)) au_dac_cfg0RegDef;        //CORE_au_dac_cfg0  0x8b18

typedef struct
{
    _IO unsigned char rg_gc_dacin_l             : 2;    //RW 0      //audio dac gain control for left channel
    _IO unsigned char rg_gc_dacin_r             : 2;    //RW 0      //audio dac gain control for right channel
    _IO unsigned char rg_vrefp_res_sel          : 1;    //RW 0      //audio dac vrefp generator resistor selected
    _IO unsigned char rg_ibsel_drv              : 3;    //RW 2      //audio pa bias current selected
} __attribute__((packed)) au_dac_cfg1RegDef;        //CORE_au_dac_cfg1  0x8b19

typedef struct
{
    _IO unsigned char rg_tstdac_sel             : 2;    //RW 0      //audio dac test path selected
    _IO unsigned char rg_gc_linein_l            : 3;    //RW 2      //audio pa linein path gain control,left channel
    _IO unsigned char rg_gc_linein_r            : 3;    //RW 2      //audio pa linein path gain control,right channel
} __attribute__((packed)) au_dac_cfg2RegDef;        //CORE_au_dac_cfg2  0x8b1a

typedef struct
{
    _IO unsigned char rg_vrefgen_ccsel_int      : 1; //RW 0      //audio dac vrefp opa miller compensation capacitor selected
    _IO unsigned char rg_vrefgen_ccsel_intopa   : 1; //RW 1      //audio dac vcm opa miller compensation capacitor selected
    _IO unsigned char rg_vrefgen_ccsel_drv      : 1; //RW 1      //audio pa vcm opa miller compensation capacitor selected
    _IO unsigned char rg_drv_ccn_sel            : 2; //RW 0      //audio pa nmos (pull)miller compensation capacitor selected
    _IO unsigned char rg_drv_ccp_sel            : 2; //RW 3      //audio pa pmos(push) miller compensation capacitor selected
    _IO unsigned char rg_tstdac_en              : 1; //RW 0      //audio dac test path enable
} __attribute__((packed)) au_dac_cfg3RegDef;    //CORE_au_dac_cfg3  0x8b1b


#endif

