#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "slave_i2c.h"
#include "Arduino.h"
#include "SparkFun_Extensible_Message_Parser.h"
#include "message_decode.h"

volatile SLAVE_I2C_STATE slave_state = SLAVE_RX;

volatile uint16_t _txBufferHead = 0;
volatile uint16_t _txBufferTail = 0;
uint8_t _txBuffer[SLAVE_TX_BUFFER_SIZE];
volatile uint16_t _rxBufferHead = 0;
volatile uint16_t _rxBufferTail = 0;
uint8_t _rxBuffer[SLAVE_RX_BUFFER_SIZE];

uint8_t txBuffer_temp[NM_PROTOCOL_PINFO3_MSG_PACK_LEN];

SEMP_PARSE_STATE *CustomParse = nullptr;
/// @brief Bluetooth parser
SEMP_PARSE_ROUTINE const CustomParserTable[] = {
    sempCustomPreamble,
};
const int CustomParserCount = sizeof(CustomParserTable) / sizeof(CustomParserTable[0]);

// List the names of the parsers
const char *const CustomParserNames[] = {
    "CUSTOM",
};
const int CustomParserNameCount = sizeof(CustomParserNames) / sizeof(CustomParserNames[0]);


void CustomDataProcess(SEMP_PARSE_STATE *parse, uint16_t type)
{
    int length = message_decode(parse, txBuffer_temp);
    if (length <= 0) {
        return;
    }
    txBufferWrite(txBuffer_temp, length);
}

int txBufferAvailable()
{
    return _txBufferHead - _txBufferTail;
}

int txBufferRead(void)
{
    return _txBuffer[_txBufferTail++];
}

int txBufferWrite(uint8_t *Buffer, const uint16_t length)
{
    // if the head isn't ahead of the tail, we don't have any characters
    memcpy(_txBuffer, Buffer, length);
    _txBufferHead = length;
    _txBufferTail = 0;
    return _txBufferHead;
}

int rxBufferAvailable(void)
{
    return ((unsigned int)(SLAVE_RX_BUFFER_SIZE + _rxBufferHead - _rxBufferTail)) % SLAVE_RX_BUFFER_SIZE;
}

int rxBufferRead(void)
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rxBufferHead == _rxBufferTail) {
        return -1;
    } else {
        uint8_t c     = _rxBuffer[_rxBufferTail];
        _rxBufferTail = (uint16_t)(_rxBufferTail + 1) % SLAVE_RX_BUFFER_SIZE;
        return c;
    }
}

void rxBufferWrite(uint8_t ch)
{
    uint16_t i = (uint16_t)(_rxBufferHead + 1) % SLAVE_RX_BUFFER_SIZE;
    if (i != _rxBufferTail) {
        _rxBuffer[_rxBufferHead] = ch;
        _rxBufferHead            = i;
    }
}
/**
 * @brief   I2C EEI(communication error or event) interrupt callback function
 */
static void I2C_EEI_Callback(void)
{
    /* If address interrupt occurred */
    if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_MATCH_ADDR0)) {
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_SLADDR0FCLR | I2C_CLR_NACKFCLR | I2C_CLR_STOPFCLR);

        if ((SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TRA))) {
            slave_state = SLAVE_TX;
            /* Enable tx end interrupt function*/
            I2C_IntCmd(I2C_UNIT, I2C_INT_TX_CPLT, ENABLE);
            /* Write the first data to DTR immediately */
            if (txBufferAvailable() > 0) {
                I2C_WriteData(I2C_UNIT, txBufferRead());
            }
        } else {
            slave_state = SLAVE_RX;
            //						I2C_IntCmd(I2C_UNIT, I2C_INT_RX_FULL, ENABLE);
        }
        /* Enable stop and NACK interrupt */
        I2C_IntCmd(I2C_UNIT, I2C_INT_STOP | I2C_INT_NACK, ENABLE);
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
            //            I2C_IntCmd(I2C_UNIT, I2C_INT_RX_FULL, DISABLE);
        }
    } else if (SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_STOP)) {
        /* If stop interrupt occurred */
        /* Disable all interrupt enable flag except SLADDR0IE*/
        I2C_IntCmd(I2C_UNIT, I2C_INT_TX_CPLT | I2C_INT_STOP | I2C_INT_NACK, DISABLE);
        /* Clear STOPF flag */
        I2C_ClearStatus(I2C_UNIT, I2C_CLR_STOPFCLR);
        if (slave_state == SLAVE_RX) {
            printf("Rx Done, %d\n", millis());
            slave_state = SLAVE_RX_DONE;
        } else if (slave_state == SLAVE_TX) {
            printf("Tx Done, %d\n", millis());
            _txBufferHead = 0;
            slave_state   = SLAVE_TX_DONE;
        }
    } else {
    }
}

