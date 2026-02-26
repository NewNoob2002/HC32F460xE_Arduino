#include <Wire.h>

i2c_peripheral_config_t I2C2_config = {
    .register_base = CM_I2C2,
    .clock_id = PWC_FCG1_I2C2,
    .scl_pin_function = GPIO_FUNC_51,
    .sda_pin_function = GPIO_FUNC_50,
};

TwoWire Wire(&I2C2_config, PA9, PA8);

#define REG_TO_I2Cx(reg) ((reg == CM_I2C1) ? "I2C1" : (reg == CM_I2C2) ? "I2C2" : (reg == CM_I2C3) ? "I2C3" : "Unknown")
// #define WIRE_ENABLE_DEBUG
#ifdef WIRE_ENABLE_DEBUG
#define WIRE_DEBUG_PRINTF(fmt, ...)                                                                                    \
    CORE_DEBUG_PRINTF("[%s] " fmt, REG_TO_I2Cx(this->_config->register_base), ##__VA_ARGS__)
#else
#define WIRE_DEBUG_PRINTF(fmt, ...)
#endif
TwoWire::TwoWire(i2c_peripheral_config_t* config, const gpio_pin_t scl_pin, const gpio_pin_t sda_pin) {
    this->_config = config;
    this->_scl_pin = scl_pin;
    this->_sda_pin = sda_pin;
#ifdef _WIRE_USE_RINGBUFFER

#endif
}

int
TwoWire::begin(uint32_t clockFreq) {
    this->_clock_frequency = clockFreq;

    GPIO_SetFunction(this->_scl_pin, this->_config->scl_pin_function);
    GPIO_SetFunction(this->_sda_pin, this->_config->sda_pin_function);

    FCG_Fcg1PeriphClockCmd(this->_config->clock_id, ENABLE);

    int32_t i32Ret;
    stc_i2c_init_t stcI2cInit;
    float32_t fErr;

    (void)I2C_DeInit(this->_config->register_base);
    (void)I2C_StructInit(&stcI2cInit);
    if (this->_clock_frequency <= 100 * 1000) {
        stcI2cInit.u32ClockDiv = I2C_CLK_DIV8;
        stcI2cInit.u32Baudrate = this->_clock_frequency;
        stcI2cInit.u32SclTime = 3UL;
    } else if (this->_clock_frequency == 400 * 1000) {
        stcI2cInit.u32ClockDiv = I2C_CLK_DIV2;
        stcI2cInit.u32Baudrate = this->_clock_frequency;
        stcI2cInit.u32SclTime = 5UL;
    }
    i32Ret = I2C_Init(this->_config->register_base, &stcI2cInit, &fErr);

    if (i32Ret != LL_OK) {
        WIRE_DEBUG_PRINTF("Failed to initialize I2C, error:%f, ret = %d", fErr, i32Ret);
        return 0;
    }

    I2C_BusWaitCmd(this->_config->register_base, ENABLE);

    this->isInitliased = true;
    WIRE_DEBUG_PRINTF("I2c init success, in mode: master\n");
    return 1;
}

void
TwoWire::end() {
    I2C_DeInit(this->_config->register_base);
}

bool
TwoWire::beginTransmission(uint8_t address) {
    uint32_t i32Ret = LL_ERR;
    bool result = false;

    I2C_Cmd(this->_config->register_base, ENABLE);

    I2C_SWResetCmd(this->_config->register_base, ENABLE);
    I2C_SWResetCmd(this->_config->register_base, DISABLE);
    if (I2C_Start(this->_config->register_base, WIRE_TIMEOUT) == LL_OK) {
        if (LL_OK == I2C_TransAddr(this->_config->register_base, address, I2C_DIR_TX, WIRE_TIMEOUT)) {
            result = true;
        }
    }
    return result;
}

uint8_t
TwoWire::endTransmission(bool stopBit) {
    if (stopBit) {
        // Stop by software
        I2C_Stop(this->_config->register_base, WIRE_TIMEOUT);
        // Disable I2C
        I2C_Cmd(this->_config->register_base, DISABLE);
        return 1;
    }

    return 0;
}

size_t
TwoWire::write(uint8_t data) {
    if (I2C_TransData(this->_config->register_base, &data, 1, WIRE_TIMEOUT) == LL_OK) {
        return 1;
    }
    return 0;
}

size_t
TwoWire::write(const uint8_t* data, size_t quantity) {
    if (I2C_TransData(this->_config->register_base, data, quantity, WIRE_TIMEOUT) == LL_OK) {
        return quantity;
    }
    return 0;
}

size_t
TwoWire::requestFrom(uint8_t address, uint8_t register_address, uint8_t* buffer, uint8_t quantity, bool sendStop) {
    int32_t ret = LL_ERR;
    if (write(register_address) == 0) {
        WIRE_DEBUG_PRINTF("I2c write register address failed\n");
        return 0;
    }
    ret = I2C_Restart(this->_config->register_base, WIRE_TIMEOUT);
    if (ret != LL_OK) {
        WIRE_DEBUG_PRINTF("I2c restart failed, ret = %d\n", ret);
        return 0;
    }
    if (1UL == quantity) {
        I2C_AckConfig(this->_config->register_base, I2C_NACK);
    }
    ret = I2C_TransAddr(this->_config->register_base, address, I2C_DIR_RX, WIRE_TIMEOUT);
    if (ret != LL_OK) {
        WIRE_DEBUG_PRINTF("I2c restart trans addr failed, ret = %d\n", ret);
        return 0;
    }
    if (sendStop) {
        ret = I2C_MasterReceiveDataAndStop(this->_config->register_base, buffer, quantity, WIRE_TIMEOUT);
    }
    I2C_AckConfig(this->_config->register_base, I2C_ACK);
    if (LL_OK != ret) {
        I2C_Stop(this->_config->register_base, WIRE_TIMEOUT);
        WIRE_DEBUG_PRINTF("I2c read register 0x%02x form 0x%02x  %d bytes failed, return %d\n", register_address,
                          address, quantity, ret);
        quantity = 0;
    } else {
        WIRE_DEBUG_PRINTF("I2c read register 0x%02x form 0x%02x %d bytes success\n", register_address, address,
                          quantity);
    }
    I2C_Cmd(this->_config->register_base, DISABLE);
    return quantity;
}

bool
TwoWire::isDeviceOnline(uint8_t address) {
    if (beginTransmission(address)) {
        endTransmission();
    } else {
        endTransmission();
        return false;
    }
    return true;
}

int
TwoWire::scanDeivces(voidFuncPtrWithArg callback) {
    int nDevice = 0;
    for (uint8_t i = 0x01; i < 0x7F; i++) {
        if (isDeviceOnline(i)) {
            if (callback) {
                callback(&i);
            } else {
                CORE_DEBUG_PRINTF("Found device address: %02x\n", i);
            }
            nDevice++;
        } else {
            if (callback) {
                callback(nullptr);
            } else {
                CORE_DEBUG_PRINTF("Not Found device address: %02x\n", i);
            }
        }
    }
    return nDevice;
}
