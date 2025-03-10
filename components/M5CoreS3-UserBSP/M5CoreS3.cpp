#include "M5CoreS3.hpp"

static const char* TAG = "M5CoreS3";

esp_err_t M5CoreS3::audioInit(const i2s_std_config_t* i2s_config) 
{ 
  return bsp_audio_init(i2s_config); 
}

const audio_codec_data_if_t* M5CoreS3::audioGetCodecItf() 
{ 
  return bsp_audio_get_codec_itf(); 
}

esp_codec_dev_handle_t M5CoreS3::audioCodecSpeakerInit() 
{ 
  return bsp_audio_codec_speaker_init(); 
}

esp_codec_dev_handle_t M5CoreS3::audioCodecMicrophoneInit() 
{ 
  return bsp_audio_codec_microphone_init(); 
}

esp_err_t M5CoreS3::i2cInit() 
{ 
  esp_err_t err = bsp_i2c_init();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "I2C init failed with error 0x%x", err);
    return err;
  }
  _i2cMasterBusHandle  = bsp_i2c_get_bus_handle();
  _i2cDevHandleAXP2101 = bsp_i2c_get_dev_handle_axp2101();
  _i2cDevHandleAW9523  = bsp_i2c_get_dev_handle_aw9523();
  // _touch               = bsp_touch_get_handle();
  assert(_i2cMasterBusHandle != NULL);
  assert(_i2cDevHandleAXP2101 != NULL);
  assert(_i2cDevHandleAW9523 != NULL);
  // assert(_touch != NULL);
  return err;
}

esp_err_t M5CoreS3::i2cDeinit() 
{ 
  return bsp_i2c_deinit(); 
}

esp_err_t M5CoreS3::spiffsMount() 
{ 
  return bsp_spiffs_mount(); 
}

esp_err_t M5CoreS3::spiffsUnmount() 
{ 
  return bsp_spiffs_unmount(); 
}

esp_err_t M5CoreS3::sdcardMount() 
{ 
  return bsp_sdcard_mount(); 
}

esp_err_t M5CoreS3::sdcardUnmount() 
{ 
  return bsp_sdcard_unmount(); 
}

esp_err_t M5CoreS3::displayBrightnessOn() 
{ 
  return bsp_display_backlight_on(); 
}

esp_err_t M5CoreS3::displayBrightnessOff() 
{ 
  return bsp_display_backlight_off(); 
}

esp_err_t M5CoreS3::displayBrightnessSet(uint8_t brightness) 
{ 
  return bsp_display_brightness_set(brightness); 
}

esp_err_t M5CoreS3::displayLock(uint32_t ms) 
{ 
  return bsp_display_lock(ms); 
}

void M5CoreS3::displayUnlock() 
{ 
  return bsp_display_unlock(); 
}

void M5CoreS3::displayRotate(lv_disp_t* disp, lv_display_rotation_t rotation) 
{ 
  return bsp_display_rotate(disp, rotation); 
}

lv_disp_t* M5CoreS3::displayInit() 
{ 
  const bsp_display_cfg_t cfg = {
    .lvgl_port_cfg =     { 
      .task_priority     = 4,
      .task_stack        = 7168,
      .task_affinity     = -1,
      .task_max_sleep_ms = 500,
      .timer_period_ms   = 5,
    },
    .buffer_size = 320 * 240,
    .double_buffer = false,
    .flags = {
      .buff_dma = true,
      .buff_spiram = false
    }
  };
  return bsp_display_start_with_config(&cfg);
}

esp_err_t M5CoreS3::cameraInit() 
{
  const camera_config_t cfg = BSP_CAMERA_DEFAULT_CONFIG;
  esp_err_t err = esp_camera_init(&cfg);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
    return err;
  }
  _camera = esp_camera_sensor_get();
  assert(_camera != NULL);
  return ESP_OK;
}

esp_err_t M5CoreS3::cameraDeinit() 
{ 
  return esp_camera_deinit(); 
}

camera_fb_t* M5CoreS3::cameraFrameGet() 
{ 
  return esp_camera_fb_get(); 
}

void M5CoreS3::cameraFrameReturn(camera_fb_t* fb) 
{ 
  esp_camera_fb_return(fb); 
}

sensor_t* M5CoreS3::cameraSensorGet() 
{ 
  return esp_camera_sensor_get(); 
}

esp_err_t M5CoreS3::alSensorInit() 
{
  if (_alSensor.begin(_i2cMasterBusHandle, LTR553_SLAVE_ADDRESS) != true) {
    ESP_LOGE(TAG, "LTR553 init failed");
    return ESP_FAIL;
  }
  return ESP_OK;
}

void M5CoreS3::alSensorDeinit() 
{ 
  _alSensor.deinit(); 
}

void M5CoreS3::alSensorStart() 
{
  _alSensor.setLightSensorThreshold(10, 200);
  _alSensor.setProximityThreshold(10, 30);
  _alSensor.setLightSensorPersists(5);
  _alSensor.setProximityPersists(5);
  _alSensor.setIRQLevel(SensorLTR553::ALS_IRQ_ACTIVE_LOW);
  _alSensor.enableIRQ(SensorLTR553::ALS_IRQ_BOTH);
  _alSensor.setLightSensorGain(SensorLTR553::ALS_GAIN_8X);
  _alSensor.setPsLedPulsePeriod(SensorLTR553::PS_LED_PLUSE_100KHZ);
  _alSensor.setPsLedDutyCycle(SensorLTR553::PS_LED_DUTY_100);
  _alSensor.setPsLedCurrent(SensorLTR553::PS_LED_CUR_50MA);
  _alSensor.setProximityRate(SensorLTR553::PS_MEAS_RATE_200MS);
  _alSensor.setPsLedPulses(1);
  _alSensor.enablePsIndicator();
  _alSensor.enableLightSensor();
  _alSensor.enableProximity();
}

int M5CoreS3::alSensorGetLight(int channel) 
{ 
  return _alSensor.getLightSensor(channel); 
}

int M5CoreS3::alSensorGetProximity(bool* saturated) 
{ 
  return _alSensor.getProximity(saturated); 
}

void esp_free_heap_print(const char* TAG)
{
	ESP_LOGE(TAG, "Free Heap: IRAM [%d:%d] SPIRAM [%d:%d]", 
		heap_caps_get_free_size(MALLOC_CAP_INTERNAL), heap_caps_get_minimum_free_size(MALLOC_CAP_INTERNAL),
		heap_caps_get_free_size(MALLOC_CAP_SPIRAM), heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
}

lv_obj_t* lv_scr_create_base()
{
  lv_obj_t* scr = lv_obj_create(NULL);
  lv_obj_set_size(scr, LV_HOR_RES, LV_VER_RES);
  return scr;
}

void uint16_swap(uint16_t* buf, size_t w, size_t h)
{
	for (size_t i = 0; i < w * h; i++) {
		buf[i] = (buf[i] >> 8) | (buf[i] << 8);
	}
}

void list_dir(const char *path) {
	DIR *dir = opendir(path);
	if (dir == NULL) {
			ESP_LOGE(TAG, "Failed to open directory: %s", path);
			return;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL) {
			ESP_LOGI(TAG, "Found %s", entry->d_name);
	}
	closedir(dir);
}

