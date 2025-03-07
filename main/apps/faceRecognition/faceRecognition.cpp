#include "faceRecognition.hpp"

static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

FaceRecognition::FaceRecognition() : _recognizer(&_feat, "/spiffs/face.db" , dl::recognition::DB_SPIFFS)
{
  ESP_LOGI(TAG, "FaceRecognition()");
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
  if(fb!=NULL) {
    const dl::image::img_t &img = {
      .data = fb->buf,
      .width = static_cast<int>(fb->width),
      .height = static_cast<int>(fb->height),
      .pix_type = dl::image::DL_IMAGE_PIX_TYPE_RGB565
    };
    std::list<dl::detect::result_t> &detect_res = _detector.run(img);
    if( detect_res.size() ) {
      dl::image::draw_hollow_rectangle (img, 
      detect_res.front().box[0], 
      detect_res.front().box[1],
      detect_res.front().box[2],
      detect_res.front().box[3],
      {255,0,0}, 2);
    };
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