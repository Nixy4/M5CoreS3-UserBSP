#pragma once

#include "M5CoreS3.hpp"
#include "mooncake.h"

using namespace std;
using namespace mooncake;

class Launcher : public UIAbility
{
private:
  lv_obj_t* _scr;
  int appCount = 0;
public:
  Launcher();
  ~Launcher();
  void onCreate() override;
  void onDestroy() override;
  void onForeground() override;
  void onHide() override;
  void onBackground() override;
  void onShow() override;
  void appButtonCreate(int appId);
  int appRegister(unique_ptr<AppAbility> app);
private:
  static bool isChinese(const std::string& str);
  static bool isEnglish(const std::string& str);
};