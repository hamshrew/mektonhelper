#ifndef SENSOR_H
#define SENSOR_H

#include <QString>
#include <QMap>
#include <QStringList>
#include "sensorcomp.h"
#include "mektonfuncs.h"
#include "splittablepart.h"

class Recon;

class Sensor : public SplittablePart
{
public:
    Sensor();

    const QString& getClass() const {return size;}
    double getBaseCost() const {return mekround(SensorComp::instance()->calcCost(size) + getReconCost());}
    double getBaseSpace() const {return SensorComp::instance()->calcSpace(size) + getReconSpaces();}
    int getKills() const {return SensorComp::instance()->calcKills(size) + getReconKills();}
    QString getSenseRange() const {return SensorComp::instance()->calcSenseRange(size);}
    QString getCommRange() const {return SensorComp::instance()->calcCommRange(size);}
    QStringList getReconList() const;
    const QMap<QString,Recon*>& getReconMapCRef() const {return reconMap;}

    void setClass(const QString& newVal) {size = newVal;}
    bool addRecon(Recon* rec);
    void remRecon(const QString& recName);

private:
    QString size;

    QMap<QString,Recon*> reconMap;

    double getReconCost() const;
    int getReconSpaces() const;
    int getReconKills() const;
};

#endif // SENSOR_H
