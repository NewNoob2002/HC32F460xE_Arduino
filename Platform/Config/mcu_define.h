#pragma once

#include <stdint.h>
typedef struct ledState_t{
    uint32_t lastToggleTime;
    uint32_t currentRate;
} ledState_t;

typedef struct online_device_t
{
    bool bq40z50;
    bool mp2762;
		bool eg25_board;
}online_device_t;

typedef enum Charger_Status_t
{
	notCharge = 0,
	normalCharge,
	fastCharge,
}Charger_Status_t;

typedef struct BatteryInfo_t
{
		uint16_t Actual_Percent;
		uint16_t Processed_Percent;
		uint16_t Percent;
		uint16_t Temp;
		uint16_t Voltage;
		Charger_Status_t chargeStatus;
} BatteryInfo_t, *pBatteryInfo_t;

typedef struct Power_Monitor_t
{
	uint32_t BatteryLastHandleTime;
	uint32_t ChargerLastHandleTime;
	uint32_t PowerOffRequeset_time;
	
	uint16_t LowBattery_Count;
	uint16_t PowerKey_PressCount;

	bool LinuxPowerOff;
	bool LowBatteryPowerOff;
	bool PowerOffReq;
	bool PowerOFF_Ensure;
	
	bool Force_ShutDown_Powerkey;
	bool Force_ShutDown_Funckey;
} Power_Monitor_t;

typedef struct SystemInfo_t
{
	  uint8_t hw_version[8];
	  uint8_t sw_version[8];
	  uint8_t reset_flag;
	  uint8_t poweroff_flag;
	  uint8_t record_flag;
	  uint8_t record_op;
		uint8_t usb_power_flag;
	  uint8_t eg25_overtime_flag;
		bool mcu_powerOn_Done;
	
		Power_Monitor_t powerMonitor;
		BatteryInfo_t batteryInfo;
}SystemInfo_t, *pSystemInfo_t;

extern SystemInfo_t systemInfo;
