#ifndef _YC11XX_IIC_H_
#define _YC11XX_IIC_H_
#include "yc11xx.h"


typedef enum {
    IIC_0 = 0,
    IIC_1 = 1,
} IIC_TypeDef;
#define IS_IIC01(IICx)         (IICx == IIC_0) || (IICx == IIC_1)

typedef enum {
    IIC_SRAM = 0,
    IIC_MRAM = 1,
} IIC_Address;
#define IS_IICAdress(xRAM)         (xRAM == IIC_SRAM) || (xRAM == IIC_MRAM)

typedef enum {
    iic_rate_25k,
    iic_rate_50k,
    iic_rate_100k,
    iic_rate_200k,
    iic_rate_400k,
    iic_rate_600k,
    iic_rate_660k,
    iic_rate_860k,  //max
} IIC_RateDev;

typedef struct {
    uint8_t scll;	/*!< Specifies the Clock Pulse Width Low. */
    uint8_t sclh;   /*!< Specifies the Clock Pulse Width High. */
    uint8_t stsu;   /*!< Specifies the Start Setup Time. */
    uint8_t sthd;   /*!< Specifies the Start Hold Time. */
    uint8_t sosu;   /*!< Specifies the Stop Setup Time. */
    uint8_t dtsu;   /*!< Specifies the Data Setup Time. */
    uint8_t dthd;   /*!< Specifies the Data Hold Time. */
} IIC_InitTypeDef;

typedef struct {
    uint8_t Ctrl;
    IIC_InitTypeDef sclsdaclk;
    uint16_t TxLen;
    uint16_t TxAddr;
    uint16_t RxAddr;
    uint16_t RxLen;
} IIC_RegDef;

#define reg_check_iicx(iicx) ((iicx) != IIC_0)
#define iic_check_done(x) (0x20 - x * 0x1e)

void IIC_Init(IIC_TypeDef type, IIC_RateDev rate, IIC_Address iic_ram_select);
void IIC_ParamConfig(IIC_TypeDef type, IIC_InitTypeDef *iic_init_struct, IIC_Address iic_ram_select);
void IIC_DeInit(IIC_TypeDef type);
void IIC_SendData(IIC_TypeDef type, uint8_t *Src, uint16_t len);
void IIC_ReceiveData(IIC_TypeDef type, uint8_t *src, uint16_t src_len, uint8_t *dest, uint16_t dest_len);

#endif /* _YC11XX_IIC_H_ */






