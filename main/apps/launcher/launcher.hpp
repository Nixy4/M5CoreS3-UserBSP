#pragma once

#include "M5CoreS3.hpp"
#include "mooncake.h"

using namespace std;
using namespace mooncake;

class Launcher : public UIAbility
{
private:
  lv_obj_t* _scr;
  int _id;
  int _appCount = 0;
public:
  Launcher();
  ~Launcher();

  void onCreate() override;
  void onDestroy() override;
  void onForeground() override;
  void onHide() override;
  void onBackground() override;
  void onShow() override;
  
  void setId(int id);
  int getId();
  int appRegister(unique_ptr<AppAbility> app);

private:
  static inline int appButtonGetCol(int id);
  static inline int appButtonGetRow(int id);
  void appButtonCreate(int appId);
  static bool isChinese(const std::string& str);
  static bool isEnglish(const std::string& str);
};