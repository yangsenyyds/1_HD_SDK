/**
 * @file yc11xx_qspi.c
 * @author duanchu.he
 * @brief QSPI driver
 * @version 0.1
 * @date 2021-07-06
 *
 *
 */
#include "yc11xx_qspi.h"
#include "reg_addr.h"
#include "reg_struct.h"

/**
 * @brief Configure qspi addr function
 *
 * @param qspix QSPI1 or QSPI2
 * @param qspi_handle 	qspi config structure
 */
void QSPI_AddrConfig(QSPI_NUM qspix, QSPI_AddrTypeDef *qspi_handle)
{
    QSPI_TX_CTRL_ADDRRegDef		qspitxctrl_handle;
    QSPI_RX_CTRL_ADDRRegDef		qspirxctrl_handle;
    QSPI_CTRL_ADDRRegDef		qspictrl_handle;
    QSPI_CMD_CTRL_ADDRRegDef	qspicmdctrl_handle;

	qspitxctrl_handle.qspi_txaddr = qspi_handle->txaddr;
	qspitxctrl_handle.qspi_txlen = qspi_handle->txlen;
	HWRITEL_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_TX_CTRL, qspix), &qspitxctrl_handle);

	qspirxctrl_handle.qspi_rxaddr = qspi_handle->rxaddr;
	qspirxctrl_handle.qspi_rxlen = qspi_handle->rxlen;
	HWRITEL_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_RX_CTRL, qspix), &qspirxctrl_handle);

	qspicmdctrl_handle.qspi_cmdaddr = qspi_handle->cmdaddr;
	qspicmdctrl_handle.qspi_cmdlen = qspi_handle->cmdlen;
	HWRITEL_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CMD_CTRL, qspix), &qspicmdctrl_handle);

	HREADW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
	qspictrl_handle.wslen = qspi_handle->wslen;
	HWRITEW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
}

/**
 * @brief Qspi set mode function
 *
 * @param qspix QSPI1 or QSPI2
 * @param val 0-stand 1-dual -quad
 */
void QSPI_SetMode (QSPI_NUM qspix, uint8_t val)
{
    QSPI_CTRL_ADDRRegDef	qspictrl_handle;

	HREADW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
	qspictrl_handle.mode_sel = val;
	HWRITEW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);

}

/**
 * @brief Qspi clear interrupt flag function
 *
 * @param qspix QSPI1 or QSPI2
 * @param state DISABLE or ENABLE
 */
void QSPI_SetInc (QSPI_NUM qspix, FunctionalState new_state)
{
    QSPI_CTRL_ADDRRegDef		qspictrl_handle;

	HREADW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
	qspictrl_handle.intr_clr = new_state;
	HWRITEW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
}

/**
 * @brief Qspi open clock function
 *
 * @param qspix QSPI1 or QSPI2
 * @param state DISABLE or ENABLE
 */
void QSPI_SetClk (QSPI_NUM qspix, FunctionalState new_state)
{
    QSPI_CTRL_ADDRRegDef	qspictrl_handle;

    CoreReg_ClkControl(REG_CLOCK_OFF_QSPI, ENABLE);

	HREADW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
	qspictrl_handle.clk_en = new_state;
	HWRITEW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
}

/**
 * @brief Qspi enable interrupt  function
 *
 * @param qspix QSPI1 or QSPI2
 * @param state DISABLE or ENABLE
 */
void QSPI_SetInterrupt(QSPI_NUM qspix, FunctionalState new_state)
{
    QSPI_CTRL_ADDRRegDef	qspictrl_handle;

	HREADW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);
	qspictrl_handle.intr_en = new_state;
	HWRITEW_STRUCT(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_CTRL, qspix), &qspictrl_handle);

}



/**
 * @brief Qspi start dma  function
 *
 * @param qspix QSPI1 or QSPI2
 * @param state DISABLE or ENABLE
 */
void QSPI_SetStart (QSPI_NUM qspix,  FunctionalState new_state)
{
	HWRITE(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_START, qspix), new_state);
}

/**
 * @brief
 *
 * @param qspix QSPI1 or QSPI2
 */
void QSPI_WaitDone (QSPI_NUM qspix)
{
    uint8_t tmp = 0;
    int timeout = 0;

    while (1)
    {
        timeout++;

         tmp = HREAD(CORE_QSPI_CONFIG_BASE(CORE_QSPI1_STATUS, qspix));

        tmp = tmp & 0x01;
        if ((tmp == 0x01) || (timeout > QSPI_WAITTIMEOUT))
            break;
    }
}





