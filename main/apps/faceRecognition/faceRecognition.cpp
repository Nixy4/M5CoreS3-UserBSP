#include "faceRecognition.hpp"

static const char* TAG = "FaceRecognition";
static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

FaceRecognition::FaceRecognition() :
  detector1(0.3F,0.3F,10,0.3F), detector2(0.4F,0.3F,10), recognizer()
{
  ESP_LOGI(TAG, "FaceRecognition()");
	recognizer.set_partition(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "face");
	recognizer.set_ids_from_flash();
}

FaceRecognition::~FaceRecognition()
{
  ESP_LOGI(TAG, "~FaceRecognition()");
}

void FaceRecognition::onCreate()
{
  ESP_LOGI(TAG, "onCreate");
  ESP_ERROR_CHECK( core.cameraInit() );
  core.displayLock(0);//!
  _img_dsc_lv.header.magic = LV_IMAGE_HEADER_MAGIC;
  _img_dsc_lv.header.cf = LV_COLOR_FORMAT_RGB565;
  _img_dsc_lv.header.flags = 0;
  _img_dsc_lv.data = (uint8_t*)heap_caps_malloc(320*240*2, MALLOC_CAP_SPIRAM| MALLOC_CAP_8BIT);
  assert(_img_dsc_lv.data);
  _img_lv = lv_img_create(lv_scr_act());
  core.displayUnlock();//!
}

void FaceRecognition::onDestroy()
{
  ESP_LOGI(TAG, "onDestroy");
  ESP_ERROR_CHECK( core.cameraDeinit() );
  heap_caps_free((void*)_img_dsc_lv.data);
}

void FaceRecognition::onRunning()
{
  ESP_LOGI(TAG, "onRunning");
  camera_fb_t* fb = core.cameraFrameGet();//!
  if(fb == NULL) {
    ESP_LOGE(TAG, "Camera capture failed");
    return;
  }
  core.displayLock(0);//!
  _img_dsc_lv.header.w = fb->width;
  _img_dsc_lv.header.h = fb->height;
  _img_dsc_lv.header.stride = fb->width*2;
  _img_dsc_lv.data_size = fb->len;
  memcpy((void*)_img_dsc_lv.data, fb->buf, fb->len);
  core.cameraFrameReturn(fb);//!
  lv_obj_invalidate(_img_lv);
  core.displayUnlock();//!
}