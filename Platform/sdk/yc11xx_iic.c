/**************************************************************************
* @file:  yc11xx_iic.c
* @Date:  2021-6-4-20:14:05
* @brief:  I2C driver
*
* Change Logs:
* Date           Author      Version        Notes
* 2021-06-04     xxx         V1.0.0         the first version

**************************************************************************/
#include "yc11xx_iic.h"
#include "reg_addr.h"
#include "reg_struct.h"


/**
 * @brief	 IIC function and parameter configure
 * @param    type: IIC_0/IIC_1
 * @param    rate : IIC baud rate
 *				iic_rate_25k,
 *				iic_rate_50k,
 *				iic_rate_100k,
 *				iic_rate_200k,
 *				iic_rate_400k,
 *				iic_rate_600k,
 *				iic_rate_660k,
 *				iic_rate_860k,//max
 * @return  None
 */
void IIC_Init(IIC_TypeDef type, IIC_RateDev rate, IIC_Address iic_ram_select)
{
	_ASSERT(IS_IIC01(type));
	_ASSERT(IS_IICAdress(iic_ram_select));
    IIC_InitTypeDef iic_config;
    CoreReg_ClkControl(REG_CLOCK_OFF_I2C, ENABLE);
    
    switch(rate)
    {
    case iic_rate_25k:
        iic_config.scll = 236;
        iic_config.sclh = 236;
        break;
    case iic_rate_50k:
        iic_config.scll = 116;
        iic_config.sclh = 116;
        break;
    case iic_rate_100k:
        iic_config.scll = 56;
        iic_config.sclh = 56;
        break;
    case iic_rate_200k:
        iic_config.scll = 26;
        iic_config.sclh = 26;
        break;
    case iic_rate_400k:
        iic_config.scll = 11;
        iic_config.sclh = 11;
        break;
    case iic_rate_600k:
        iic_config.scll = 6;
        iic_config.sclh = 6;
        break;
    case iic_rate_660k:
        iic_config.scll = 5;
        iic_config.sclh = 5;
        break;
    case iic_rate_860k:
        iic_config.scll = 3;
        iic_config.sclh = 3;
        break;
    }

    iic_config.stsu = (iic_config.sclh) / 2;
    iic_config.sthd = (iic_config.sclh) / 2;
    iic_config.sosu = (iic_config.sclh) / 2;
    iic_config.dtsu = iic_config.sclh;
    iic_config.dthd = 0;
    IIC_ParamConfig(type, &iic_config, iic_ram_select);
}

/**
 * @brief  	configure IIC function
 * @param  	type: IIC_0/IIC_1
 * @param  	I2C_InitStruct :I2C timer patameter
 *			scll:	SCL Pulse Width Low.
 *			sclh:   SCL Pulse Width High.
 *			stsu: 	the Start Setup Time.
 *			sthd: 	the Start Hold Time.
 *			sosu:	the Stop Setup Time.
 *	 		dtsu: 	the Data Setup Time.
 *	 		dthd:	the Data Hold Time.
 * @return 	None
 */
void IIC_ParamConfig(IIC_TypeDef type, IIC_InitTypeDef *iic_init_struct, IIC_Address iic_ram_select)
{
	_ASSERT(IS_IIC01(type));
    IICD_CTRLRegDef	iic_ctrl;
    IIC_DeInit(type);
    IIC_RegDef *iic_addr = (IIC_RegDef *)(reg_map(CORE_IICD_CTRL_BASE(type)));

    HREAD_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
    iic_ctrl.iicd_auto_inc = ENABLE;
    iic_ctrl.iicd_restart = ENABLE;
    iic_ctrl.iicd_mram_sel = iic_ram_select;
    HWRITE_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);

    /*set iic clock speed*/
    iic_addr->sclsdaclk.scll = iic_init_struct->scll;
    iic_addr->sclsdaclk.sclh = iic_init_struct->sclh;
    iic_addr->sclsdaclk.stsu = iic_init_struct->stsu;
    iic_addr->sclsdaclk.sthd = iic_init_struct->sthd;
    iic_addr->sclsdaclk.sosu = iic_init_struct->sosu;
    iic_addr->sclsdaclk.dtsu = iic_init_struct->dtsu;
    iic_addr->sclsdaclk.dthd = iic_init_struct->dthd;
}

/**
 * @brief    clear IIC Reg configure
 * @param    type: IIC_0/IIC_1
 * @return  None
 */
