/*
 * Copyright (c) 2006-2021, YICHIP Development Team
 * @file     yc_startup_yc1121G.s
 * @brief    source file for setting startup_yc3122
 *
 * Change Logs:
 * Date           Author             Version        Notes
 * 2020-11-06     wushengyan         V1.0.0         the first version
 * 2021-03-03     zhangming          V1.1.0					the 1121G first version
 */

#define REGBYTES (4)
/* Enable interrupts when returning from the handler */
#define MSTATUS_PRV1 0x1880
#define MSTATUS_MIE  0x00000008
#define MSTATUS_FS   0x00006000
#define MEM_WAKE_FLAG   0x100044d2

		.text
		.globl flash_start
		.globl delay
		.section .text.startup
		la sp, _stack

        /* set exception and irq mtvec */
		la a0,trap
		ori a0,a0,1
        csrw mtvec,a0

        /* enable fs */
		li t0,MSTATUS_FS
		csrs mstatus,t0
		csrw fcsr,x0

		/*judgement init*/
		li t0, MEM_WAKE_FLAG /*reg*/
		li t1, 1  /*data*/
		lb t0,(t0)
		beq t0,t1,__EN_MIE

        /* Load data section */
        la a0, _sidata
        la a1, _sdata
        la a2, _edata
        bgeu a1,a2,2f
1:      
        lw t0, (a0)
        sw t0, (a1)
        addi a0,a0,4
        addi a1,a1,4
        bltu a1,a2,1b
2: 
        /* clear bss section */
        la a0,_sbss
        la a1,_ebss
        bgeu a0,a1,2f
1:
        sw zero,(a0)
        addi a0,a0,4
        bltu a0,a1,1b
2:

#ifndef __NO_SYSTEM_INIT
//        jal systeminit
#endif

#ifndef __NO_BOARD_INIT
//        jal board_init
#endif
__EN_MIE:
		li t0, 0x00000800
		csrs 0x304,t0

		li t0, MSTATUS_MIE
		csrs mstatus, t0

		jal main

        /* never retch here*/
__exit:
        j __exit






.macro DISABLE_MIE
    csrc mstatus, MSTATUS_MIE
.endm

.macro ENABLE_MIE
    csrs mstatus, MSTATUS_MIE
.endm

disable_mie:
	.global disable_mie
    csrc mstatus, MSTATUS_MIE
	mret

enable_mie:
	.global enable_mie
    csrs mstatus, MSTATUS_MIE
	mret

.macro GET_IRQ_NUM
    li t1,0x0001fc04
    lw t1,0(t1)
    li t3,0x01
    li t5,0x00
get_irq_num_loop:
    and t4, t1, t3
    blt  x0, t4, get_irq_num_end
    addi t5, t5, 1
    slli t3, t3, 1
    j get_irq_num_loop
get_irq_num_end:
    mv   t1,  t5
.endm



.macro SAVE_CONTEXT
    addi sp,sp,-20*4
    sw x1 , 0 *REGBYTES(sp) /* ra */
    sw x4 , 1 *REGBYTES(sp) /* tp */
    sw x5 , 2 *REGBYTES(sp) /* t0 */
    sw x6 , 3 *REGBYTES(sp) /* t1 */
    sw x7 , 4 *REGBYTES(sp) /* t2 */
    sw x10, 5 *REGBYTES(sp) /* a0 */
    sw x11, 6 *REGBYTES(sp) /* a1 */
    sw x12, 7 *REGBYTES(sp) /* a2 */
    sw x13, 8 *REGBYTES(sp) /* a3 */
    sw x14, 9 *REGBYTES(sp) /* a4 */
    sw x15, 10*REGBYTES(sp) /* a5 */
    sw x16, 11*REGBYTES(sp) /* a6 */
    sw x17, 12*REGBYTES(sp) /* a7 */
    sw x28, 13*REGBYTES(sp) /* t3 */
    sw x29, 14*REGBYTES(sp) /* t4 */
    sw x30, 15*REGBYTES(sp) /* t5 */
    sw x31, 16*REGBYTES(sp) /* t6 */
.endm

