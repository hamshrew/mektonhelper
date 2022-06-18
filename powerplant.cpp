#include "powerplant.h"

PowerPlant::PowerPlant():
    charge(0),
    hot(false),
    source(FUSION)
{
}

QString PowerPlant::getChargeName() const
{
    QString name;
    switch (charge)
    {
    case 0:
        name = "Standard";
        break;
    case 1:
        name = "Overcharged";
        break;
    case 2:
        name = "Supercharged";
        break;
    default:
        name = "Undercharged";
        break;
    }

    return name;
}

QString PowerPlant::getSourceName() const
{
    switch (source)
    {
    case FUSION: return "Fusion";
    case BIOENERGY: return "Bioenergy";
    case POWERCELL: return "Power Cell";
    case COMBUSTION: return "Combustion";
    }
    // Not needed but the compiler complains if it isn't here.
    return "Fusion";
}

double PowerPlant::getCostMult() const
{
    double sourceMod = 1.0;

    double baseMod = 0;

    switch (charge)
    {
    case 0:
        baseMod = hot ? -0.1: 0;
        break;
    case 1:
        baseMod = 0.15;
        break;
    case 2:
        baseMod = 0.3;
        break;
    default:
        baseMod = hot? -0.25: -0.15;
        break;
    }

    switch(source)
    {
    case BIOENERGY:
        sourceMod = 1.5;
        break;
    case POWERCELL:
        sourceMod = 0.85;
        break;
    case COMBUSTION:
        sourceMod = 0.67;
        break;
    default:
        sourceMod = 1.0;
        break;
    }

    double cost;

    if (baseMod)
        cost = baseMod * sourceMod;
    else cost = sourceMod - 1;

    return cost;
}
