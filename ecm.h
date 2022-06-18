#ifndef ECM_H
#define ECM_H

#include "splittablepart.h"

class ECM : public SplittablePart
{
public:
    ECM(const QString& name);

    // Virtuals
    double getBaseCost() const;
    double getBaseSpace() const {return getBaseCost();}

    // Accessors
    int getValue() const {return value;}
    int getRadius() const {return radius;}
    int getBeaming() const {return beaming;}

    // Setters
    void setValue(int newVal) {value = newVal;}
    void setRadius(int newVal) {radius = newVal;}
    void setBeaming(int newVal) {beaming = newVal;}

private:
    int value;
    int radius;
    int beaming;
};

#endif // ECM_H
