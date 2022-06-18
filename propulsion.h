#ifndef PROPULSION_H
#define PROPULSION_H

#include "propcomp.h"

#include <QString>
#include <QMap>

class QStringList;

class Propulsion
{
public:
    Propulsion(const QString& type);

    const QString& getType() const {return type;}
    int getRating() const {return rating;}
    double getCost(double wt) {return rating*wt*PropComp::instance()->calcCostMul(type) + eff*0.5;}
    const QString& getMode() const {return PropComp::instance()->getMoveMode(type);}
    bool usesFuel() const {return PropComp::instance()->usesFuel(type);}
    double getSpaces(const QString& loc) const;
    double getTotalSpace(double wt) const;
    double getAllocSpace() const;
    double getEff() const {return eff;}

    void normalize(double wt);
    void cleanLocs(const QStringList& valid);

    void setType(const QString& newVal) {type = newVal;}
    void setRating(int newVal) {rating = newVal;}
    void setSpaces(const QString& loc,double spaces);
    void setEff(double newVal,double wt) {eff = newVal; normalize(wt);}
    void setEffForced(double newVal) {eff = newVal;}

private:
    QString type;
    int rating;
    double eff;

    QMap<QString,double> spaceMap;
};

#endif // PROPULSION_H
