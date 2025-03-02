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

M5CoreS3& core = M5CoreS3::getInstance();
Mooncake cake;

extern "C" void app_main(void)
{
	core.i2cInit();
	core.displayInit();
	core.displayBrightnessOn();	
  //打印core的地址
  ESP_LOGI(TAG, "core address: %p", &core);

	xTaskCreatePinnedToCore([](void* arg) 
	{
		const char* TAG = "Mooncake";

		int launcher = cake.installApp(make_unique<Launcher>());

		ESP_LOGI(TAG, "launcher APP ID: %d", launcher);
		cake.openApp(launcher);
		ESP_LOGI(TAG, "launcher APP State: %d", cake.getAppCurrentState(launcher));
		
		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}
	}, "mooncake", 1024*8, NULL, 5, NULL, APP_CPU_NUM);

}