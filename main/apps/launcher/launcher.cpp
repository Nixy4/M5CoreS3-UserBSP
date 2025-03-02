#include "launcher.hpp"

// extern const uint16_t image_data_logo[32400];
extern const lv_image_dsc_t lv_img_logo;

Launcher::Launcher()
{
  //打印core的地址
  ESP_LOGI(TAG, "core address: %p", &core);
}

void Launcher::onSleeping() 
{
  
}

void Launcher::onOpen()
{
  core.displayLock(0);//!
  _lvScr = lv_obj_create(NULL);
  lv_obj_set_size(_lvScr, LV_HOR_RES, LV_VER_RES);
  lv_obj_t* img = lv_img_create(_lvScr);
  lv_img_set_src(img, &lv_img_logo);
  lv_obj_center(img);
  lv_scr_load_anim(_lvScr, _lvBootAnim, 1000, 0, true);
  core.displayUnlock();//!
  vTaskDelay(2500 / portTICK_PERIOD_MS);
  
  core.displayLock(0);//!
  _lvScr = lv_obj_create(NULL);
  lv_obj_set_size(_lvScr, LV_HOR_RES, LV_VER_RES);
  lv_obj_t* label = lv_label_create(_lvScr);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_16,0);
  lv_label_set_text(label, "Hello, World!");
  lv_obj_center(label);
  lv_scr_load_anim(_lvScr, _lvBootAnim, 500, 0, true);
  core.displayUnlock();//!
}

void Launcher::onRunning()
{
  ESP_LOGI(TAG, "onRunning");
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void Launcher::onClose()
{
  
}

Launcher::~Launcher()
{

}