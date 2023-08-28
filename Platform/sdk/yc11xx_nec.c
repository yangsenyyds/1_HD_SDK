/**
 * @file yc11xx_nec.c
 * @brief NEC driver
 * @date 2021-08-07
 */
#include "yc11xx_nec.h"
#include "reg_addr.h"
#include "reg_struct.h"
/**
 * @Description: NEC init
 * @param {NEC_Config} config:Initial configuration
 *                      StartBuff : Configure NEC to learn the initial addresss
 *                      EndBuff   :Configure NEC learning end address
 *                      ReadPtr   :Configure NEC to learn the read pointer address
 *                      ClkDivNum :nec detect clk divide number, 7 effective data bits
 *                      DetectEn  :    NEC_Disable/NEC_enable
 * @return {*} None
 */
void NEC_RxLearnInit(NEC_Config config)
{
    NEC_CONTROLRegDef nec_control;
    HWRITEW(CORE_NEC_STARTADDR, config.StartBuff);
    HWRITEW(CORE_NEC_ENDADDR, config.EndBuff);
    HWRITEW(CORE_NEC_RPTR, config.StartBuff);
    HWRITEW(CORE_NEC_WPTR, config.StartBuff);
    
    HREAD_STRUCT(CORE_NEC_CONTROL, &nec_control);
    nec_control.nec_clkdiv_num = config.ClkDivNum;
    nec_control.nec_en = config.DetectEn;
    HWRITE_STRUCT(CORE_NEC_CONTROL, &nec_control);
}


/**
 * @Description: NEC_SimulationInit
 * @param {NEC_Config} config:Initial configuration
 *                      FunctionalState : Whether to use analog amplification
 *                      NEC_GainLevel   :Analog signal amplification level
 * @return {*} None
 */
void NEC_SimulationInit(FunctionalState state,NEC_GainLevel gain_level)
{
    	uint32_t nec_value;
    	SYSTEM_CTRLRegDef system_ctrl;
        HREAD_STRUCT(CORE_SYSTEM_CTRL, &system_ctrl);
        system_ctrl.nec_in_sel = state;
        HWRITE_STRUCT(CORE_SYSTEM_CTRL, &system_ctrl);
   	nec_value = (HREADL(CORE_BUCK_CTRL)|(((state << 1)|(gain_level<<2)) << 24));
   	CoreReg_LpmWriteBuckCfg(nec_value);
}

/**
 * @Description: nec buffer items
 * @param {*}None
 * @return {*} The size of the parsed data
 */
uint16_t NEC_RxBuffLen(void)
{
    return HREADW(CORE_NEC_ITEMS);
}

/**
 * @Description: Update NEC read pointer position
 * @param {uint16_t} *ptr : The position of the new read pointer
 * @return {*}None
 */
void NEC_SetReadPtr(uint16_t *ptr)
{
    HWRITEW(CORE_NEC_RPTR, ptr);
}
