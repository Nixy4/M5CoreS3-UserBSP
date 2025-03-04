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
  //常量
  M5CoreS3& core = M5CoreS3::getInstance();
  const char* TAG = "Launcher";
  //变量
  lv_obj_t* _scr_lv = nullptr;

public:
  Launcher();
  void onSleeping() override;
  void onOpen() override;
  void onRunning() override;
  void onClose() override;
  ~Launcher();

private:
  void _bootAnim();
};