.macro RESTORE_CONTEXT
    lw x1 , 0 *REGBYTES(sp) /* ra */
    lw x4 , 1 *REGBYTES(sp) /* tp */
    lw x5 , 2 *REGBYTES(sp) /* t0 */
    lw x6 , 3 *REGBYTES(sp) /* t1 */
    lw x7 , 4 *REGBYTES(sp) /* t2 */
    lw x10, 5 *REGBYTES(sp) /* a0 */
    lw x11, 6 *REGBYTES(sp) /* a1 */
    lw x12, 7 *REGBYTES(sp) /* a2 */
    lw x13, 8 *REGBYTES(sp) /* a3 */
    lw x14, 9 *REGBYTES(sp) /* a4 */
    lw x15, 10*REGBYTES(sp) /* a5 */
    lw x16, 11*REGBYTES(sp) /* a6 */
    lw x17, 12*REGBYTES(sp) /* a7 */
    lw x28, 13*REGBYTES(sp) /* t3 */
    lw x29, 14*REGBYTES(sp) /* t4 */
    lw x30, 15*REGBYTES(sp) /* t5 */
    lw x31, 16*REGBYTES(sp) /* t6 */
    addi sp, sp, 20*REGBYTES
.endm

.macro SAVE_CSR_CONTEXT
    csrr t0,mepc
    csrr t1,mcause
    sw   t0,17*REGBYTES(sp) /* mepc */
    sw   t1,18*REGBYTES(sp) /* mcause */
.endm

.macro RESTORE_CSR_CONTEXT
    lw   t0,17*REGBYTES(sp) /* mepc */
    lw   t1,18*REGBYTES(sp) /* mcause */
    csrw mcause, t1
    csrw mepc,   t0
.endm



    .align 2
    .global Default_IRQHandler
    .weak   Default_IRQHandler
    .type   Default_IRQHandler, %function
Default_IRQHandler:

    SAVE_CONTEXT

    SAVE_CSR_CONTEXT

    /* get irq */
    la t0,isr_table
//    GET_IRQ_NUM    		/* t1: irq num */
    li t1,0x0001fc04		/* t1: irq num */
    lw t1,0(t1)
    slli t2, t1, 2
    add t0, t0, t2
    lw t2, (t0)
	sw t1,19*REGBYTES(sp)


//	ENABLE_MIE

    jalr t2         /* jump to irq */

	DISABLE_MIE

	/* clear pending mask*/
	lw t1,19*REGBYTES(sp)
	li t0,0x0001fc04
	sw t1,(t0)


	/* enable pri mie*/
    li      t0, MSTATUS_PRV1
    csrs    mstatus, t0

    RESTORE_CSR_CONTEXT

    RESTORE_CONTEXT

    mret


/* trap start*/
.section .text.trap
/* In CLIC mode, the exeception entry must be 64bytes aligned */
.align 6
.global trap
.weak trap
.type trap, %function
trap:
    /* check for interrupt */
    addi sp,sp,-4
    sw   t0,0x0(sp)
    csrr t0,mcause
    blt  t0,x0, .Interrupt /* go to Interrupt*/
    addi sp,sp,4

    /* save regs */
    addi sp,sp,-23*4
    sw   x1 , 0 *REGBYTES(sp)
    sw   x2 , 1 *REGBYTES(sp)
    sw   x3 , 2 *REGBYTES(sp)
    sw   x4 , 3 *REGBYTES(sp)
    sw   x5 , 4 *REGBYTES(sp)
    sw   x6 , 5 *REGBYTES(sp)
    sw   x7 , 6 *REGBYTES(sp)
    sw   x8 , 7 *REGBYTES(sp)
    sw   x9 , 8 *REGBYTES(sp)
    sw   x10, 9 *REGBYTES(sp)
    sw   x11, 10*REGBYTES(sp)
    sw   x12, 11*REGBYTES(sp)
    sw   x13, 12*REGBYTES(sp)
    sw   x14, 13*REGBYTES(sp)
    sw   x15, 14*REGBYTES(sp)
    sw   x16, 15*REGBYTES(sp)
    sw   x17, 16*REGBYTES(sp)
    sw   x28, 17*REGBYTES(sp)
    sw   x29, 18*REGBYTES(sp)
    sw   x30, 19*REGBYTES(sp)
    sw   x31, 20*REGBYTES(sp)
    
    csrr a0,  mepc
    sw   a0,  21*REGBYTES(sp)
    csrr a0,  mstatus
    sw   a0,  22*REGBYTES(sp)
    mv   a0,  sp


    jal trap_c

    /*never reatch here */
    j .