static void I2C_TEI_Callback(void)
{
    if ((SET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_TX_CPLT)) &&
        (RESET == I2C_GetStatus(I2C_UNIT, I2C_FLAG_NACKF))) {
        if (txBufferAvailable() > 0) {
            I2C_WriteData(I2C_UNIT, txBufferRead());
        }
    }
}

/**
 * @brief   I2C RXI(receive buffer full) interrupt callback function
 * @param   None
 * @retval  None
 */
static void I2C_RXI_Callback(void)
{
    rxBufferWrite(I2C_ReadData(I2C_UNIT));
}

int32_t slave_i2c_init()
{
    FCG_Fcg1PeriphClockCmd(I2C_FCG_USE, ENABLE);
    /* Initialize I2C port*/
    GPIO_SetFunc(I2C_SCL_PORT, I2C_SCL_PIN, I2C_GPIO_SCL_FUNC);
    GPIO_SetFunc(I2C_SDA_PORT, I2C_SDA_PIN, I2C_GPIO_SDA_FUNC);

    stc_i2c_init_t stcI2cInit;
    float32_t fErr;
    stc_irq_signin_config_t stcIrqRegCfg;

    (void)I2C_DeInit(I2C_UNIT);

    (void)I2C_StructInit(&stcI2cInit);
    stcI2cInit.u32ClockDiv = I2C_CLK_DIV2;
    stcI2cInit.u32Baudrate = 400000;
    stcI2cInit.u32SclTime  = 5UL;
    const int32_t i32Ret   = I2C_Init(I2C_UNIT, &stcI2cInit, &fErr);

    if (LL_OK == i32Ret) {
        I2C_SlaveAddrConfig(I2C_UNIT, I2C_ADDR0, I2C_ADDR_7BIT, DEVICE_ADDR);

        stcIrqRegCfg.enIRQn      = I2C_EEI_IRQN_DEF;
        stcIrqRegCfg.enIntSrc    = I2C_INT_EEI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_EEI_Callback;
        (void)INTC_IrqSignIn(&stcIrqRegCfg);
        NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_09);
        NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

        stcIrqRegCfg.enIRQn      = I2C_RXI_IRQN_DEF;
        stcIrqRegCfg.enIntSrc    = I2C_INT_RXI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_RXI_Callback;
        (void)INTC_IrqSignIn(&stcIrqRegCfg);
        NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

        stcIrqRegCfg.enIRQn      = I2C_TEI_IRQN_DEF;
        stcIrqRegCfg.enIntSrc    = I2C_INT_TEI_DEF;
        stcIrqRegCfg.pfnCallback = &I2C_TEI_Callback;
        (void)INTC_IrqSignIn(&stcIrqRegCfg);
        NVIC_ClearPendingIRQ(stcIrqRegCfg.enIRQn);
        NVIC_SetPriority(stcIrqRegCfg.enIRQn, DDL_IRQ_PRIO_DEFAULT);
        NVIC_EnableIRQ(stcIrqRegCfg.enIRQn);

        CustomParse = sempBeginParser(CustomParserTable, CustomParserCount,
                                      CustomParserNames, CustomParserNameCount,
                                      128, 128, CustomDataProcess, "BluetoothDebug");
        if (!CustomParse)
            printf("Failed to initialize the parser");
    }
    I2C_Cmd(I2C_UNIT, ENABLE);
    I2C_IntCmd(I2C_UNIT, I2C_INT_MATCH_ADDR0 | I2C_INT_RX_FULL, ENABLE);
    return i32Ret;
}

void slave_i2c_update()
{
    if (rxBufferAvailable() > 0 && slave_state == SLAVE_RX_DONE) {
        sempParseNextByte(CustomParse, rxBufferRead());
    }
}