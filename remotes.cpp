#include "remotes.h"
#include "drone.h"

Remotes::Remotes():
    size(0),
    range(0),
    storesRemotes(false),
    loc("None"),
    wired(false),
    eff(0)
{
}

Remotes::~Remotes()
{
    Drone* drone;
    foreach (drone,drones)
    {
        delete drone;
    }
}

double Remotes::getWeight() const
{
    if (!storesRemotes)
        return 0;
    double wt = 0;

    Drone* drone;

    foreach (drone,drones)
    {
        wt += drone->getWeight();
    }

    return wt;
}

double Remotes::getCostMult() const
{
    double mult;
    switch(range)
    {
    case 0:
        mult = 0.4;
        break;
    case 1:
        mult = 0.7;
        break;
    case 3:
        mult = 0.9;
        break;
    case 5:
        mult = 1.0;
        break;
    case 7:
        mult = 1.1;
        break;
    case 9:
        mult = 1.2;
        break;
    case 12:
        mult = 1.4;
        break;
    case 15:
        mult = 1.6;
        break;
    case 20:
        mult = 2.0;
        break;
    }

    if (wired)
        mult *= 0.6;

    return mult;
}

void Remotes::remDrone(int index)
{
    if (index < drones.size() && index >= 0)
    {
        delete drones[index];
        drones.removeAt(index);
    }
}

double Remotes::getDroneCost() const
{
    double droneCost = 0;

    for (QList<Drone*>::const_iterator iter = drones.begin();iter != drones.end();++iter)
    {
        droneCost += (*iter)->getCost();
    }

    return droneCost;
}
