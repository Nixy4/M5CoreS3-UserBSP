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
static Launcher& launcher;

class test : public AppAbility
{
private:
	lv_obj_t* _scr;
public:

	int id_self = 0;
	static int count;

	test() 
	{
		ESP_LOGI(TAG, "test constructor");
		id_self = count++;
		setAppInfo().name = "test" + to_string(id_self);
		LV_IMG_DECLARE(human_handsup);
		setAppInfo().icon = (void*)&human_handsup;
	}

	~test() 
	{

	}

	void onCreate() override
	{
		ESP_LOGI(TAG, "test onCreate");
		_scr = lv_scr_create_base();
		lv_obj_t* label = lv_label_create(_scr);
		lv_label_set_text(label, getAppInfo().name.c_str());
		lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
		lv_obj_center(label);
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
		core.displayLock(0);//!
		lv_scr_load_anim(_scr, LV_SCR_LOAD_ANIM_FADE_IN, 300, 0, 0);
		core.displayUnlock();//!
	}

	void onClose() override
	{
		ESP_LOGI(TAG, "test onClose");
		cake.extensionManager()->showUIAbility(0);
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
		launcher = *cake.getExtensionInstance<Launcher>( launcher_id );
		launcher.setId(launcher_id);
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		launcher.appRegister(make_unique<test>());
		cake.extensionManager()->showUIAbility(launcher_id);
		while(1)
		{
			cake.update();
			vTaskDelay(1);
		}
	}, TAG , 1024*4, NULL, 5, NULL, APP_CPU_NUM);

}