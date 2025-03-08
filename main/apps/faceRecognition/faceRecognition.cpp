#include "faceRecognition.hpp"

static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

FaceRecognition::FaceRecognition() : 
  _recognizer(&_feat, "/spiffs/face.db" , dl::recognition::DB_SPIFFS)
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

  //*初始化摄像头
  ESP_ERROR_CHECK( core.cameraInit() );

  //*设置图像参数,申请缓冲区
  core.displayLock(0);//!
  _img_dsc_lv.header.magic = LV_IMAGE_HEADER_MAGIC;
  _img_dsc_lv.header.cf = LV_COLOR_FORMAT_RGB565;
  _img_dsc_lv.header.flags = 0;
  _img_dsc_lv.header.w = 320;
  _img_dsc_lv.header.h = 240;
  _img_dsc_lv.header.stride = 320*2;
  _img_dsc_lv.data = (uint8_t*)heap_caps_malloc(320*240*2, MALLOC_CAP_SPIRAM| MALLOC_CAP_8BIT);
  assert(_img_dsc_lv.data);
  //*创建图像对象
  lv_obj_t* scr = lv_scr_act();
  _img_lv = lv_img_create(scr);
  lv_obj_set_size(_img_lv, LV_HOR_RES, LV_VER_RES);
  lv_obj_center(_img_lv);
  lv_img_set_src(_img_lv, &_img_dsc_lv);
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
  camera_fb_t* fb = core.cameraFrameGet();//!
  if(fb==NULL) {
    ESP_LOGE(TAG, "cameraFrameGet failed");
    return;
  }
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
    ESP_LOGI(TAG, "detect_res.front().box: %d %d %d %d",
    detect_res.front().box[0],
    detect_res.front().box[1],
    detect_res.front().box[2],
    detect_res.front().box[3]);
  };
  core.displayLock(0);//!
  _img_dsc_lv.data_size = fb->len;
  memcpy((void*)_img_dsc_lv.data, fb->buf, fb->len);
  core.cameraFrameReturn(fb);//!
  uint16_swap((uint16_t*)_img_dsc_lv.data, 320, 240);
  lv_obj_invalidate(_img_lv);
  core.displayUnlock();//!
}