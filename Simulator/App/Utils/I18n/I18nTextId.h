#ifndef I18N_TEXT_ID_H
#define I18N_TEXT_ID_H

#include <cstddef>

namespace I18n {

enum class TextId : size_t {
    Press = 0,
    ShutdownHint,
    HardwareCheckWarning,
    SaveConfigWarning,
    SaveConfigMessage,
    SaveConfigPowerOffBoard,
    SaveConfigPowerOffLowBattery,
    SaveConfigPowerOffForced,
    SaveConfigPowerOffPushKey,
    RecordTypeOptions,
    RecordIntervalOptions,
    WorkRadioProtocolOptions,
    WorkRadioChannelOptions,
    SystemWorkTitle,
    SystemWorkInfo,
    SystemGpsTitle,
    SystemGpsInfo,
    SystemWifiTitle,
    SystemWifiInfo,
    SystemBatteryTitle,
    SystemBatteryInfo,
    SystemStorageTitle,
    SystemStorageInfo,
    SystemTitle,
    SystemInfo,
    WorkModeRover,
    WorkModeBase,
    StatusOn,
    StatusOff,
    NtripServer,
    NtripClient,
    NotAvailable,
    BatteryExternalPower,
    BatteryNotCharge,
    BatteryFastCharge,
    BatteryNormalCharge,
    StorageHour,
    PositionBase,
    PositionNone,
    PositionSingle,
    PositionFix,
    PositionFloat,
    StarMapTitle,
    StarMapGps,
    StarMapBds,
    StarMapGlonass,
    StarMapGalileo,
    StarMapSbas,
    StarMapQzss,
    StarMapIrnss,
    Count
};

} // namespace I18n

#endif
