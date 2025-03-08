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

class test : public AppAbility
{
public:

	int id_self = 0;
	static int count;

	test() 
	{
		ESP_LOGI(TAG, "test constructor");
		id_self = count++;
		setAppInfo().name = "test" + to_string(id_self);
	}

	~test() {}

	void onCreate() override
	{
		ESP_LOGI(TAG, "test onCreate");
	}

	void onSleeping() override
	{
		// ESP_LOGI(TAG, "test onSleeping");
	}

	void onRunning() override
	{
		// ESP_LOGI(TAG, "test onRunning");
	}

	void onOpen() override
	{
		ESP_LOGI(TAG, "test onOpen");
	}

	void onClose() override
	{
		ESP_LOGI(TAG, "test onClose");
	}

	void onDestroy() override
	{
		ESP_LOGI(TAG, "test onDestory");
	}
};

int test::count = 0;

extern "C" void app_main(void)
{
	//*Device
	core.i2cInit();
	core.spiffsMount();
	
	core.displayInit();
	core.displayBrightnessOn();

	// //*Middleware
	xTaskCreatePinnedToCore([](void* arg) 
	{
		int launcher_id = cake.createExtension(make_unique<Launcher>());
		Launcher* launcher_handle =	cake.getExtensionInstance<Launcher>( launcher_id );
		launcher_handle->appRegister(make_unique<test>());
		launcher_handle->appRegister(make_unique<test>());
		launcher_handle->appRegister(make_unique<test>());
		launcher_handle->appRegister(make_unique<test>());
		cake.extensionManager()->showUIAbility(launcher_id);
		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}
	}, TAG , 1024*4, NULL, 5, NULL, APP_CPU_NUM);

}