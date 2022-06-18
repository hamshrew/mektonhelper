#include "matedsystem.h"
#include "mektonfuncs.h"

MatedSystem::MatedSystem()
{
}

MatedSystem::~MatedSystem()
{

}

double MatedSystem::getBaseCost() const
{
    double tot = 0;

    SplittablePart* sp;

    foreach (sp,matedParts)
    {
        tot += sp->getBaseCost();
    }

    return tot;
}

double MatedSystem::getBaseSpace() const
{
    if (matedParts.size() == 1)
    {
        return matedParts.first()->getSpace();
    }
    double tot = 0;

    SplittablePart* sp;

    foreach (sp,matedParts)
    {
        tot += sp->getSpace();
    }

    return mekround(tot / 1.33);
}

bool MatedSystem::addPart(SplittablePart* part)
{
    if (matedParts.size() > 2 || part->isMated() || dynamic_cast<MatedSystem*>(part))
        return false;

    matedParts.append(part);
    part->setMate(this);
    return true;
}

bool MatedSystem::remPart(SplittablePart* part)
{
    if (!matedParts.contains(part))
        return false;

    part->setMate(0);
    matedParts.removeOne(part);
    return true;
}
