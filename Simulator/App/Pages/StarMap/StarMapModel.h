#ifndef __STAR_MAP_MODEL_H
#define __STAR_MAP_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page {

class StarMapModel {
public:
    StarMapModel() = default;
    ~StarMapModel() = default;

    void Init();
    void Deinit();
    void SetStatusBarStyle(DataProc::StatusBar_Style_t style) const;

private:
    Account* account{};
};

}

#endif
