#include "sensorcomp.h"

SensorComp::SensorComp()
{
    senseCostMap["Superlight"] = 0;
    senseCostMap["Light Weight"] = 1;
    senseCostMap["Striker"] = 2;
    senseCostMap["Medium Striker"] = 4;
    senseCostMap["Heavy Striker"] = 6;
    senseCostMap["Medium Weight"] = 9;
    senseCostMap["Light Heavy"] = 12;
    senseCostMap["Medium Heavy"] = 16;
    senseCostMap["Armored Heavy"] = 22;
    senseCostMap["Super Heavy"] = 27;
    senseCostMap["Mega Heavy"] = 32;
    senseCostMap["Backup"] = 2;

    senseKillMap["Superlight"] = 0;
    senseKillMap["Light Weight"] = 1;
    senseKillMap["Striker"] = 1;
    senseKillMap["Medium Striker"] = 1;
    senseKillMap["Heavy Striker"] = 2;
    senseKillMap["Medium Weight"] = 2;
    senseKillMap["Light Heavy"] = 2;
    senseKillMap["Medium Heavy"] = 3;
    senseKillMap["Armored Heavy"] = 3;
    senseKillMap["Super Heavy"] = 3;
    senseKillMap["Mega Heavy"] = 4;
    senseKillMap["Backup"] = 2;

    senseRangeMap["Superlight"] = "1 km";
    senseRangeMap["Light Weight"] = "2 km";
    senseRangeMap["Striker"] = "4 km";
    senseRangeMap["Medium Striker"] = "7 km";
    senseRangeMap["Heavy Striker"] = "11 km";
    senseRangeMap["Medium Weight"] = "15 km";
    senseRangeMap["Light Heavy"] = "20 km";
    senseRangeMap["Medium Heavy"] = "26 km";
    senseRangeMap["Armored Heavy"] = "30 km";
    senseRangeMap["Super Heavy"] = "40 km";
    senseRangeMap["Mega Heavy"] = "50 km";
    senseRangeMap["Backup"] = "1 km";

    senseCommMap["Superlight"] = "300 km";
    senseCommMap["Light Weight"] = "500 km";
    senseCommMap["Striker"] = "800 km";
    senseCommMap["Medium Striker"] = "1000 km";
    senseCommMap["Heavy Striker"] = "1300 km";
    senseCommMap["Medium Weight"] = "1500 km";
    senseCommMap["Light Heavy"] = "1800 km";
    senseCommMap["Medium Heavy"] = "2300 km";
    senseCommMap["Armored Heavy"] = "3000 km";
    senseCommMap["Super Heavy"] = "5000 km";
    senseCommMap["Mega Heavy"] = "Planetary";
    senseCommMap["Backup"] = "300 km";
}

SensorComp::~SensorComp()
{
}

SensorComp* SensorComp::instance()
{
    static SensorComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new SensorComp();
    return _instance;
}
