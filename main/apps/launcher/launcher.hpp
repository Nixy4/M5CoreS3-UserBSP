#pragma once
//*OS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//*Device
#include "M5CoreS3.hpp"
//*Middleware
#include "mooncake.h"

using namespace mooncake;

class Launcher : public AppAbility
{
private:
  M5CoreS3& core = M5CoreS3::getInstance();
  const char* TAG = "Launcher";
  lv_obj_t* _lvScr = nullptr;
  lv_screen_load_anim_t _lvBootAnim = LV_SCR_LOAD_ANIM_FADE_IN;
public:
  Launcher();
  void onSleeping() override;
  void onOpen() override;
  void onRunning() override;
  void onClose() override;
  ~Launcher();
};