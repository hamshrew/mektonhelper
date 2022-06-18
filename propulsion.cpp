#include "propulsion.h"
#include "mektonfuncs.h"

#include <QStringList>

Propulsion::Propulsion(const QString& type):
    type(type),
    rating(0)
{
}

void Propulsion::setSpaces(const QString &loc, double spaces)
{
    if (spaceMap.contains(loc) && (spaces == 0.0))
    {
        spaceMap.remove(loc);
        return;
    }

    spaceMap[loc] = spaces;
}

double Propulsion::getSpaces(const QString &loc) const
{
    if (!spaceMap.contains(loc))
        return 0;

    return spaceMap[loc];
}

double Propulsion::getAllocSpace() const
{
    double space = 0;

    double temp;

    foreach (temp,spaceMap)
    {
        space += temp;
    }

    return space;
}

void Propulsion::normalize(double wt)
{
    double totalSpace = getTotalSpace(wt);
    // check for too much efficiency
    if (totalSpace < 0)
    {
        eff = eff + totalSpace;
        totalSpace = 0;
    }

    if (totalSpace >= getAllocSpace())
        return;

    bool tooMuch = false;

    double total = 0;

    QString temp;

    foreach (temp,spaceMap.keys())
    {
        if (tooMuch)
        {
            spaceMap[temp] = 0;
            continue;
        }

        if ((spaceMap[temp] + total) > totalSpace)
        {
            spaceMap[temp] = totalSpace - total;
            tooMuch = true;            
        }
        total += spaceMap[temp];
    }

    if (!tooMuch)
        return;

    // clean any empties
    QMap<QString,double>::iterator it = spaceMap.begin();

    while (it != spaceMap.end())
    {
        if (*it == 0.0)
        {
            it = spaceMap.erase(it);
            continue;
        }
        ++it;
    }
}

void Propulsion::cleanLocs(const QStringList &valid)
{
    QMap<QString,double>::iterator it = spaceMap.begin();

    while (it != spaceMap.end())
    {
        if (!valid.contains(it.key()))
        {
            it = spaceMap.erase(it);
            continue;
        }
        ++it;
    }
}

double Propulsion::getTotalSpace(double wt) const
{
    return mekround(rating*wt*PropComp::instance()->calcCostMul(type)) - eff;
}
