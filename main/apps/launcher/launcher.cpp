#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "lvgl.h"
#include "mooncake.h"
#include "launcher.hpp"

using namespace std;

static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

void Launcher::onCreate()
{
  //如果是开机重启,则播放开机动画
  esp_reset_reason_t reason = esp_reset_reason();
  if(reason == ESP_RST_POWERON)
  {
    ESP_LOGI(TAG, "onBoot");
    core.displayLock(0);//!
    lv_obj_set_style_bg_color(lv_scr_act(), {60,60,60}, 0);
    lv_obj_t* label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(label, {0xFF,0xFF,0xFF}, 0);
    lv_label_set_text(label, "M5Stack Core S3");
    lv_obj_center(label);
    core.displayUnlock();//!
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
  core.displayLock(0);//!
  lv_obj_t* scr = lv_obj_create(NULL);
  lv_obj_set_size(scr, LV_HOR_RES, LV_VER_RES);
  lv_obj_t* app = lv_button_create(scr);
  lv_obj_set_size(app, 50, 50);
  lv_obj_center(app);
  lv_obj_t* label = lv_label_create(app);
  lv_label_set_text(label, "App");
  lv_obj_center(label);
  lv_obj_add_event_cb(app, [](lv_event_t* e) 
  {
    ESP_LOGI("Launcher", "App Clicked");
  }, LV_EVENT_CLICKED, NULL);
  lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_OVER_BOTTOM, 500, 0, 1);
  core.displayUnlock();//!
}

void Launcher::onDestroy()
{
  ESP_LOGI(TAG, "onDestroy");
}

void Launcher::onRunning()
{
  // ESP_LOGI(TAG, "onRunning");
}