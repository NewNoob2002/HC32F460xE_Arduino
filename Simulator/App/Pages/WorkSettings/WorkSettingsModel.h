#ifndef __LIVEMAP_MODEL_H
#define __LIVEMAP_MODEL_H

#include "lvgl/lvgl.h"
#include "Common/DataProc/DataProc.h"

namespace Page
{

class WorkSettingsModel
{
public:
    WorkSettingsModel();
    ~WorkSettingsModel() {}
    void Init();
    void Deinit();

private:
    Account* account;
    static int onEvent(Account* account, Account::EventParam_t* param);

private:
};

}

#endif
