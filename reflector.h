#ifndef REFLECTOR_H
#define REFLECTOR_H

#include "splittablepart.h"

class Reflector : public SplittablePart
{
public:
    Reflector();

    unsigned int getValue() const {return value;}
    double getBaseCost() const {return value*value;}
    double getBaseSpace() const {return getBaseCost();}

    void setValue(unsigned int newVal) {value = newVal;}

private:
    unsigned int value;
};

#endif // REFLECTOR_H
