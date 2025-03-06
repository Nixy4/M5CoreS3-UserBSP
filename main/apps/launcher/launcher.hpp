#pragma once

//*Device
#include "M5CoreS3.hpp"
//*Middleware
#include "mooncake.h"
#include "lvgl.h"

using namespace mooncake;

class Launcher : public BasicAbility
{
private:
    const char* TAG = "Launcher";
    // lv_obj_t* scr;
    // TaskHandle_t taskHandle;
    // void onBoot();
    // void task(void* arg);

public:
    void onCreate() override;
    void onDestroy() override;
    void onRunning() override;
    // void onOpen() override;
    // void onClose() override;
    // void onSleeping() override;
};