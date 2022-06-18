#ifndef RECON_H
#define RECON_H

#include <QString>

class Recon
{
public:
    Recon();

    // Accessors
    const QString& getName() const {return name;}
    int getLevels() const {return levels;}
    int getSpace() const {return space;}
    int getKills() const {return kills;}
    double getCost() const {return cost * levels;}
    bool isHasLevels() const {return hasLevels;}

    // Setters
    void setName(const QString& newVal) {name = newVal;}
    void setLevels(int newVal) {levels = newVal;}
    void setSpace(int newVal) {space = newVal;}
    void setKills(int newVal) {kills = newVal;}
    void setBaseCost(double newVal) {cost = newVal;}
    void setHasLevels(bool newVal) {hasLevels = newVal;}

private:
    QString name;
    int levels;
    bool hasLevels;
    double cost;
    int space;
    int kills;
};

#endif // RECON_H
