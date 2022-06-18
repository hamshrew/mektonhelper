#ifndef COCKPITCONTROLS_H
#define COCKPITCONTROLS_H

#include <QString>

class CockpitControls
{
public:
    CockpitControls();

    // Accessors
    const QString& getName() const {return name;}
    int getBonus() const {return poolBon;}
    double getBonusMult() const {return poolMult;}
    double getCostMult() const {return costMult;}

    // Setters
    void setName(const QString& newVal) {name = newVal;}
    void setBonus(int newVal) {poolBon = newVal;}
    void setBonusMult(double newVal) {poolMult = newVal;}
    void setCostMult(double newVal) {costMult = newVal;}

private:
    QString name;
    int poolBon;
    double poolMult;
    double costMult;
};

#endif // COCKPITCONTROLS_H
