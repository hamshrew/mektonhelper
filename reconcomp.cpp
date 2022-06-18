#include "reconcomp.h"

ReconComp::ReconComp()
{
    costMap["Advanced Sensor Package"] = 10;
    costMap["Radio/Radar Analyzer"] = 5;
    costMap["Resolution Intensifiers"] = 0.5;
    costMap["Spotting Radar"] = 10;
    costMap["Target Analyzer"] = 5;
    costMap["Marine Suite"] = 2;
    costMap["Gravity Lens"] = 5;
    costMap["Magnetic Resonance Lens"] = 15;

    spaceMap["Advanced Sensor Package"] = 2;
    spaceMap["Radio/Radar Analyzer"] = 1;
    spaceMap["Resolution Intensifiers"] = 1;
    spaceMap["Spotting Radar"] = 5;
    spaceMap["Target Analyzer"] = 1;
    spaceMap["Marine Suite"] = 1;
    spaceMap["Gravity Lens"] = 1;
    spaceMap["Magnetic Resonance Lens"] = 1;
}

ReconComp::~ReconComp()
{
}

ReconComp* ReconComp::instance()
{
    static ReconComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new ReconComp();
    return _instance;
}
