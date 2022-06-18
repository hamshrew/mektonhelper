#include "splittablepart.h"
#include "matedsystem.h"

SplittablePart::SplittablePart():
    loc("None"),
    eff(0),
    mated(0)
{
}

SplittablePart::~SplittablePart()
{
    if (isMated())
    {
        mated->remPart(this);
    }
}

double SplittablePart::getAllocSpace() const
{
    if (loc == "None" || isMated())
        return 0;

    if (!isSplit())
        return getSpace();

    double total = 0;

    double s;
    foreach (s,splitMap)
    {
        total += s;
    }

    return total;
}

double SplittablePart::getAllocSpace(const QString &ser) const
{
    if (isMated())
        return 0;

    if (ser == loc)
        return getSpace();

    if (!isSplit())
        return 0;

    if (splitMap.contains(ser))
        return splitMap[ser];

    return 0;
}

void SplittablePart::setAlloc(const QString &spot, double sp)
{
    if (sp == 0 && splitMap.contains(spot))
    {
        splitMap.remove(spot);
        return;
    }

    if (getAllocSpace() > getSpace())
        return;

    splitMap[spot] = sp;

    if (getAllocSpace() > getSpace())
    {
        splitMap[spot] = sp - (getAllocSpace() - getSpace());
    }    
}

void SplittablePart::normalize(const QStringList &locList)
{
    QMap<QString,double>::iterator it = splitMap.begin();

    double total = 0;
    double sp = getSpace();

    while (it != splitMap.end())
    {
        if (!locList.contains(it.key()))
        {
            it = splitMap.erase(it);
            continue;
        }

        if (total + it.value() > sp)
            it.value() = sp - total;

        total += it.value();

        ++it;
    }
}
