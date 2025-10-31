#ifndef _HardWAREI2C_H
#define _HardWAREI2C_H
#include "hc32_ll.h"

/* Define I2C unit used for the example */
#define I2C_UNIT                        (CM_I2C2)
#define I2C_FCG_USE                     (FCG1_PERIPH_I2C2)

/* Define port and pin for SDA and SCL */
#define I2C_SCL_PORT                    (GPIO_PORT_A)
#define I2C_SCL_PIN                     (GPIO_PIN_09)
#define I2C_SDA_PORT                    (GPIO_PORT_A)
#define I2C_SDA_PIN                     (GPIO_PIN_08)
#define I2C_GPIO_SCL_FUNC               (GPIO_FUNC_51)
#define I2C_GPIO_SDA_FUNC               (GPIO_FUNC_50)

int32_t Slave_Initialize(void);

#endif

