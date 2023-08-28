#ifndef _YC11XX_H_
#define _YC11XX_H_
#include <string.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char 		uint8_t;
typedef unsigned short  	uint16_t;
#ifndef _UINT32_T_DECLARED
typedef unsigned int 		uint32_t;
#define _UINT32_T_DECLARED
#endif
typedef unsigned long long	uint64_t;

typedef signed char 		int8_t;
typedef signed short     	int16_t;
#ifndef _INT32_T_DECLARED
typedef signed int 		    int32_t;
#define _INT32_T_DECLARED
#endif
typedef signed long long	int64_t;

typedef unsigned char       byte;
typedef unsigned short      word;


#ifndef FunctionalState
typedef enum {DISABLE = 0, ENABLE = 1} FunctionalState;
#define IS_FUNCTIONAL_STATE(state) ((state== DISABLE) || (state == ENABLE))
#endif

#ifndef FunctionalState
typedef enum {ERROR = 0, SUCCESS = 1} ErrorStatus;
#define IS_ERROR_STATE(status) ((status== ERROR) || (status == SUCCESS))
#endif


#ifndef  OS_ENTER_CRITICAL
extern uint16_t gOsEnterCriticalCnt;
#define OS_INITIAL_CRITICAL()   {gOsEnterCriticalCnt = 0;}
#define OS_ENTER_CRITICAL()     {__asm("csrc mstatus, 0x00000008"); gOsEnterCriticalCnt++;}
#define OS_EXIT_CRITICAL()      {if((--gOsEnterCriticalCnt) == 0) { __asm("csrs mstatus, 0x00000008");}}
#endif


#define CLOCK_HALVED

#define BIT0        (0x00000001U)
#define BIT1        (0x00000002U)
#define BIT2        (0x00000004U)
#define BIT3        (0x00000008U)
#define BIT4        (0x00000010U)
#define BIT5        (0x00000020U)
#define BIT6        (0x00000040U)
#define BIT7        (0x00000080U)
#define BIT8        (0x00000100U)
#define BIT9        (0x00000200U)
#define BIT10       (0x00000400U)
#define BIT11       (0x00000800U)
#define BIT12       (0x00001000U)
#define BIT13       (0x00002000U)
#define BIT14       (0x00004000U)
#define BIT15       (0x00008000U)
#define BIT16       (0x00010000U)
#define BIT17       (0x00020000U)
#define BIT18       (0x00040000U)
#define BIT19       (0x00080000U)
#define BIT20       (0x00100000U)
#define BIT21       (0x00200000U)
#define BIT22       (0x00400000U)
#define BIT23       (0x00800000U)
#define BIT24       (0x01000000U)
#define BIT25       (0x02000000U)
#define BIT26       (0x04000000U)
#define BIT27       (0x08000000U)
#define BIT28       (0x10000000U)
#define BIT29       (0x20000000U)
#define BIT30       (0x40000000U)
#define BIT31       (0x80000000U)

typedef enum
{
    RAM_UseBTSRam = 0,
    RAM_UseM0MRam,
} RAM_RegionUse;

typedef struct
{
    unsigned int buck_setting_area : 28;	//User should not change this area

    unsigned int user_bit_second_power_on : 1;  //R 0		//For Check First Power On
    unsigned int user_bit_charge_power_off : 1; //R 0		//For Check Last is charge Power Off
    unsigned int user_bit_reserved : 2;	        //R 0		//For Future Use
} LPM_AppCtrlRegDef;    //LPM_CTRL	0x833e

typedef struct
{
    unsigned int da_ldo_config : 24;	//User should not change this area
    unsigned int da_ldo_dig_core_vsel : 3;
    unsigned int da_ldo_io_doze_en : 1;
    unsigned int da_ldo_io_ocp_en : 1;
    unsigned int da_ldo_io_vsel : 1;
    unsigned int da_ldo_io_en : 1;
    unsigned int reserved : 1;
} LPM_DaLdoRegDef;      //LPM_CTRL	0x83a1

