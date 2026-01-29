#include "HAL.h"
#include "Wire.h"
#include "bq40z50.h"
#include "mp2762a.h"

void i2c_scan_callback(void *e)
{
    if (e == nullptr)
        return;
    uint8_t addr = *(uint8_t *)e;
    switch (addr) {
        default: {
            CORE_DEBUG_PRINTF("0x%02X - Unknow", addr);
            break;
        }

        case 0x0B: {
            CORE_DEBUG_PRINTF("0x%02X - BQ40Z50 Battery Pack Manager / Fuel gauge", addr);
            systemInfo.online_device.bq40z50 = 1;
						bq40z50Begin(&Wire);
						checkBatteryInfo(&systemInfo.powerMonitor.batteryInfo);
            break;
        }

        case 0x5C: {
            CORE_DEBUG_PRINTF("0x%02X - MP27692A Power Management / Charger", addr);
            systemInfo.online_device.mp2762 = 1;
						mp2762aBegin(&Wire);
						chagrer_begin(&systemInfo.powerMonitor.batteryInfo);
            break;
        }
    }
}

int HAL::I2C_Scan()
{
    if (!Wire.begin()) {
        CORE_DEBUG_PRINTF("I2C: init failed");
        return -1;
    }
    CORE_DEBUG_PRINTF("I2C: device scanning...");

    return Wire.scanDeivces(i2c_scan_callback);
}