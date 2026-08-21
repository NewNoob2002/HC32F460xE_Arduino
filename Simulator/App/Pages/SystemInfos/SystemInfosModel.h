#ifndef __SYSTEM_INFOS_MODEL_H
#define __SYSTEM_INFOS_MODEL_H

#include "../../Platform/Config/mcu_define.h"
#include "Common/DataProc/DataProc_Def.h"
#include "Utils/DataCenter/Account.h"

namespace Page {

class SystemInfosModel {
  public:
    void Init();
    void Deinit();

    void GetGPSInfo(PositionInfo_t* gps) const;

    void GetBatteryInfo(BatteryInfo_t* power) const;

    void GetRecordInfo(RecordInfo_t* record) const;

    void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

  private:
    Account* account = nullptr;
};

} // namespace Page

#endif
