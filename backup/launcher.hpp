#pragma once

#include "M5CoreS3.hpp"
#include "mooncake.h"

using namespace std;
using namespace mooncake;

class Launcher : public UIAbility
{
private:
    struct app
    {
        string name;
        int id;
    };
    vector<app> apps;

public:
    Launcher();
    ~Launcher();
    void onCreate() override;
    void onDestroy() override;
    void onRunning() override;
};