#ifndef SPLITTABLEPART_H
#define SPLITTABLEPART_H

#include "mektonfuncs.h"

#include <QMap>
#include <QString>
#include <QStringList>

class MatedSystem;

class SplittablePart
{
public:
    SplittablePart();
    virtual ~SplittablePart();

    const QString& getName() const {return name;}
    double getAllocSpace() const;
    double getAllocSpace(const QString& ser) const;
    virtual double getBaseCost() const = 0;
    double getCost() const {return getBaseCost() + mekround(eff/2.0) + (isSplit() ? getSplitCost() : 0);}
    double getSpace() const {return getBaseSpace() - eff;}
    double getSpace(const QString& ser) const {return getAllocSpace(ser);}
    virtual double getBaseSpace() const = 0;
    const QString& getLoc() const {return loc;}
    double getEff() const {return eff;}
    virtual double getSplitCost() const {return splitMap.empty() ? 0 : (splitMap.size() - 1)*3.0;}
    bool isSplit() const {return loc == "Split";}
    QStringList getSplitLocs() const {return splitMap.keys();}
    bool isMated() const {return mated ? true :false;}

    virtual void setName(const QString& newVal) {name = newVal;}
    virtual void setLoc(const QString& newVal) {loc = newVal;}
    void setAlloc(const QString& spot,double sp);
    virtual void setEff(double newVal) {eff = newVal; resetEff();}
    void setMate(MatedSystem* mate) {mated = mate;}

    void normalize(const QStringList& locList);

protected:
    QString name;
    QString loc;
    double eff;
    void resetEff() {eff = (eff > getBaseSpace()) ? getBaseSpace() : eff;}


private:
    QMap<QString,double> splitMap;
    MatedSystem* mated;

};

#endif // SPLITTABLEPART_H
