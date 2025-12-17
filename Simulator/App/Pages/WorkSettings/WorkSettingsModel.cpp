#include "WorkSettingsModel.h"

using namespace Page;

WorkSettingsModel::WorkSettingsModel()
{

}

void WorkSettingsModel::Init()
{
    account = new Account("LiveMapModel", DataProc::Center(), 0, this);
    account->Subscribe("StatusBar");
    account->SetEventCallback(onEvent);
}

void WorkSettingsModel::Deinit()
{
    if (account)
    {
        delete account;
        account = nullptr;
    }
}

int WorkSettingsModel::onEvent(Account* account, Account::EventParam_t* param)
{
    if (param->event != Account::EVENT_PUB_PUBLISH)
    {
        return Account::RES_UNSUPPORTED_REQUEST;
    }

    // if (strcmp(param->tran->ID, "SportStatus") != 0
    //         || param->size != sizeof(HAL::SportStatus_Info_t))
    // {
    //     return Account::RES_PARAM_ERROR;
    // }

    return Account::RES_OK;
}
