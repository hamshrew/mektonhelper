#include "propcomp.h"

PropComp::PropComp()
{
    costMulMap["GES"] = 0.025;
    costMulMap["HydroJets"] = 0.025;
    costMulMap["HydroThrusters"] = 0.0375;
    costMulMap["MagnetoHydroDynamics"] = 0.0375;
    costMulMap["Thrusters"] = 0.0375;
    costMulMap["Gravitics"] = 0.05;

    moveModeMap["GES"] = "Ground";
    moveModeMap["HydroJets"] = "Water";
    moveModeMap["HydroThrusters"] = "Water";
    moveModeMap["MagnetoHydroDynamics"] = "Water";
    moveModeMap["Thrusters"] = "Air";
    moveModeMap["Gravitics"] = "Air";

    fuelMap["GES"] = true;
    fuelMap["HydroJets"] = true;
    fuelMap["HydroThrusters"] = true;
    fuelMap["MagnetoHydroDynamics"] = false;
    fuelMap["Thrusters"] = true;
    fuelMap["Gravitics"] = false;
}

PropComp::~PropComp()
{
}

PropComp* PropComp::instance()
{
    static PropComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new PropComp();
    return _instance;
}