/** @} */ /* End of group Configuration_of_CMSIS */
typedef enum
{
    /* -------------------  Cortex-M0 Processor Exceptions Numbers  -------------------                                                                 */
    Reset_IRQn                    = -15,              /*!<   1  Reset Vector, invoked on Power up and warm reset                  */
    NonMaskableInt_IRQn           = -14,              /*!<   2  Non maskable Interrupt, cannot be stopped or preempted         */
    HardFault_IRQn                = -13,              /*!<   3  Hard Fault, all classes of Fault                                                 */
    SVCall_IRQn                   =  -5,              /*!<  11  System Service Call via SVC instruction                                  */
    DebugMonitor_IRQn             =  -4,              /*!<  12  Debug Monitor    	*/
    PendSV_IRQn                   =  -2,              /*!<  14  Pendable request for system service                                       */
    /* ---------------------  xx Specific Interrupt Numbers  --------------------                                                                     */
    USB_IRQn              =   16,
    iicd_IRQn             =   17,
    qspi_IRQn             =   18,
    spid_IRQn             =   19,
    uart_IRQn             =   20,
    uartb_IRQn            =   21,
    adcd_IRQn             =   22,
    i2s_IRQn              =   23,
    bt_IRQn               =   24,
    host_IRQn            =   25,
    gpio_IRQn              =   26,
    sbc0_IRQn             =   27,
    sbc1_IRQn             =   28,
    fft_IRQn              =   29,
    dma_IRQn              =   30,
    sdio_IRQn             =   31,
    pwm0_IRQn           =   32,
    pwm1_IRQn           =   33,
    pwm2_IRQn           =   34,
    pwm3_IRQn           =   35,
    pwm4_IRQn           =   36,
    pwm5_IRQn           =   37,
    pwm6_IRQn           =   38,
    pwm7_IRQn           =   39,
    wdt2_IRQn             =   40,
    wdt_IRQn             =   41,
    deb_IRQn			= 42,
    iicd1_IRQn			= 43,
    sdma_intr			= 44,
    rv_tm_IRQn			= 48,
    qspi1_IRQn			= 49,
    qspi2_IRQn			= 50,
} IRQn_Type;

#define reg_check_ram_m0(reg)				(((int)(reg) & 0x00010000) != 0)
#define reg_map(reg)						((int)(reg) | 0x10000000)
#define reg_map_m0(reg)						((int)(reg) | 0x10010000)
#define reg_map2(reg)						((int)(reg) | 0x1001c000)
#define reg_map_rv(reg)						((int)(reg) | 0x1001f000)
#define PREFETCH_LINE(addr)					*(volatile int*)addr = 0
#define des_ctrl							*(volatile uint8_t*)0x30000002
#define des_key(x)							*(volatile uint8_t*)(0x30000003 + x)
#define des_in(x)							*(volatile uint8_t*)(0x30000018 + x)
#define crypt_status						*(volatile uint8_t*)0x30010000
#define des_out(x)							*(volatile uint8_t*)(0x30010004 + x)
#define des_start							*(volatile uint8_t*)0x30008000
#define rsa_exp(x)							*(volatile int32_t*)(0x30020000 + x*4)
#define rsa_out(x)							*(volatile int32_t*)(0x30020000 + x*4)
#define rsa_in(x)							*(volatile int32_t*)(0x30020080 + x*4)
#define rsa_mod(x)							*(volatile int32_t*)(0x30020100 + x*4)
#define rsa_ctrl							*(volatile int32_t*)0x30020180
#define reg_check_qspix(qspix)				(qspix != 0)
#define reg_check_uart_ram(reg)				((reg & 0x8000) != 0)


#define YC1121G_RV_BASE                          (0x0001f000)   /* RV Base Address */

#define RV_IRQ_PRIO_BASEADDR                    (YC1121G_RV_BASE + 0x0400) //0x1f400
#define RV_IRQ_PEND_BASEADDR                    (YC1121G_RV_BASE + 0x0800) //0x1f800
#define RV_IRQ_ENAB_BASEADDR                    (YC1121G_RV_BASE + 0x0a00) //0x1fa00
#define RV_IRQ_THRD_BASEADDR                    (YC1121G_RV_BASE + 0x0c00) //0x1fc00
#define RV_IRQ_CLAM_BASEADDR                    (YC1121G_RV_BASE + 0x0c04) //0x1fc04

