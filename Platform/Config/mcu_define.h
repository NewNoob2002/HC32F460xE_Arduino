#pragma once

#include <stdint.h>
#include <stdbool.h>

#define SHARED_MAGIC_LIVE  0x55AAAA55 // 正常运行/请求状态
#define SHARED_MAGIC_CRASH 0xDEADBEEF // 发生崩溃状态

enum BootCommand {
    CMD_NORMAL_BOOT = 0x10, // 正常启动
    CMD_ENTER_IAP = 0x20, // App 请求进入升级模式
    CMD_SKIP_DELAY = 0x30 // 跳过 Boot 延时，直接跳 App
};

typedef struct {
    uint32_t magic; // 魔数，用来判断数据是否有效（如 0x55AAAA55）
    uint32_t command; // 命令：1-进入升级模式, 2-跳过延时, 3-App崩溃了
    uint32_t crash_pc; // 记录崩溃时的 PC 地址
    uint32_t reset_count;
} SharedData_t;

typedef struct ledState_t {
    uint32_t lastToggleTime;
    uint16_t currentRate;
    bool usedByOtherTask;
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

typedef enum On_Off_Status_t {
    On_Off_Status_OFF = 0,
    On_Off_Status_ON = 1,
} On_Off_Status_t;

typedef enum RadioMode_t {
    radio_mode_tx = 0,
    radio_mode_rx = 1,
    radio_mode_bridge = 2,
} RadioMode_t;

typedef enum Protocol_index_t {
    TRIMTALK,
    TRIMMK3,
    TT450S,
    TRANSEOT,
    SOUTH,
    HUACE,
    SATEL,
    CCS,
    PROTOCOL_MAX
} Protocol_index_t;

typedef enum Channel_index_t {
    Channel1,
    Channel2,
    Channel3,
    Channel4,
    Channel5,
    Channel6,
    Channel7,
    Channel8,
    Channel9,
    ChannelMax
} Channel_index_t;

typedef enum WorkMode_t {
    rover_mode = 0,
    base_mode,
    single_mode,
    autobase_mode
} WorkMode_t;

typedef enum PositionStatus_t {
    position_none = 0,
    position_single = 1,
    position_fix = 4,
    position_float = 5,
} PositionStatus_t;

typedef struct BatteryInfo_t {
    uint16_t Actual_Percent;
    uint16_t Processed_Percent;
    uint16_t Percent;
    uint16_t Temp;
    uint16_t Voltage;
    uint16_t LowBatteryCount;
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

    bool LinuxPowerOff;
    bool LowBatteryPowerOff;
    bool ShutdownReq;
    bool ShutdownEnsure;

    bool Force_ShutDown;

    uint8_t reset_flag;
    uint8_t poweroff_flag;
    uint8_t ExternalPower;
    bool panel_power_on;
    uint32_t pannel_power_on_time;
} Power_Monitor_t, *pPower_Monitor_t;

typedef struct PositionInfo_t {
    PositionStatus_t coordinate_status; // 0- none, 1-single, 4-fixed, 5- float, 7-fixedcoordinate
    uint8_t satellite_number_used;
    uint8_t satellite_number_track;
    double coordinate_lon;
    double coordinate_lat;
    double coordinate_alt;
} PositionInfo_t;

typedef struct RecordInfo_t {
    On_Off_Status_t record_status; // 0-off, 1-on
    float record_leftspace;
    uint8_t record_type; // 1-xyz 2-Rinex3.02
    uint8_t record_interval; // 0x00-15min, 0x01-60min, 0x02-120min, 0x04-240min, 0x18- 24hour
    uint8_t record_op;
    uint8_t record_name[16];
    uint8_t record_change_flag;
} RecordInfo_t;

typedef struct RadioInfo_t {
    RadioMode_t radio_mode;
    On_Off_Status_t radio_status;
    uint8_t radio_protocol;
    uint8_t radio_channel;
    uint8_t radio_change_flag;
} RadioInfo_t;

typedef struct WifiInfo_t {
    uint8_t wifi_mode;
    On_Off_Status_t wifi_status;
    char wifi_ssid[16];
    char wifi_ip[4];
} WifiInfo_t;

typedef struct NtripInfo_t {
    On_Off_Status_t gprs_status;
    On_Off_Status_t NtripServer_status;
    On_Off_Status_t NtripClient_status;
    uint8_t NtripServer_IP[4];
    uint8_t NtripClient_IP[4];
    uint8_t NtripServer_Mountpoint[32];
    uint8_t NtripClient_Mountpoint[32];
} NtripInfo_t;

typedef struct SystemInfo_t {
    bool eg25_overtime;
    uint8_t work_status;
    uint16_t i2c_communicate_err_count;
    uint16_t i2c__err_count;

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