void IIC_DeInit(IIC_TypeDef type)
{
	_ASSERT(IS_IIC01(type));
    HWRITE(CORE_IICD_CTRL_BASE(type), 0);
}

/**
 * @brief	 IIC send Dma buffer data
 * @param    type: IIC_0/IIC_1
 * @param    src :send buffer addr
 * @param    len : send data length
 * @return	  None
 */
void IIC_SendData(IIC_TypeDef type, uint8_t *src, uint16_t len)
{
	_ASSERT(IS_IIC01(type));
    DMA_STARTRegDef dma_start;
    IIC_RegDef *iic_addr = (IIC_RegDef *)(reg_map(CORE_IICD_CTRL));

    HWRITEW(CORE_IICD_TXADDR, (int)src);
    HWRITEW(CORE_IICD_TXLEN, len);

    HWRITEW((uint32_t )&iic_addr->TxLen, len);
    HWRITEW((uint32_t)&iic_addr->RxLen, 0);
    HREAD_STRUCT(CORE_DMA_START, &dma_start);
    if(type == IIC_0) {
        dma_start.iic_dma_start = ENABLE;
    }
    else {
        dma_start.iic1_dma_start = ENABLE;
    }
    HWRITE_STRUCT(CORE_DMA_START, &dma_start);
    while(!(HREAD(CORE_DMA_STATUS) & (iic_check_done(type))));
}

/**
 * @brief  	 IIC receive data by dma
 * @param    type: IIC_0/IIC_1
 * @param    src : send data buffer
 * @param    src_len : send data length
 * @param    dest : read buffer addr
 * @param    dest_len :read data length
 * @return	 None
 */
void IIC_ReceiveData(IIC_TypeDef type, uint8_t *src, uint16_t src_len, uint8_t *dest, uint16_t dest_len)
{
	_ASSERT(IS_IIC01(type));
    DMA_STARTRegDef dma_start;
    IIC_RegDef *iic_addr = NULL;
    HWRITEW(CORE_IICD_TXADDR_BASE(type), (int)src);
    HWRITEW(CORE_IICD_TXLEN_BASE(type), src_len);
    HWRITEW(CORE_IICD_RXADDR_BASE(type), (int) dest);
    HWRITEW(CORE_IICD_RXLEN_BASE(type), dest_len);
    iic_addr = (IIC_RegDef *)(reg_map(CORE_IICD_CTRL_BASE(type)));

    HWRITEW((uint32_t)&iic_addr->TxLen, src_len);
    HWRITEW((uint32_t )&iic_addr->RxLen, dest_len);
    HREAD_STRUCT(CORE_DMA_START, &dma_start);
    if(type == IIC_0)
    {
        dma_start.iic_dma_start = ENABLE;
    }
    else
    {
        dma_start.iic1_dma_start = ENABLE;
    }
    HWRITE_STRUCT(CORE_DMA_START, &dma_start);
    while(!(HREAD(CORE_DMA_STATUS) & (iic_check_done(type))));
}

/**
 * @brief  	 Enable IIC interrupt
 * @param    type: IIC_0/IIC_1
 * @param    irqnum: Interrupt priority:1~15
 * @return    None
 */
void IIC_IRQ_Enable(IIC_TypeDef type)
{
	_ASSERT(IS_IIC01(type));
    IICD_CTRLRegDef	iic_ctrl;
	HREAD_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
	iic_ctrl.iicd_intr_en = ENABLE;
	HWRITE_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
}

/**
 * @brief  Disable IIC interrupt
 * @param    type:IIC_0/IIC_1
 * @return  None
 */
void IIC_IRQ_Disable(IIC_TypeDef type)
{
	_ASSERT(IS_IIC01(type));
    IICD_CTRLRegDef	iic_ctrl;
	HREAD_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
	iic_ctrl.iicd_intr_en = DISABLE;
	HWRITE_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
}

/**
 * @brief  Clear Interrupt trigger status
 * @param    type:IIC_0/IIC_1
 * @return  None
 */
void IIC_IRQStatusClear(IIC_TypeDef type)
{
	_ASSERT(IS_IIC01(type));
         IICD_CTRLRegDef	iic_ctrl;
	HREAD_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
	iic_ctrl.iicd_intr_clr = ENABLE;
	HWRITE_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
	iic_ctrl.iicd_intr_clr = DISABLE;
	HWRITE_STRUCT(CORE_IICD_CTRL_BASE(type), &iic_ctrl);
}





