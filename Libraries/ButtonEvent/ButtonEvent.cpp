#include "ButtonEvent.h"
#include <cstring>

#define ARDUINO
#ifdef ARDUINO
#  include "delay.h"
#  define GET_TICK() millis()
#endif

#ifndef GET_TICK
#  error "Please define the GET_TICK() function to get the system time "
#endif

#ifndef UINT32_MAX
#  define UINT32_MAX  4294967295u
#endif

/**
  * @brief  按键事件构造函数
  * @param  LongPressTimeMs_Set: 按键长按触发超时设置
  * @param  LongPressTimeRepeatMs_Set: 长按重复触发时间
  * @retval 无
  */
ButtonEvent::ButtonEvent(
    uint16_t longPressTime,
    uint16_t longPressTimeRepeat,
    uint16_t doubleClickTime
)
{
    memset(&priv, 0, sizeof(priv));

    priv.longPressTimeCfg = longPressTime;
    priv.longPressRepeatTimeCfg = longPressTimeRepeat;
    priv.doubleClickTimeCfg = doubleClickTime;

    priv.lastLongPressTime = priv.lastClickTime = priv.lastPressTime = 0;
    priv.isLongPressed = false;
    priv.nowState = STATE_NO_PRESS;
    priv.clickState = CLICK_STATE_NONE;

    priv.eventCallback = NULL;
}

/**
  * @brief  获取与上次时间的时间差(带uint32溢出识别)
  * @param  prevTick: 上的时间戳
  * @retval 时间差
  */
uint32_t ButtonEvent::GetTickElaps(uint32_t prevTick)
{
    uint32_t actTime = GET_TICK();

    if(actTime >= prevTick)
    {
        prevTick = actTime - prevTick;
    }
    else
    {
        prevTick = UINT32_MAX - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}

/**
  * @brief  按键事件绑定
  * @param  function: 回调函数指针
  * @retval 无
  */
void ButtonEvent::EventAttach(FuncCallback_t function)
{
    priv.eventCallback = function;
}

/**
  * @brief  监控事件，建议扫描周期10ms
  * @param  nowState: 当前按键状态
  * @retval 无
  */
void ButtonEvent::EventMonitor(bool isPress)
{
    if(priv.eventCallback == NULL)
    {
        return;
    }

    // 按键按下处理
    if (isPress && priv.nowState == STATE_NO_PRESS)
    {
        priv.nowState = STATE_PRESS;
        IsPressed = true;
        priv.lastPressTime = GET_TICK();

        priv.eventCallback(this, EVENT_PRESSED);
        priv.eventCallback(this, EVENT_CHANGED);
    }

    if(priv.nowState == STATE_NO_PRESS)
    {
        // 处理延时单击检测
        if(priv.clickState == CLICK_STATE_WAIT_DOUBLE && 
           GetTickElaps(priv.firstClickTime) >= priv.doubleClickTimeCfg)
        {
            priv.clickState = CLICK_STATE_NONE;
            
            // 触发延迟的单击事件
            if(priv.firstClickWasShort)
            {
                priv.eventCallback(this, EVENT_SHORT_CLICKED);
            }
            priv.eventCallback(this, EVENT_CLICKED);
        }
        return;
    }

    // 按键持续按下处理
    if(isPress)
    {
        priv.eventCallback(this, EVENT_PRESSING);

        // 长按检测
        if (GetTickElaps(priv.lastPressTime) >= priv.longPressTimeCfg)
        {
            priv.nowState = STATE_LONG_PRESS;

            if(!priv.isLongPressed)
            {
                priv.eventCallback(this, EVENT_LONG_PRESSED);
                priv.lastLongPressTime = GET_TICK();
                IsLongPressed = priv.isLongPressed = true;
            }
            else if(GetTickElaps(priv.lastLongPressTime) >= priv.longPressRepeatTimeCfg)
            {
                priv.lastLongPressTime = GET_TICK();
                priv.eventCallback(this, EVENT_LONG_PRESSED_REPEAT);
            }
        }
    }
    // 按键释放处理
    else if (!isPress)
    {
        priv.nowState = STATE_NO_PRESS;
        uint32_t currentTime = GET_TICK();
        uint32_t pressDuration = GetTickElaps(priv.lastPressTime);
        bool isShortClick = (pressDuration < priv.longPressTimeCfg);

        // 长按释放事件
        if(priv.isLongPressed)
        {
            priv.eventCallback(this, EVENT_LONG_PRESSED_RELEASED);
            priv.isLongPressed = false;
            IsClicked = true;
            priv.eventCallback(this, EVENT_RELEASED);
            priv.eventCallback(this, EVENT_CHANGED);
            return; // 长按释放后不处理点击事件
        }

        priv.isLongPressed = false;
        IsClicked = true;

        // 双击/单击检测逻辑
        if(priv.clickState == CLICK_STATE_WAIT_DOUBLE)
        {
            // 这是第二次点击，触发双击事件
            priv.clickState = CLICK_STATE_NONE;
            priv.clickCnt++;
            priv.eventCallback(this, EVENT_DOUBLE_CLICKED);
        }
        else
        {
            // 这是第一次点击，进入等待状态
            priv.clickState = CLICK_STATE_WAIT_DOUBLE;
            priv.firstClickTime = currentTime;
            priv.firstClickWasShort = isShortClick;
            // 不立即触发单击事件，等待双击超时或第二次点击
        }

        priv.lastClickTime = currentTime;
        priv.eventCallback(this, EVENT_RELEASED);
        priv.eventCallback(this, EVENT_CHANGED);
    }
}

