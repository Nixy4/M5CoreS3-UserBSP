#include "launcher.hpp"



static const char* TAG = "Launcher";
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
}

void Launcher::onDestroy()
{
  ESP_LOGI(TAG, "onDestroy");
}

void Launcher::onRunning()
{
  // ESP_LOGI(TAG, "onRunning");
}

void Launcher::appAdd(string name, int id)
{
  app a;
  a.name = name;
  a.id = id;
  apps.push_back(a);
}

void Launcher::appDelete(int id)
{
  for(int i=0;i<apps.size();i++)
  {
    app a = apps.at(i);
    if(a.id == id)
    {
      apps.erase(apps.begin() + i);
      break;
    }
  }
}

void Launcher::update()
{
  core.displayLock(0);//!
  lv_obj_t* scr = lv_scr_create_base();
  lv_obj_set_style_base_dir(scr, LV_BASE_DIR_NEUTRAL, 0);
  lv_obj_set_size(scr, LV_HOR_RES, LV_VER_RES);
  lv_obj_set_flex_flow(scr, LV_FLEX_FLOW_ROW_WRAP);
  lv_obj_set_flex_align(scr, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  
  lv_obj_t* botton;
  lv_obj_t* label;
  for(int i=0;i<apps.size();i++)
  {
    app a = apps.at(i);
    botton = lv_button_create(scr);
    lv_obj_set_flex_grow(botton, 1);
    label = lv_label_create(botton);
    lv_label_set_text(label, a.name.c_str() );
    lv_obj_center(label);
    lv_obj_add_event_cb(botton, [](lv_event_t* e) 
    {
      ESP_LOGI(TAG, "App Button Clicked");
    }, LV_EVENT_CLICKED, &a);
  }
  lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, 1);   
  core.displayUnlock();//!  
}