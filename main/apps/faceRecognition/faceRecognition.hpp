#pragma once

#include "M5CoreS3.hpp"
#include "mooncake.h"
#include "human_face_detect_mnp01.hpp"
#include "human_face_detect_msr01.hpp"
#include "face_recognition_112_v1_s8.hpp"

using namespace mooncake;

class FaceRecognition : public BasicAbility
{
private:
	const uint32_t  DL_OUT_W          = 320;
	const uint32_t  DL_OUT_H          = 240;
	const uint32_t  DL_RECOG_INTERVAL = 1000;

  sensor_t* _cameraHandle;

  lv_img_dsc_t _img_dsc_lv;
  lv_obj_t* _img_lv;

  volatile TickType_t _lastTick = 0;

	HumanFaceDetectMSR01 detector1;
	HumanFaceDetectMNP01 detector2;
	FaceRecognition112V1S8 recognizer;

public:
  FaceRecognition();
  ~FaceRecognition();
  void onCreate() override;
  void onDestroy() override;
  void onRunning() override;
};