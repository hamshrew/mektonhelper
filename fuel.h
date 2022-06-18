#ifndef FUEL_H
#define FUEL_H

#include <QString>

class QStringList;

class Fuel
{
public:
    Fuel();

    int getNumTanks() const {return numTanks;}
    bool isExternal() const {return external;}
    double getCost() const {return numTanks * (external ? 2 :1) + eff*0.5;}
    double getSpace() const {return numTanks * (external ? 1 : 2) - eff;}
    int getFlight() const {return numTanks * 500;}
    int getWeightPercent() const {return numTanks *(external ? 0 : 5);}
    double getBMod() const {return numTanks * (external ? 0.5 : 0);}
    const QString& getLoc() const {return loc;}
    double getEff() {return eff;}

    void setNumTanks(int newVal) {numTanks = newVal; eff = (getSpace() < 0) ? (getSpace() + eff) :eff;}
    void setExternal(bool newVal) {external = newVal; eff = (getSpace() < 0) ? (getSpace() + eff) :eff;}
    void setLoc(const QString& newVal) {loc = newVal;}
    void setEff(double newVal) {eff = ((getSpace() + eff - newVal) < 0) ? (getSpace() + eff) : newVal;}

private:
    bool external;
    int numTanks;
    QString loc;
    double eff;
};

#endif // FUEL_H
