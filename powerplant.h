#ifndef POWERPLANT_H
#define POWERPLANT_H

#include "mekton_defs.h"
#include <QString>

class PowerPlant
{
public:
    PowerPlant();

    // Accessors
    int getCharge() const {return charge;}
    bool isHot() const {return hot;}
    PlantSources getSource() const {return source;}
    QString getSourceName() const;
    QString getChargeName() const;
    double getCostMult() const;

    // Setters
    void setCharge(int newVal) {charge = newVal;}
    void setHot(bool newVal) {hot = newVal;}
    void setSource(PlantSources newVal) {source = newVal;}

private:
    int charge;
    bool hot;
    PlantSources source;
};

#endif // POWERPLANT_H