#define RV_IRQ_PRIO(x)				*(volatile uint8_t*)(RV_IRQ_PRIO_BASEADDR + ((uint8_t)((uint8_t)(x)/2)))
#define RV_IRQ_PEND(x)				*(volatile uint8_t*)(RV_IRQ_PEND_BASEADDR + ((uint8_t)((uint8_t)(x)/8)))
#define RV_IRQ_ENAB(x)				*(volatile uint8_t*)(RV_IRQ_ENAB_BASEADDR + ((uint8_t)((uint8_t)(x)/8)))
#define RV_IRQ_THOD					*(volatile int*)(RV_IRQ_THRD_BASEADDR)
#define RV_IRQ_CLAM					*(volatile int*)(RV_IRQ_CLAM_BASEADDR)

#define TRACE_FIFO				            *(volatile int*)0xe0002020
#define NVIC_ISER				            *(volatile int*)0xe000e100
#define NVIC_ICER				            *(volatile int*)0xe000e180


/* SysTick control & status */
#define INITCPU_SYST_CSR     ((volatile unsigned int *)0x1001f000)
/* SysTick Reload value */
#define INITCPU_SYST_RVR     ((volatile unsigned int *)0x1001f008)
/* SysTick Current value */
#define INITCPU_SYST_CVR     ((volatile unsigned int *)0x1001f00c)
/* SysTick CSR register bits */
#define INITCPU_SYST_CSR_COUNTFLAG 16
#define INITCPU_SYST_CSR_CLKSOURCE 8
#define INITCPU_SYST_CSR_TICKINT   1
#define INITCPU_SYST_CSR_ENABLE    3
#define INITCPU_SYSTICK_SYSCLOCK		  0xFF
#define INITCPU_SYSTICK_HALF_SYSCLOCK	   0


#define TO_16BIT_ADDR(A) (((int)A)&0xFFFF)

#define HREADTool(reg)		*(volatile byte*)(0x01000000|reg)
#define HREAD(reg)			*(volatile byte*)(reg_map(reg))
#define HREAD2(reg)			*(volatile byte*)(reg_map2(reg))
#define HREADRV(reg)		*(volatile int*)(reg_map_rv(reg))
#define HREAD3(reg)			*(volatile byte*)(reg_map_m0(reg))
#define HREAD4(reg)			*(volatile int*)(reg_map_m0(reg))
#define HREADM(addr)		*(volatile byte*)(addr)
#define HREADW(reg)			(HREAD(reg) | HREAD(reg + 1) << 8)
#define HREAD24BIT(reg)		((int)(HREAD(reg)) | (HREAD(reg + 1) << 8)| (HREAD(reg + 2) << 16))
#define HREADADDR3(reg)		((int)(HREAD(reg)) | (HREAD(reg + 1) << 8)| (HREAD(reg + 2) << 16))
#define HWRITE(reg, val)    do {(HREAD(reg) = (byte)(val));}while(0)
#define HWRITEW(reg, val)   do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8); }while(0)

#define HREADL(reg)		        (int)(HREAD(reg)) | (HREAD(reg + 1) << 8)| (HREAD(reg + 2) << 16)| (HREAD(reg + 3) << 24)
#define HWRITEL(reg, val)	    do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8);HWRITE(reg + 2, (int)(val) >> 16);HWRITE(reg + 3, (int)(val) >> 24); }while(0)
#define HWRITE_INLINE(reg,val)  do {(HREAD(reg) = (byte)(val));}while(0)
#define HWRITEW_INLINE(reg,val) do { HWRITE_INLINE(reg, (int)(val));HWRITE_INLINE(reg + 1, (int)(val) >> 8); }while(0)
#define HREAD_INLINE(reg)	    ((HREAD(reg)))

