#include <string.h>
#include <irqn.h>
#include "HardwareI2cSlave.h"
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 * @brief I2c communication mode enum
 */
typedef enum {
    MD_TX = 0U,
    MD_RX = 1U,
} stc_i2c_com_mode_t;

/**
 * @brief I2c communication status enum
 */
typedef enum {
    I2C_COM_BUSY = 0U,
    I2C_COM_IDLE = 1U,
} stc_i2c_com_status_t;

/**
 * @brief I2c communication structure
 */
typedef struct {
    stc_i2c_com_mode_t    enMode;         /*!< I2C communication mode*/
    uint32_t              u32Len;         /*!< I2C communication data length*/
    uint8_t              *pBuf;           /*!< I2C communication data buffer pointer*/
    __IO uint32_t         u32DataIndex;   /*!< I2C communication data transfer index*/
    __IO stc_i2c_com_status_t  enComStatus;    /*!< I2C communication status*/
} stc_i2c_communication_t;

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/* unlock/lock peripheral */
#define EXAMPLE_PERIPH_WE               (LL_PERIPH_GPIO | LL_PERIPH_EFM | LL_PERIPH_FCG | \
                                         LL_PERIPH_PWC_CLK_RMU | LL_PERIPH_SRAM)
#define EXAMPLE_PERIPH_WP               (LL_PERIPH_EFM | LL_PERIPH_FCG | LL_PERIPH_SRAM)

/* Define slave device address for example */
#define DEVICE_ADDR                     (0x11U)
/* I2C address mode */
#define I2C_ADDR_MD_7BIT                (0U)
#define I2C_ADDR_MD_10BIT               (1U)
/* Config I2C address mode: I2C_ADDR_MD_7BIT or I2C_ADDR_MD_10BIT */
#define I2C_ADDR_MD                     (I2C_ADDR_MD_7BIT)

/* Note: The polarity of EEI interrupt should be higher than other I2C interrupt */
#define I2C_EEI_IRQN_DEF                (INT001_IRQn)
#define I2C_RXI_IRQN_DEF                (INT002_IRQn)
#define I2C_TXI_IRQN_DEF                (INT003_IRQn)
#define I2C_TEI_IRQN_DEF                (INT004_IRQn)

#define I2C_INT_EEI_DEF                 (INT_SRC_I2C2_EEI)
#define I2C_INT_RXI_DEF                 (INT_SRC_I2C2_RXI)
#define I2C_INT_TXI_DEF                 (INT_SRC_I2C2_TXI)
#define I2C_INT_TEI_DEF                 (INT_SRC_I2C2_TEI)

#define TIMEOUT                         (0x40000UL)

/* Define Write and read data length for the example */
#define TEST_DATA_LEN                   (256U)
/* Define i2c baudrate */
#define I2C_BAUDRATE                    (400000UL)

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/
static uint8_t u8RxBuf[TEST_DATA_LEN];
static stc_i2c_communication_t stcI2cCom;

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  Slave receive data
 *
 * @param  pu8RxData             Pointer to the data buffer
 * @param  u32Size               Data size
 * @retval int32_t:
 *            - LL_OK:           Success
 *            - LL_ERR_BUSY:     Busy
 */
