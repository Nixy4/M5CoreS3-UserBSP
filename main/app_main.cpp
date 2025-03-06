//* std
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <iostream>
#include <thread>
#include <chrono>
//*SoC
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_log.h"
//*Peripherals

//*Device
#include "M5CoreS3.hpp"

//*Middleware
#include "lvgl.h"
#include "mooncake.h"
//*APP
#include "launcher.hpp"

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

	//*Middleware
	xTaskCreatePinnedToCore([](void* arg) 
	{
		const char* TAG = "Mooncake";

		cake.createExtension(make_unique<Launcher>());
		
		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}

	}, "Mooncake", 1024*4, NULL, 5, NULL, APP_CPU_NUM);

}