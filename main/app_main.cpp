#include "dirent.h"
#include "M5CoreS3.hpp"
#include "mooncake.h"
// #include "launcher.hpp"
#include "faceRecognition.hpp"

#include "human_face_detect.hpp"
#include "human_face_recognition.hpp"

using namespace std;
using namespace mooncake;

static const char* TAG = "MAIN";
static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();



extern "C" void app_main(void)
{
	//*Device
	core.i2cInit();
	core.displayInit();
	core.displayBrightnessOn();
	core.spiffsMount();

	// //*Middleware
	xTaskCreatePinnedToCore([](void* arg) 
	{
		vTaskDelay(2000);
		cake.createExtension(make_unique<FaceRecognition>());
		// cake.update();
		// cake.update();
		// cake.update();
		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}

	}, TAG , 1024*4, NULL, 5, NULL, APP_CPU_NUM);

}