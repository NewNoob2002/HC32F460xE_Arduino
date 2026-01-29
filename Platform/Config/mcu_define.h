#pragma once

#include <stdint.h>
typedef struct ledState_t{
    uint32_t lastToggleTime;
    uint32_t currentRate;
} ledState_t;

typedef struct online_device_t
{
    uint8_t bq40z50;
    uint8_t mp2762;
		uint8_t eg25_board;
}online_device_t;

typedef enum Charger_Status_t
{
	notCharge = 0,
	normalCharge,
	fastCharge,
}Charger_Status_t;

typedef struct Battery_Compensate_t{
    uint8_t comp_offset;
    uint8_t is_comp_updated;
		uint8_t last_soc;
    uint32_t stable_timer;
} Battery_Compensate_t;

typedef struct BatteryInfo_t
{
    uint16_t Actual_Percent;
    uint16_t Processed_Percent;
    uint16_t Percent;
    uint16_t Temp;
    uint16_t Voltage;
    uint16_t LowBatteryCount;

    Charger_Status_t chargeStatus;
		Battery_Compensate_t Compensate;
} BatteryInfo_t, *pBatteryInfo_t;

typedef struct RecordInfo_t {
    uint8_t record_status; // 0-off, 1-on
    uint8_t record_op;
    uint8_t record_change_flag;
} RecordInfo_t;

typedef struct Power_Monitor_t
{
    BatteryInfo_t batteryInfo;
    uint32_t WatchDogLastFeedTime;
    uint32_t BatteryLastHandleTime;
		uint32_t ChargerLastHandleTime;
	
    uint32_t pannel_power_on_time;
		uint32_t pannel_shutdown_req_time;
	
    uint8_t LinuxPowerOff;
    uint8_t LowBatteryPowerOff;
    uint8_t ShutdownReq;
    uint8_t ShutdownEnsure;
    uint8_t Force_ShutDown;

    uint8_t reset_flag;
    uint8_t poweroff_flag;
    uint8_t ExternalPower;
    uint8_t panel_power_on;

} Power_Monitor_t;

typedef struct SystemInfo_t
{
    uint8_t eg25_overtime;
    uint8_t work_status;
    uint16_t i2c_communicate_err_count;
    uint16_t i2c__err_count;

    online_device_t online_device;
    RecordInfo_t recordInfo;
    Power_Monitor_t powerMonitor;
}SystemInfo_t, *pSystemInfo_t;

extern SystemInfo_t systemInfo;
