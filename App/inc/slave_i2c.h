#ifndef SLAVE_I2C_h
#define SLAVE_I2C_h

#include <stdint.h>

/* Define I2C unit used for the example */
#define I2C_UNIT                        (CM_I2C3)
#define I2C_FCG_USE                     (FCG1_PERIPH_I2C3)
/* Define slave device address for example */
#define DEVICE_ADDR                     (0x06U)


/* Define port and pin for SDA and SCL */
#define I2C_SCL_PORT                    (GPIO_PORT_B)
#define I2C_SCL_PIN                     (GPIO_PIN_14)
#define I2C_SDA_PORT                    (GPIO_PORT_B)
#define I2C_SDA_PIN                     (GPIO_PIN_15)
#define I2C_GPIO_SCL_FUNC               (GPIO_FUNC_49)
#define I2C_GPIO_SDA_FUNC               (GPIO_FUNC_48)


#define I2C_EEI_IRQN_DEF                (INT005_IRQn)
#define I2C_RXI_IRQN_DEF                (INT006_IRQn)
#define I2C_TXI_IRQN_DEF                (INT003_IRQn)
#define I2C_TEI_IRQN_DEF                (INT004_IRQn)

#define I2C_INT_EEI_DEF                 (INT_SRC_I2C3_EEI)
#define I2C_INT_RXI_DEF                 (INT_SRC_I2C3_RXI)
#define I2C_INT_TXI_DEF                 (INT_SRC_I2C3_TXI)
#define I2C_INT_TEI_DEF                 (INT_SRC_I2C3_TEI)
#ifdef __cplusplus
extern "C" {

#endif

int32_t slave_i2c_init();
int32_t I2C_Slave_Transmit(uint8_t au8Data[], uint32_t u32Size, uint32_t u32Timeout);
#ifdef __cplusplus
}
#endif

#endif
