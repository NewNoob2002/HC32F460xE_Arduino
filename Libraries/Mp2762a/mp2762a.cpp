#include "mp2762a.h"

TwoWire *mp2762aI2c = nullptr;

bool mp2762aBegin(TwoWire *i2cBus)
{
	if(i2cBus == nullptr)
		return false;
	mp2762aI2c = i2cBus;
	return true;
}

uint8_t mp2762aReadRegister8(uint8_t reg)
{
    uint8_t  byte= 0;

    if(mp2762aI2c->beginTransmission(MP2762A_DEVICE_ADDRESS))
		{
			mp2762aI2c->requestFrom(MP2762A_DEVICE_ADDRESS, reg, &byte, 1);
		}
    return byte;
}

uint16_t mp2762aReadRegister16(uint8_t reg)
{
		uint8_t buf[2];

    if(mp2762aI2c->beginTransmission(MP2762A_DEVICE_ADDRESS))
		{
			mp2762aI2c->requestFrom(MP2762A_DEVICE_ADDRESS, reg, buf, 2);
		}
		uint16_t value = (buf[1] << 8) | buf[0];
    return value;
}

uint8_t mp2762aWriteRegister8(uint8_t reg, uint8_t data)
{
    if(mp2762aI2c->beginTransmission(MP2762A_DEVICE_ADDRESS))
		{
			uint8_t buf[2] = {reg, data};
			mp2762aI2c->write(buf, 2);
			mp2762aI2c->endTransmission();
			return 1;
		}
		mp2762aI2c->endTransmission();
    return 0;
}

// Given a bit field, and a startingBitValue
// Example: Battery voltage is bit 12.5mV per bit
float convertBitsToDoubler(uint16_t bitField, float startingBitValue)
{
    float totalMv = 0;
    for (int x = 0; x < 16; x++)
    {
        if (bitField & 0x0001)
            totalMv += startingBitValue;

        bitField >>= 1;

        startingBitValue *= 2;
    }
    return (totalMv);
}

// Set the Precharge threshold
// 5.8V, 6.0, 6.2, 6.4, 6.6, 6.8, 7.4, 7.2 (oddly out of order)
void mp2762setFastChargeVoltageMv(uint16_t mVoltLevel)
{
    // Default to 6.8V (requires option '2')
    uint8_t option = 1;         // This is option 2 confusingly
    uint8_t newVbattPre = 0x01; // Default to 6.8V

    if (mVoltLevel <= 5800)
    {
        option = 0;
        newVbattPre = 0x00; // 5.8V
    }
    else if (mVoltLevel <= 6000)
    {
        option = 0;
        newVbattPre = 0x01; // 6.0V
    }
    else if (mVoltLevel <= 6200)
    {
        option = 0;
        newVbattPre = 0x02; // 6.2V
    }
    else if (mVoltLevel <= 6400)
    {
        option = 0;
        newVbattPre = 0x03; // 6.4V
    }
    else if (mVoltLevel <= 6600)
    {
        option = 1;
        newVbattPre = 0x00; // 6.6V
    }
    else if (mVoltLevel <= 6800)
    {
        option = 1;
        newVbattPre = 0x01; // 6.8V
    }
    else if (mVoltLevel <= 7200)
    {
        option = 1;
        newVbattPre = 0x03; // 7.2V
    }
    else if (mVoltLevel <= 7400)
    {
        option = 1;
        newVbattPre = 0x02; // 7.4V
    }

    // Set the Precharge bits
    uint8_t status = 0;
    status = mp2762aReadRegister8(MP2762A_PRECHARGE_THRESHOLD);
    status &= ~(0x03 << 4); // Clear bits 4 and 5
    newVbattPre <<= 4;      // Shift to correct position
    status |= newVbattPre;  // Set bits accordingly
    mp2762aWriteRegister8(MP2762A_PRECHARGE_THRESHOLD, status);

    // Set the option bit
    status = mp2762aReadRegister8(MP2762A_PRECHARGE_THRESHOLD_OPTION);
    status &= ~(1 << 3); // Clear bit 3
    option <<= 3;        // Shift to correct position
    status |= option;    // Set bit accordingly
    mp2762aWriteRegister8(MP2762A_PRECHARGE_THRESHOLD_OPTION, status);
}

