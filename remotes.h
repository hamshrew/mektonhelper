#ifndef REMOTES_H
#define REMOTES_H

#include <QString>
#include <QList>
#include "mektonfuncs.h"

class Drone;

class Remotes
{
public:
    Remotes();
    ~Remotes();

    // Accessors
    unsigned int getClass() const {return size;}
    const QString& getLoc() const {return loc;}
    unsigned int getControlRange() const {return range;}
    unsigned int getOperationRange() const {return range ? range*3 : 1;}
    double getBaseCost() const {return size ? (2+size*2)*getCostMult():0;}
    double getCost() const {return getBaseCost() + getDroneCost() + mekround(eff /2.0);}
    double getBaseSpace() const {return storesRemotes ? getBaseCost() : size ? 1 : 0;}
    double getSpace() const {return getBaseSpace() - eff;}
    double getWeight() const;
    bool isWired() const {return wired;}
    bool isStoring() const {return storesRemotes;}
    double getEff() const {return eff;}
    QList<Drone*>& getDronesRef() {return drones;}

    // Setters
    void setClass(unsigned int newVal) {size = newVal; resetEff();}
    void setLoc(const QString& newVal) {loc = newVal;}
    void setRange(unsigned int newVal) {range = newVal; resetEff();}
    void setStoresRemotes(bool newVal) {storesRemotes = newVal; resetEff();}
    void setWired(bool newVal) {wired = newVal; resetEff();}
    void setEff(double newVal) {eff = newVal; resetEff();}

    void addDrone(Drone* newDrone) {drones.push_back(newDrone);}
    void remDrone(int index);

private:
    unsigned int size;
    unsigned int range;
    bool storesRemotes;
    QString loc;
    bool wired;
    double eff;

    QList<Drone*> drones;

    double getCostMult() const;
    void resetEff() {eff = (eff > getBaseSpace()) ? getBaseSpace() : eff;}
    double getDroneCost() const;
};

#endif // REMOTES_H
