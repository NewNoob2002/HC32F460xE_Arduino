#ifndef SLAVE_I2C_h
#define SLAVE_I2C_h

#include <stdint.h>

/* Define I2C unit used for the example */
#define I2C_UNIT             (CM_I2C1)
#define I2C_FCG_USE          (FCG1_PERIPH_I2C1)
/* Define slave device address for example */
#define DEVICE_ADDR          (0x11U)

#define SLAVE_TX_BUFFER_SIZE 256
#define SLAVE_RX_BUFFER_SIZE 512
/* Define port and pin for SDA and SCL */
#define I2C_SCL_PORT         (GPIO_PORT_A)
#define I2C_SCL_PIN          (GPIO_PIN_03)
#define I2C_SDA_PORT         (GPIO_PORT_A)
#define I2C_SDA_PIN          (GPIO_PIN_02)
#define I2C_GPIO_SCL_FUNC    (GPIO_FUNC_49)
#define I2C_GPIO_SDA_FUNC    (GPIO_FUNC_48)

#define I2C_EEI_IRQN_DEF     (INT005_IRQn)
#define I2C_RXI_IRQN_DEF     (INT006_IRQn)
#define I2C_TXI_IRQN_DEF     (INT003_IRQn)
#define I2C_TEI_IRQN_DEF     (INT004_IRQn)

#define I2C_INT_EEI_DEF      (INT_SRC_I2C1_EEI)
#define I2C_INT_RXI_DEF      (INT_SRC_I2C1_RXI)
#define I2C_INT_TXI_DEF      (INT_SRC_I2C1_TXI)
#define I2C_INT_TEI_DEF      (INT_SRC_I2C1_TEI)

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { SLAVE_RX = 0, SLAVE_RX_DONE, SLAVE_TX, SLAVE_TX_DONE } SLAVE_I2C_STATE;

int32_t slave_i2c_init();
void slave_i2c_update();

int txBufferAvailable();
int txBufferRead(void);
int txBufferWrite(uint8_t* Buffer, uint16_t length);
#ifdef __cplusplus
}
#endif

#endif
