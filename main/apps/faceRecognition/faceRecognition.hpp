#pragma once

#include "M5CoreS3.hpp"
#include "human_face_detect.hpp"
#include "human_face_recognition.hpp"
#include "mooncake.h"

using namespace mooncake;

class FaceRecognition : public BasicAbility
{
private:
  const char* TAG = "FaceRecognition";
  sensor_t* _cameraHandle;

  HumanFaceDetect _detector;
  HumanFaceFeat _feat;
  HumanFaceRecognizer _recognizer;
  
  lv_img_dsc_t _img_dsc_lv;
  lv_obj_t* _img_lv;

  // esp_err_t faceAdd(const char* name);
  // esp_err_t faceDelete(const char* name);
  // esp_err_t facePrint();

public:
  FaceRecognition();
  ~FaceRecognition();
  void onCreate() override;
  void onDestroy() override;
  void onRunning() override;
};