.Interrupt:
    lw    t0, 0x0(sp)
    addi  sp, sp, 4

    j    Default_IRQHandler
/* trap end*/

        .globl lpo_delay
lpo_delay:
		/* This function has a high time requirement. */
		/* ROM uses 8 bytes as a line. It is necessary to avoid spanning 8 bytes. */
        add a0,a0,-1
        bnez a0,lpo_delay
        ret

	.global trap_c
	.weak trap_c
	.type trap_c,%function
trap_c:
	j trap_c


    .align  6
    .weak   Default_Handler
    .global Default_Handler
    .type   Default_Handler, %function
Default_Handler:
    j       Default_Handler
    .size   Default_Handler, . - Default_Handler

/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro  def_irq_handler handler_name
    .weak   \handler_name
    .globl  \handler_name
    .set    \handler_name, Default_Handler
    .endm
	def_irq_handler	    NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler	
	def_irq_handler		NMI_Handler	
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler	
	def_irq_handler		NMI_Handler	
	def_irq_handler		NMI_Handler	
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
    def_irq_handler     USB_IRQHandler      
	def_irq_handler     IIC_IRQHandler      
	def_irq_handler     QSPI_IRQHandler     
	def_irq_handler     SPI_IRQHandler      
	def_irq_handler     UART_IRQHandler     
	def_irq_handler     UARTB_IRQHandler    
    def_irq_handler     ADC_IRQHandler      
	def_irq_handler     IIS_IRQHandler      
	def_irq_handler     BT_IRQHandler       
    def_irq_handler     HOST_IRQHandler
    def_irq_handler     GPIO_IRQHandler
    def_irq_handler     SBC_ERR0_IRQHandl 
    def_irq_handler     SBC_ERR1_IRQHandl 
    def_irq_handler     FFT_IRQHandler		
    def_irq_handler     DMA_IRQHandler		
    def_irq_handler     SD_IRQHandler			
    def_irq_handler     PWM0_IRQHandler
    def_irq_handler     PWM1_IRQHandler
    def_irq_handler     PWM2_IRQHandler
    def_irq_handler     PWM3_IRQHandler
    def_irq_handler     PWM4_IRQHandler
    def_irq_handler     PWM5_IRQHandler
    def_irq_handler     PWM6_IRQHandler
    def_irq_handler     PWM7_IRQHandler
    def_irq_handler     WDT2_IRQHandler		
    def_irq_handler     WDT_IRQHandler		
    def_irq_handler     DEB_IRQHandler
    def_irq_handler     IICD1_IRQHandler
    def_irq_handler	  	SDMA_IRQHandler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler		NMI_Handler
	def_irq_handler     RV_TM_IRQHandler
	def_irq_handler     QSPI1_IRQHandler
	def_irq_handler     QSPI2_IRQHandler
   

    .align 4
isr_table:
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
	.long		0
    .long 	    USB_IRQHandler      
	.long       IIC_IRQHandler
	.long       QSPI_IRQHandler
	.long       SPI_IRQHandler
	.long       UART_IRQHandler
	.long       UARTB_IRQHandler
    .long       ADC_IRQHandler
	.long       IIS_IRQHandler
	.long       BT_IRQHandler
	.long       HOST_IRQHandler
	.long       GPIO_IRQHandler
	.long       SBC_ERR0_IRQHandl
	.long       SBC_ERR1_IRQHandl
	.long       FFT_IRQHandler
	.long       DMA_IRQHandler
	.long       SD_IRQHandler
	.long       PWM0_IRQHandler
	.long       PWM1_IRQHandler
	.long       PWM2_IRQHandler
	.long       PWM3_IRQHandler
	.long       PWM4_IRQHandler
	.long       PWM5_IRQHandler
	.long       PWM6_IRQHandler
	.long       PWM7_IRQHandler
	.long       WDT2_IRQHandler
	.long       WDT_IRQHandler
	.long       DEB_IRQHandler
	.long       IICD1_IRQHandler
	.long		SDMA_IRQHandler
	.long		0
	.long		0
	.long		0
	.long       RV_TM_IRQHandler
	.long       QSPI1_IRQHandler
	.long       QSPI2_IRQHandler
