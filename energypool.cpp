#include "energypool.h"
#include "weapon.h"
#include "shield.h"

EnergyPool::EnergyPool():
    rating(0),
    portfolio(3),
    morphable(false)
{
}

EnergyPool::~EnergyPool()
{
    Weapon* wp;
    foreach (wp,weapons)
    {
        delete wp;
    }

    Shield* sh;
    foreach (sh,shields)
    {
        delete sh;
    }
}

int EnergyPool::getMaxPower() const
{
    if (!rating)
        return 50;

    if (rating == 1)
        return 25;

    return rating*5 + 30;
}

int EnergyPool::getKills() const
{
    switch (rating)
    {
    case 1: return 4;
    case 2: return 7;
    case 3: return 8;
    case 4: return 10;
    case 5: return 11;
    case 6: return 13;
    default: return 5;
    }

    // Shut up the compiler
    return 5;
}

double EnergyPool::getBaseCost() const
{
    double cost = rating*10;
    if (!cost)
        cost = 10;

    if (morphable)
        cost *= 1.25;

    switch (portfolio)
    {
    case 1: cost *= 0.6;
        break;
    case 2: cost *= 0.75;
        break;
    case 3: cost *= 1.0;
        break;
    case 4: cost *= 1.2;
        break;
    case 5: cost *= 1.3;
        break;
    case 7: cost *= 1.4;
        break;
    case 10: cost *= 1.5;
        break;
    default: cost *= 2.0;
        break;
    }

    return cost;
}

void EnergyPool::remWeapon(int index)
{
    if (index >= 0 && index < weapons.size())
    {
        delete weapons[index];
        weapons.removeAt(index);
    }
}

void EnergyPool::remShield(int index)
{
    if (index >= 0 && index < shields.size())
    {
        delete shields[index];
        shields.removeAt(index);
    }
}

void EnergyPool::addWeapon(Weapon *wp)
{
    wp->setLoc(loc);
    weapons.push_back(wp);
}

void EnergyPool::addShield(Shield *sh)
{
    sh->setLoc(loc);
    shields.push_back(sh);
}

void EnergyPool::setLoc(const QString &newVal)
{
    loc = newVal;

    Weapon* wp;
    foreach (wp,weapons)
    {
        wp->setLoc(newVal);
    }

    Shield* sh;
    foreach (sh,shields)
    {
        sh->setLoc(newVal);
    }
}
