#ifndef SHUTDOWN_VIEW_H
#define SHUTDOWN_VIEW_H

#include "../Page.h"

namespace Page
{

class ShutdownView
{
public:
    void Create(lv_obj_t* root);

public:
    struct
    {
        lv_obj_t* labelTitle;
        lv_obj_t* labelTick;
    } ui;

private:

};

}

#endif // !SHUTDOWN_VIEW_H