#define HWRITE2(reg,val) 	    do {HREAD2(reg)=(byte)(val); volatile int n = 1; while(n--);}while(0)
#define HWRITERV(reg,val) 	    do {HREADRV(reg)=(int)(val); volatile int n = 1; while(n--);}while(0)
#define HWRITE3(reg,val) 	    do {HREAD3(reg)=(byte)(val); volatile int n = 1; while(n--);}while(0)
#define HWRITE4(reg,val) 	    do {HREAD4(reg)=(int)(val); volatile int n = 1; while(n--);}while(0)
#define HWRITEM(addr,val)       do {HREADM(addr)=(byte)val;  volatile int n = 1; while(n--);} while(0)
#define HWRITE24BIT(reg,val)    do { HWRITE(reg, (int)(val));HWRITE(reg + 1, (int)(val) >> 8); HWRITE(reg + 2, (int)(val) >> 16); }while(0)

#define HWOR(reg, val)	    HWRITE(reg, ((HREAD(reg)) | (val)))
#define HWCOR(reg, val)	    HWRITE(reg, HREAD(reg) & ~(val))
#define HWORW(reg, val)	    HWRITEW(reg, ((HREADW(reg) )| (val)))
#define HWCORW(reg, val)    HWRITEW(reg, HREADW(reg) & ~(val))

#define SETBIT(reg, val)    HWRITE(reg, HREAD(reg) | (val))
#define CLRBIT(reg, val)    HWRITE(reg, HREAD(reg) & ~(val))

#define HREAD_STRUCT(reg, stc)      (*(uint8_t*)stc = *(volatile byte*)(reg_map(reg)))
#define HWRITE_STRUCT(reg, stc)     (*(volatile byte*)(reg_map(reg)) = *(uint8_t*)(stc))
#define HREADW_STRUCT(reg, stc)     (*(uint16_t*)stc = HREADW(reg))
#define HWRITEW_STRUCT(reg, stc)    HWRITEW(reg, *(uint16_t*)(stc))
#define HREADL_STRUCT(reg, stc)         (*(uint32_t*)stc = HREADL(reg))
#define HWRITEL_STRUCT(reg, stc)        HWRITEL(reg, *(uint32_t*)(stc))
#define HREAD24BIT_STRUCT(reg, stc)     (*(uint32_t*)stc = HREAD24BIT(reg))
#define HWRITE24BIT_STRUCT(reg, stc)    HWRITE24BIT(reg, *(uint32_t*)(stc))

#define RB_UPDATE_PTR(p,s,e) ((p) == (e))?((p)=(s)):((p)++)
#define UART_RAM_SELECT(p,s) ((p)?(reg_map_m0(s)):(reg_map(s)))

/********************************************************************************
** Macros to get and put bytes to and from a stream (Big Endian format)
*/
void _assert_handler(const char *file, int line, const char *func);
#define _ASSERT(x)	\
	if (!(x))                                                                    \
		{                                                                             \
		_assert_handler(__FILE__,__LINE__,__FUNCTION__);\
		}
#define _ASSERT_FAULT()	\
		{                                                                             \
		OS_ENTER_CRITICAL();\
		_assert_handler(__FILE__,__LINE__,__FUNCTION__);\
		}
/***********************************core_clkoff*************************************************/
typedef enum
{
    REG_CLOCK_OFF_AUTH_ROM		= 0x0001,
    REG_CLOCK_OFF_EFUSE			= 0x0002,
    REG_CLOCK_OFF_DEBUG_UART	= 0x0004,
    REG_CLOCK_OFF_ITRACE		= 0x0008,
    REG_CLOCK_OFF_QSPI			= 0x0010,
    REG_CLOCK_OFF_AUDIO_DAC		= 0x0020,
    REG_CLOCK_OFF_VOICE_FILTER	= 0x0040,
    REG_CLOCK_OFF_I2C			= 0x0080,
    REG_CLOCK_OFF_CM0 			= 0x0100,
    REG_CLOCK_OFF_MRAM			= 0x0200,
    REG_CLOCK_OFF_USB			= 0x0400,
    REG_CLOCK_OFF_SBC			= 0x0800,
    REG_CLOCK_OFF_SPI			= 0x1000,
    REG_CLOCK_OFF_SDIO			= 0x2000,
    REG_CLOCK_OFF_I2S			= 0x4000,
    REG_CLOCK_OFF_UART			= 0x8000,
    REG_CLOCK_OFF_EQ			= 0x10000,
    REG_CLOCK_OFF_MIC			= 0x10001,
} CORE_ClockOffEnumDef;