static int32_t I2C_Slave_Receive_IT(uint8_t *pu8RxData, uint32_t u32Size)
{
    int32_t i32Ret = LL_OK;

    if (I2C_COM_IDLE == stcI2cCom.enComStatus) {
        stcI2cCom.enComStatus = I2C_COM_BUSY;

        stcI2cCom.u32DataIndex = 0U;
        stcI2cCom.enMode = MD_RX;
        stcI2cCom.u32Len = u32Size;
        stcI2cCom.pBuf = pu8RxData;

        I2C_Cmd(I2C_UNIT, ENABLE);
        /* Config slave address match and receive full interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_MATCH_ADDR0 | I2C_INT_RX_FULL, ENABLE);
    } else {
        i32Ret = LL_ERR_BUSY;
    }

    return i32Ret;
}

/**
 * @brief  Slave transmit data
 *
 * @param  pu8TxData             Pointer to the data buffer
 * @param  u32Size               Data size
 * @retval int32_t:
 *            - LL_OK:           Success
 *            - LL_ERR_BUSY:     Busy
 */
static int32_t I2C_Slave_Transmit_IT(uint8_t *pu8TxData, uint32_t u32Size)
{
    int32_t i32Ret = LL_OK;

    if (I2C_COM_IDLE == stcI2cCom.enComStatus) {
        stcI2cCom.enComStatus = I2C_COM_BUSY;

        stcI2cCom.u32DataIndex = 0U;
        stcI2cCom.enMode = MD_TX;
        stcI2cCom.u32Len = u32Size;
        stcI2cCom.pBuf = pu8TxData;

        I2C_Cmd(I2C_UNIT, ENABLE);
        /* Config slave address match interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_MATCH_ADDR0, ENABLE);
    } else {
        i32Ret = LL_ERR_BUSY;
    }

    return i32Ret;
}

/**
 * @brief   static function for buffer write.
 * @param   [in] u8Data         the data to be write.
 * @retval  None
 */
static void BufWrite(uint8_t u8Data)
{
    if (stcI2cCom.u32DataIndex < stcI2cCom.u32Len) {
        u8RxBuf[stcI2cCom.u32DataIndex] = u8Data;
        stcI2cCom.u32DataIndex++;
    }
}

/**
 * @brief   Static function for buffer read.
 * @param   None
 * @retval  uint8_t             The data read out from buffer.
 */
static uint8_t BufRead(void)
{
    uint8_t temp;
    if (stcI2cCom.u32DataIndex < stcI2cCom.u32Len) {
        temp = u8RxBuf[stcI2cCom.u32DataIndex];
        stcI2cCom.u32DataIndex++;
    } else {
        temp = 0xFFU;
    }

    return temp;
}

/**
 * @brief   I2C EEI(communication error or event) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_EEI_Callback(void)
{
    /* If address interrupt occurred */
    if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_MATCH_ADDR0)) {
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_SLADDR0FCLR | I2C_CLR_NACKFCLR | I2C_CLR_STOPFCLR);

        if ((SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TRA))) {
            /* Enable tx end interrupt function*/
            I2C_IntCmd(I2C_UNIT, I2C_INT_TX_CPLT, ENABLE);
            /* Write the first data to DTR immediately */
            I2C_WriteData(I2C_UNIT, BufRead());

            /* Enable stop and NACK interrupt */
            I2C_IntCmd(I2C_UNIT, I2C_INT_STOP | I2C_INT_NACK, ENABLE);
        } else if ((RESET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TRA))) {
            /* Enable stop and NACK interrupt */
            I2C_IntCmd(I2C_UNIT, I2C_INT_STOP | I2C_INT_NACK, ENABLE);
        } else {
        }
    } else if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_NACKF)) {
        /* If NACK interrupt occurred */
        /* clear NACK flag*/
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_NACKFCLR);
        /* Stop tx or rx process*/
        if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TRA)) {
            /* Config tx end interrupt function disable*/
            I2C_IntCmd(I2C_UNIT, I2C_INT_TX_CPLT, DISABLE);
            I2C_ClearStatus(I2C_UNIT, I2C_CLR_TENDFCLR);

            /* Read DRR register to release */
            (void)I2C_ReadData(I2C_UNIT);
        } else {
            /* Config rx buffer full interrupt function disable */
            I2C_IntCmd(I2C_UNIT, I2C_INT_RX_FULL, DISABLE);
        }
    } else if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_STOP)) {
        /* If stop interrupt occurred */
        /* Disable all interrupt enable flag except SLADDR0IE*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_TX_CPLT | I2C_INT_RX_FULL | I2C_INT_STOP | I2C_INT_NACK, DISABLE);
        /* Clear STOPF flag */
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_STOPFCLR);
        I2C_Cmd(I2C_UNIT, DISABLE);
        /* Communication finished */
        stcI2cCom.enComStatus = I2C_COM_IDLE;
			  I2C_Cmd(I2C_UNIT, ENABLE);
        /* Config slave address match and receive full interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_MATCH_ADDR0 | I2C_INT_RX_FULL, ENABLE);
    } else {
    }
}


/**
 * @brief   I2C TEI(transfer end) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_TEI_Callback(void)
{
    if ((SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TX_CPLT)) &&
        (RESET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_NACKF))) {
        I2C_WriteData(I2C_UNIT, BufRead());
    }
}

/**
 * @brief   I2C RXI(receive buffer full) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_RXI_Callback(void)
{
    if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_RX_FULL)) {
        BufWrite(I2C_ReadData(I2C_UNIT));
    }
}

inline void i2c_irq_register(stc_irq_signin_config_t &irq, const char *name, uint32_t priority = DDL_IRQ_PRIO_05)
{
    // get auto-assigned irqn and set in irq struct
    IRQn_Type irqn;
    irqn_aa_get(irqn, name);
    irq.enIRQn = irqn;

    // create irq registration struct
    stc_irq_signin_config_t irqConf = {
        .enIntSrc    = irq.enIntSrc,
        .enIRQn      = irq.enIRQn,
        .pfnCallback = irq.pfnCallback,
    };

    // register and enable irq
    INTC_IrqSignIn(&irqConf);
    NVIC_SetPriority(irqConf.enIRQn, priority);
    NVIC_ClearPendingIRQ(irqConf.enIRQn);
    NVIC_EnableIRQ(irqConf.enIRQn);
}

inline void i2c_irq_resign(stc_irq_signin_config_t &irq, const char *name)
{
    // disable interrupt and clear pending
    NVIC_DisableIRQ(irq.enIRQn);
    NVIC_ClearPendingIRQ(irq.enIRQn);
    INTC_IrqSignOut(irq.enIRQn);

    // resign auto-assigned irqn
    irqn_aa_resign(irq.enIRQn, name);
}

/**
 * @brief   Initialize the I2C peripheral for slave
 * @param   None
 * @retval int32_t:
 *            - LL_OK:              Success
 *            - LL_ERR_INVD_PARAM:  Invalid parameter
 */
