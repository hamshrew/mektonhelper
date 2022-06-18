#include "servo.h"

Servo::Servo():
    designation(""),
    type(TORSO),
    size(SUPERLIGHT),
    armorSize(SUPERLIGHT),
    armorType(NORMAL),
    ram(NONE),
    mounts(0),
    reinforce(0)
{
}

Servo::~Servo()
{

}

void Servo::setReinforce(int newVal)
{
    // Pods can't use reinforcement by definition
    if (!getKills())
    {
        return;
    }

    if ((newVal *2) > ServoComp::instance()->getSpace(size,type))
    {
        reinforce = ServoComp::instance()->getSpace(size,type) / 2;
    }
    else if (newVal <= (- ServoComp::instance()->getKills(size,type)))
    {
        reinforce = -ServoComp::instance()->getKills(size,type) + 1;
    }
    else {
        reinforce = newVal;
    }
}