#define MHz 1000000
#define SYSCLK_XTAL_24MHZ  24
#define SYSCLK_DPLL_48MHZ  48
#define SYSCLK_DPLL_64MHZ  64
#define SYSCLK_DPLL_96MHZ  96
#define SYSCLK_DPLL_192MHZ 192

void Toolmemcpy(uint8_t *dest, const uint8_t *src, uint16_t len);
uint8_t hexchar2hex(char hexchar);
void xmemcpy(uint8_t *dest, const uint8_t *src, uint16_t len);
uint16_t xstrlen(const char *s);
void error_handle(void);

void whileDelay(int delayValue);
void whileDelayshort(int delayValue);

bool xramcmp(volatile uint8_t *op1, volatile uint8_t *op2, int len);
void xramcpy(volatile uint8_t *des, volatile uint8_t *src, int len);
uint32_t math_abs(int32_t value);
uint32_t CoreReg_ClkControl(CORE_ClockOffEnumDef clkIndex, bool isEnable);

void CoreReg_LpmWriteCommon(uint32_t writeVal, uint16_t writeLpmReg, uint8_t writeLpmVal);
void CoreReg_LpmWrite(uint32_t writeVal, uint8_t writeLpmVal);
void CoreReg_LpmWriteCtrl(uint32_t writeVal);
void CoreReg_LpmWriteCtrl2(uint32_t writeVal);
void CoreReg_LpmWriteGpioLowWake(uint32_t writeVal);
void CoreReg_LpmWriteGpioHighWake(uint32_t writeVal);
void CoreReg_LpmWriteCounter(uint32_t writeVal);
void CoreReg_LpmWriteBuckCfg(uint32_t writeVal);
void CoreReg_LpmWriteChargeCtrl(uint32_t writeVal);
void CoreReg_LpmWriteXtal32kCtrl(uint32_t writeVal);

void CoreReg_LpmWrite2(uint32_t writeVal, uint8_t writeLpmVal);
void CoreReg_LpmWrite2EnterScanMode(uint32_t writeVal);
void CoreReg_LpmWrite2ChargeLowHighWakeup(uint32_t writeVal);
void CoreReg_LpmWrite2LdoControl(uint32_t writeVal);
void CoreReg_LpmWrite2Gpio2TouchWake(uint32_t writeVal);
void CoreReg_LpmWrite2IceWdtLongRst(uint32_t writeVal);
uint32_t CoreReg_LpmGetIceWdtLongRst(void);

void CoreReg_LpmAppRegRead(LPM_AppCtrlRegDef *lpmCtrl);
void CoreReg_LpmAppRegWrite(LPM_AppCtrlRegDef *lpmCtrl);
void CoreReg_LpmAppRegFirstPowerOnControl(bool isPowerOn);
void CoreReg_LpmAppRegChargePowerOffControl(bool isChargePowerOff);

void Audio_AdcDSMDIV_12MConfig(short init, int frac);
void Audio_AdcDSMDIV_12MEnable(short en, short dither_en, short div2);
void Audio_AdcDSMDIV_11MConfig(short init, int frac);
void Audio_AdcDSMDIV_11MEnable(short en, short dither_en, short div2);
void Audio_ClkInit(void);
void wait_lpodelay(uint32_t nus);
extern int lpo_delay(int us);
void SYS_RESET(void);

void __attribute__((noinline)) SetLockLpmWriteReg(void);
void __attribute__((noinline)) SetReleaseLpmWriteReg(void);

#ifdef __cplusplus
}
#endif

#endif
