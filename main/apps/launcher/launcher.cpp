#include "launcher.hpp"

#define TAG "Launcher"
static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

Launcher::Launcher()
{
  //如果是开机重启,则播放开机动画
  esp_reset_reason_t reason = esp_reset_reason();
  if(reason == ESP_RST_POWERON)
  {
    core.displayLock(0);//!
    lv_obj_set_style_bg_color(lv_scr_act(), {60,60,60}, 0);
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label, {0xFF,0xFF,0xFF}, 0);
    lv_label_set_text(label, "M5Stack Core S3");
    lv_obj_center(label);
    core.displayUnlock();//!
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
  core.displayLock(0);//!
  _scr = lv_scr_create_base();
  lv_obj_set_style_bg_color(_scr, {0,0,0}, 0);
  lv_obj_set_style_base_dir(_scr, LV_BASE_DIR_NEUTRAL, 0);
  lv_obj_set_size(_scr, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_flex_flow(_scr, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_flex_align(_scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); 
  lv_scr_load_anim(_scr, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, true);
  core.displayUnlock();//!

  esp_free_heap_print(TAG "1");
}

Launcher::~Launcher()
{

}

void Launcher::onCreate()
{

}

void Launcher::onDestroy()
{

}

void Launcher::onForeground()
{
  
}

void Launcher::onHide()
{

}

void Launcher::onBackground()
{

}

void Launcher::onShow()
{
  lv_scr_load_anim(_scr, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, 0);
  esp_free_heap_print(TAG "2");
}

void Launcher::appButtonCreate(int id)
{
  core.displayLock(0);//!
  lv_obj_t* button = lv_btn_create(_scr);
  lv_obj_set_flex_grow(button, 1);
  lv_obj_set_size(button, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  AppAbility::AppInfo_t info = cake.getAppInfo(id);
  //设置按钮图标
  if(info.icon == nullptr)
  {
    lv_obj_t* label = lv_label_create(button);
    char buf[5] = {0};
    if(isChinese(info.name))
    {
      strncpy(buf, info.name.c_str(), 4);
    }
    else if(isEnglish(info.name))
    {
      strncpy(buf, info.name.c_str(), 1);
    }
    else
    {
      strncpy(buf, info.name.c_str(), 1);
    }
    lv_label_set_text(label, buf);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label); 
  }
  else
  {
    lv_obj_t* img = lv_img_create(button);
    lv_img_set_src(img, info.icon);
    lv_obj_center(img);
  }
  //设置APP名称
  lv_obj_t* label = lv_label_create(button);
  lv_label_set_text(label, info.name.c_str());
  lv_obj_set_style_text_font(label, &lv_font_montserrat_12, 0);
  lv_obj_align_to(label, button, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
  core.displayUnlock();//!
}

int Launcher::appRegister(unique_ptr<AppAbility> appAbility)
{
  esp_free_heap_print(TAG "3");
  int id = cake.installApp( move(appAbility) );
  esp_free_heap_print(TAG "4");
  appButtonCreate(id);
  esp_free_heap_print(TAG" 5");
  return id;
}

bool Launcher::isChinese(const std::string& str) 
{
  for (size_t i = 0; i < str.size(); ++i) {
  unsigned char c = str[i];
  // 检查是否为中文字符（UTF-8 编码）
  if (c >= 0xE4 && c <= 0xE9) {
      if (i + 2 < str.size() && (unsigned char)str[i + 1] >= 0x80 && (unsigned char)str[i + 1] <= 0xBF &&
        (unsigned char)str[i + 2] >= 0x80 && (unsigned char)str[i + 2] <= 0xBF) {
        return true;
      }
    }
  }
  return false;
}

bool Launcher::isEnglish(const std::string& str) 
{
  for (char c : str) {
    // 检查是否为英文字符（ASCII 编码）
    if (c >= 0x00 && c <= 0x7F) {
      return true;
    }
  }
  return false;
}