void mp2762setFastChargeCurrentMa(uint16_t currentLevelMa)
{
    // defualt to 1A
    uint8_t newIFast = 0x01;
		if(currentLevelMa <= 50)
		{
			newIFast = 0x01;
		}
		else{
			uint16_t steps = currentLevelMa / 50;
			newIFast = 0x01 * steps;
		}
		
    mp2762aWriteRegister8(MP2762A_SETCHARGE_CURRENT, newIFast);
}

void mp2762setPrechargeCurrentMa(uint16_t currentLevelMa)
{
    uint8_t newIPre = 0x03; // Default to 180mA

    if (currentLevelMa <= 180)
        newIPre = 0x03; // 180mA
    else
    {
        uint8_t steps = (currentLevelMa - 240) / 60; //(480 - 240)/ 60 = 4
        newIPre = 0x05 + steps;
    }

    // Set the Precharge current bits
    uint8_t status = 0;
    status = mp2762aReadRegister8(MP2762A_PRECHARGE_CURRENT);
    status &= ~(0x0F << 4); // Clear bits 7, 6, 5, 4
    newIPre <<= 4;            // Shift to correct position
    status |= newIPre;        // Set bits accordingly
    mp2762aWriteRegister8(MP2762A_PRECHARGE_CURRENT, status);
}

float mp2762getBatteryVoltageMv()
{
    uint16_t voltage = mp2762aReadRegister16(MP2762A_BATTERY_VOLTAGE);
    float batteryVoltage = convertBitsToDoubler(voltage >>= 6, 12.5); // Battery voltage is bit 15:6 so we need a 6 bit shift
    return (batteryVoltage);
}

float mp2762getSystemVoltageMv()
{
    uint16_t voltage = mp2762aReadRegister16(MP2762A_SYSTEM_VOLTAGE);
    float batteryVoltage = convertBitsToDoubler(voltage >>= 6, 12.5); // Battery voltage is bit 15:6 so we need a 6 bit shift
    return (batteryVoltage);
}

uint8_t mp2762getChargeStatus()
{
    uint8_t status = 0;
    status = mp2762aReadRegister8(MP2762A_STATUS);
    status >>= 2;
    status &= 0x03;
    return (status);
}

float mp2762getChargeCurrentMa()
{
    uint8_t CurrentL = 0, CurrentH = 0;
    CurrentL = mp2762aReadRegister8(MP2762A_CHARGE_CURRENT_L);
    CurrentH = mp2762aReadRegister8(MP2762A_CHARGE_CURRENT_H);

    uint16_t Current = (CurrentH << 8) | CurrentL;
    Current = (Current >> 6) & 0x03FF;

    float charge_current_mA = Current * 12.5;
    return charge_current_mA;
}

float mp2762getInputVolatgeMv()
{
    uint8_t VoltageL = 0, VoltageH = 0;
    VoltageL = mp2762aReadRegister8(MP2762A_INPUT_VOLTAGE_L);
    VoltageH = mp2762aReadRegister8(MP2762A_INPUT_VOLTAGE_H);

    uint16_t Voltage = (VoltageL << 8) | VoltageH;
    Voltage = (Voltage >> 6) & 0x03FF;

    float Input_voltage_mV = Voltage * 25;
    return Input_voltage_mV;
}

float mp2762getInputCurrentMa()
{
    uint8_t CurrentL = 0, CurrentH = 0;
    CurrentL = mp2762aReadRegister8(MP2762A_INPUT_CURRENT_L);
    CurrentH = mp2762aReadRegister8(MP2762A_INPUT_CURRENT_H);

    uint16_t Current = (CurrentH << 8) | CurrentL;
    Current = (Current >> 6) & 0x03FF;

    float charge_current_mA = Current * 6.25;
    return charge_current_mA;
}

void mp2762resetSafetyTimer()
{
    uint8_t status = 0;
    status = mp2762aReadRegister8(MP2762A_CONFIG_0);

    status &= ~(1 << 4); // Clear the CHG_EN bit
    mp2762aWriteRegister8(MP2762A_CONFIG_0, status);

    status |= (1 << 4); // Set the CHG_EN bit
    mp2762aWriteRegister8(MP2762A_CONFIG_0, status);
}

void mp2762registerReset()
{
    uint8_t status = 0;
    status = mp2762aReadRegister8(MP2762A_CONFIG_0);
    status |= 1 << 7; // Set REG_RST
    mp2762aWriteRegister8(MP2762A_CONFIG_0, status);
}