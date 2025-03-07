#pragma once

#include "M5CoreS3.hpp"
#include "mooncake.h"

using namespace std;
using namespace mooncake;

class Launcher : public BasicAbility
{
private:
    struct app
    {
        string name;
        int id;
    };
    vector<app> apps;

public:
    void onCreate() override;
    void onDestroy() override;
    void onRunning() override;

    void appAdd(string name, int id);
    void appDelete(int id);
    void update();
};