#pragma once

#include "esp_log.h"
#include "esp_camera.h"
#include "bsp/esp-bsp.h"
#include "SensorLTR553.hpp"

class M5CoreS3
{
private:
  const char* TAG = "M5CoreS3";
  M5CoreS3() { }
  ~M5CoreS3() { }
public:
  static M5CoreS3& getInstance() 
  {
    static M5CoreS3 incetance;
    return incetance;
  } 
  M5CoreS3(const M5CoreS3&) = delete;
  M5CoreS3& operator = (const M5CoreS3&) = delete;

	i2c_master_bus_handle_t _i2cMasterBusHandle  = NULL;
	i2c_master_dev_handle_t _i2cDevHandleAXP2101 = NULL;
	i2c_master_dev_handle_t _i2cDevHandleAW9523  = NULL;
 	lv_disp_t*              _display             = NULL;
	sensor_t*               _camera              = NULL;
	SensorLTR553 _alSensor; // Ambient Light Sensor

	/*Audio*/
  esp_err_t audioInit(const i2s_std_config_t* i2s_config);//*original
  const audio_codec_data_if_t* audioGetCodecItf();//*original
  esp_codec_dev_handle_t audioCodecSpeakerInit();//*original
  esp_codec_dev_handle_t audioCodecMicrophoneInit();//*original

	/*I2C*/
  esp_err_t i2cInit();
  esp_err_t i2cDeinit();//*original

	/*SPIFFS*/
  esp_err_t spiffsMount();//*original
  esp_err_t spiffsUnmount();//*original

	/*SD Card*/
  esp_err_t sdcardMount();//*original
  esp_err_t sdcardUnmount();//*original

	/*Display*/
	lv_disp_t* displayInit();
  esp_err_t displayBrightnessOn();//*original
  esp_err_t displayBrightnessOff();//*original
  esp_err_t displayBrightnessSet(uint8_t brightness);//*original
  esp_err_t displayLock(uint32_t ms);//*original
  void displayUnlock();//*original
  void displayRotate(lv_disp_t* disp, lv_display_rotation_t rotation);//*original

	/*Camera*/
  esp_err_t cameraInit();
  esp_err_t cameraDeinit();
  camera_fb_t* cameraFrameGet();
  void cameraFrameReturn(camera_fb_t* fb);
  sensor_t* cameraSensorGet();

	/*Ambient Light Sensor*/
  esp_err_t alSensorInit();
  void alSensorDeinit();
  void alSensorStart();
  int alSensorGetLight(int channel);
  int alSensorGetProximity(bool* saturated);
};