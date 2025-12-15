#ifndef DATA_PROC_DEF_H
#define DATA_PROC_DEF_H

#include <stdint.h>

#if defined(__cplusplus)
namespace DataProc
{

/* Recorder */
typedef enum
{
    RECORDER_CMD_START,
    RECORDER_CMD_STOP,
} Recorder_Cmd_t;

/* Storage */
typedef enum
{
    STORAGE_CMD_LOAD,
    STORAGE_CMD_SAVE,
    STORAGE_CMD_ADD,
    STORAGE_CMD_REMOVE
} Storage_Cmd_t;

typedef enum
{
    STORAGE_TYPE_UNKNOW,
    STORAGE_TYPE_INT,
    STORAGE_TYPE_FLOAT,
    STORAGE_TYPE_DOUBLE,
    STORAGE_TYPE_STRING
} Storage_Type_t;

typedef struct
{
    Storage_Cmd_t cmd;
    const char* key;
    void* value;
    uint16_t size;
    Storage_Type_t type;
} Storage_Info_t;

#define STORAGE_VALUE_REG(act, data, dataType)\
do{\
    DataProc::Storage_Info_t info; \
    DATA_PROC_INIT_STRUCT(info); \
    info.cmd = DataProc::STORAGE_CMD_ADD; \
    info.key = #data; \
    info.value = &data; \
    info.size = sizeof(data); \
    info.type = dataType; \
    act->Notify("Storage", &info, sizeof(info)); \
}while(0)

typedef struct
{
    bool isDetect;
    float totalSizeMB;
    float freeSizeMB;
    const char* type;
} Storage_Basic_Info_t;

/* StatusBar */
typedef enum
{
    STATUS_BAR_STYLE_TRANSP,
    STATUS_BAR_STYLE_BLACK,
} StatusBar_Style_t;

typedef enum
{
    STATUS_BAR_CMD_APPEAR,
    STATUS_BAR_CMD_SET_STYLE,
    STATUS_BAR_CMD_SET_LABEL_REC
} StatusBar_Cmd_t;

typedef struct
{
    StatusBar_Cmd_t cmd;
    union
    {
        bool appear;
        bool delay;
        bool record_active;
        StatusBar_Style_t style;
    } param;
} StatusBar_Info_t;

/* MusicPlayer */
typedef struct
{
    const char* music;
} MusicPlayer_Info_t;

/* SysConfig */
typedef enum
{
    SYSCONFIG_CMD_LOAD,
    SYSCONFIG_CMD_SAVE,
} SysConfig_Cmd_t;

typedef struct
{
    SysConfig_Cmd_t cmd;
    float longitude;
    float latitude;
    int16_t timeZone;
    bool soundEnable;
    char language[8];
    char arrowTheme[16];
    char mapDirPath[16];
    char mapExtName[8];
    bool mapWGS84;
} SysConfig_Info_t;

}

#endif //__cplusplus

#endif