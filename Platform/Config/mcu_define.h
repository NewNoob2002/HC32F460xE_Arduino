#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "../../Simulator/App/Common/DataProc/DataProc_Def.h"

typedef struct ledState_t {
    uint32_t lastToggleTime;
    uint32_t currentRate;
} ledState_t;

typedef struct online_device_t {
    bool bq40z50;
    bool mp2762;
    bool eg25_board;
} online_device_t;

typedef enum Charger_Status_t {
    notCharge = 0,
    normalCharge,
    fastCharge,
} Charger_Status_t;

typedef struct BatteryInfo_t {
    uint16_t Actual_Percent;
    uint16_t Processed_Percent;
    uint16_t Percent;
    uint16_t Temp;
    uint16_t Voltage;
    float Percent_f;
    float Voltage_f;
    float Temp_f;
    Charger_Status_t chargeStatus;
} BatteryInfo_t, *pBatteryInfo_t;

typedef struct Power_Monitor_t {
    BatteryInfo_t batteryInfo;
    uint32_t WatchDogLastFeedTime;
    uint32_t BatteryLastHandleTime;
    uint32_t ChargerLastHandleTime;
    uint32_t PowerOffRequeset_time;

    uint16_t LowBattery_Count;
    uint16_t PowerKey_PressCount;

    bool LinuxPowerOff;
    bool LowBatteryPowerOff;
    bool ShutdownReq;
    bool ShutdownEnsure;

    bool Force_ShutDown_Powerkey;
    bool Force_ShutDown_Funckey;

    uint8_t reset_flag;
    uint8_t poweroff_flag;
    uint8_t ExternalPower;
    bool panel_power_on;
} Power_Monitor_t, *pPower_Monitor_t;

typedef struct PositionInfo_t {
    uint8_t coordinate_status; // 0- none, 1-single, 4-fixed, 5- float, 7-fixedcoordinate
    uint8_t satellite_number_used;
    uint8_t satellite_number_track;
    double coordinate_lon;
    double coordinate_lat;
    double coordinate_alt;
} PositionInfo_t;

typedef struct RecordInfo_t {
    uint8_t record_status; // 0-off, 1-on
    float record_leftspace;
    uint8_t record_type; // 1-xyz 2-Rinex3.02
    uint8_t record_interval; // 0x00-15min, 0x01-60min, 0x02-120min, 0x04-240min, 0x18- 24hour
    uint8_t record_op;
    uint8_t record_name[16];
#if defined(__cplusplus)
    DataProc::Recorder_Cmd_t cmd;
#endif
} RecordInfo_t;

typedef struct RadioInfo_t {
    uint8_t radio_mode;
    uint8_t radio_status;
    uint8_t radio_protocol;
    uint8_t radio_channel;
} RadioInfo_t;

typedef struct WifiInfo_t {
    uint8_t wifi_mode;
    uint8_t wifi_status;
    char wifi_ssid[16];
    char wifi_ip[4];
} WifiInfo_t;

typedef struct NtripInfo_t {
    uint8_t gprs_status;
    uint8_t NtripServer_status;
    uint8_t NtripClient_status;
    uint8_t NtripServer_IP[4];
    uint8_t NtripClient_IP[4];
    uint8_t NtripServer_Mountpoint[32];
    uint8_t NtripClient_Mountpoint[32];
} NtripInfo_t;

typedef enum WorkMode_t {
    rover_mode = 0,
    base_mode,
    single_mode,
    autobase_mode
} WorkMode_t;

typedef struct SystemInfo_t {
    bool eg25_overtime;
    uint8_t panel_operation_flag;
    uint8_t work_status;
    uint16_t i2c_communicate_err_count;

    online_device_t online_device;
    WifiInfo_t wifiInfo;
    RecordInfo_t recordInfo;
    PositionInfo_t positionInfo;
    RadioInfo_t radioInfo;
    Power_Monitor_t powerMonitor;
    NtripInfo_t ntripInfo;
    WorkMode_t work_mode;
} SystemInfo_t, *pSystemInfo_t;

extern SystemInfo_t systemInfo;
