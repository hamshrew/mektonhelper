#ifndef ENERGYPOOL_H
#define ENERGYPOOL_H

#include "splittablepart.h"

class Weapon;
class Shield;

class EnergyPool : public SplittablePart
{
public:
    EnergyPool();
    ~EnergyPool();

    // Accessors
    int getPowerAvailable() const {return rating*5;}
    int getMaxPower() const;
    int getKills() const;
    int getPortfolio() const {return portfolio;}
    bool isMorphable() const {return morphable;}
    int getNumApps() const {return weapons.size() + shields.size();}

    double getBaseCost() const;
    double getBaseSpace() const {return getBaseCost();}

    QList<Weapon*>& getWeaponsRef() {return weapons;}
    QList<Shield*>& getShieldsRef() {return shields;}

    // Setters
    void setRating(int newVal) {rating = newVal;}
    void setPortfolio(int newVal) {portfolio = newVal;}
    void setMorphable(bool newVal) {morphable = newVal;}

    // Override setLoc
    void setLoc(const QString &newVal);

    void addWeapon(Weapon* wp);
    void addShield(Shield* sh);

    void remWeapon(int index);
    void remShield(int index);

private:
    int rating;
    int portfolio;
    bool morphable;

    QList<Weapon*> weapons;
    QList<Shield*> shields;
};

#endif // ENERGYPOOL_H