int32_t Slave_Initialize(void)
{
			/* Enable I2C Peripheral*/
    FCG_Fcg1PeriphClockCmd(I2C_FCG_USE, ENABLE);
    (void)memset(u8RxBuf, (int32_t)0x01U, 128);

    /* Initialize I2C port*/
    GPIO_SetFunc(I2C_SCL_PORT, I2C_SCL_PIN, I2C_GPIO_SCL_FUNC);
    GPIO_SetFunc(I2C_SDA_PORT, I2C_SDA_PIN, I2C_GPIO_SDA_FUNC);
	
    int32_t i32Ret;
    stc_i2c_init_t stcI2cInit;
    stc_irq_signin_config_t stcIrqRegCfg;
    float32_t fErr;

    (void)I2C_DeInit(I2C_UNIT);

    stcI2cCom.enComStatus = I2C_COM_IDLE;

    (void)I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32ClockDiv = I2C_CLK_DIV2;
    stcI2cInit.u32Baudrate = I2C_BAUDRATE;
    stcI2cInit.u32SclTime = 5U;
    i32Ret = I2C_Init(I2C_UNIT, &stcI2cInit, &fErr);

    if (LL_OK == i32Ret) {
        /* Set slave address*/
#if (I2C_ADDR_MD == I2C_ADDR_MD_10BIT)
        I2C_SlaveAddrConfig(I2C_UNIT, I2C_ADDR0, I2C_ADDR_10BIT, DEVICE_ADDR);
#else
        I2C_SlaveAddrConfig(I2C_UNIT, I2C_ADDR0, I2C_ADDR_7BIT, DEVICE_ADDR);
#endif
        stcIrqRegCfg.enIntSrc = I2C_INT_EEI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_EEI_Callback;
        i2c_irq_register(stcIrqRegCfg, "I2C_EEI_IRQN_DEF");
        // stcIrqRegCfg.enIRQn = I2C_EEI_IRQN_DEF;
        // stcIrqRegCfg.enIntSrc = I2C_INT_EEI_DEF;
        // stcIrqRegCfg.pfnCallback = &I2C_EEI_Callback;
        // (void)INTC_IrqSignIn(&stcIrqRegCfg);
        // NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        // NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        // NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

        stcIrqRegCfg.enIntSrc = I2C_INT_RXI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_RXI_Callback;
        i2c_irq_register(stcIrqRegCfg, "I2C_RXI_IRQN_DEF");
        // stcIrqRegCfg.enIRQn = I2C_RXI_IRQN_DEF;
        // stcIrqRegCfg.enIntSrc = I2C_INT_RXI_DEF;
        // stcIrqRegCfg.pfnCallback = &I2C_RXI_Callback;
        // (void)INTC_IrqSignIn(&stcIrqRegCfg);
        // NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        // NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        // NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

        stcIrqRegCfg.enIntSrc = I2C_INT_TEI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_TEI_Callback;
        i2c_irq_register(stcIrqRegCfg, "I2C_TEI_IRQN_DEF");
        // stcIrqRegCfg.enIRQn = I2C_TEI_IRQN_DEF;
        // stcIrqRegCfg.enIntSrc = I2C_INT_TEI_DEF;
        // stcIrqRegCfg.pfnCallback = &I2C_TEI_Callback;
        // (void)INTC_IrqSignIn(&stcIrqRegCfg);
        // NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        // NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        // NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);
				
		I2C_Cmd(I2C_UNIT, ENABLE);
        /* Config slave address match and receive full interrupt function*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_MATCH_ADDR0 | I2C_INT_RX_FULL, ENABLE);
    }
    return i32Ret;
}