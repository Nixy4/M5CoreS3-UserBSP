#include "dirent.h"
#include "M5CoreS3.hpp"
#include "mooncake.h"
#include "launcher.hpp"
#include "faceRecognition.hpp"

using namespace std;
using namespace mooncake;

static const char* TAG = "MAIN";
static M5CoreS3& core = M5CoreS3::getInstance();
static Mooncake& cake = GetMooncake();

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

extern "C" void app_main(void)
{
	//*Device
	core.i2cInit();
	core.displayInit();
	core.displayBrightnessOn();
	// core.spiffsMount();
	// list_dir("/spiffs");
	
	//*Middleware
	xTaskCreatePinnedToCore([](void* arg) 
	{
		int id;
		id = cake.createExtension(make_unique<Launcher>());
		Launcher* launcher = cake.getExtensionInstance<Launcher>(id);
		ESP_LOGI(TAG, "Launcher ID: %d", id);

		id = cake.createExtension(make_unique<FaceRecognition>());
		launcher->appAdd("FaceRecognition", id);
		launcher->update();

		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}

	}, TAG , 1024*4, NULL, 5, NULL, APP_CPU_NUM);

}