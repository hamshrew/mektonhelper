#ifndef MATEDSYSTEM_H
#define MATEDSYSTEM_H

#include "splittablepart.h"

#include <QList>

class MatedSystem : public SplittablePart
{
public:
    MatedSystem();
    virtual ~MatedSystem();

    // Overloads for splittable part class
    double getBaseCost() const;
    double getBaseSpace() const;

    SplittablePart* getPart(int index) const {if (index >= 0 && index < matedParts.size()) return matedParts.at(index); else return 0;}

    bool addPart(SplittablePart* part);
    bool remPart(SplittablePart* part);

private:
    QList<SplittablePart*> matedParts;

};

#endif // MATEDSYSTEM_H
