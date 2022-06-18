#ifndef SENSORCOMP_H
#define SENSORCOMP_H

#include <QMap>
#include <QString>

class SensorComp
{
public:
    static SensorComp* instance();

    int calcCost(const QString& size) const {return senseCostMap[size];}
    int calcKills(const QString& size) const {return senseKillMap[size];}
    int calcSpace(const QString& size) const {return (size == "Backup") ? 2 : 1;}
    QString calcSenseRange(const QString& size) const {return senseRangeMap[size];}
    QString calcCommRange(const QString& size) const {return senseCommMap[size];}

private:
    QMap<QString,int> senseCostMap;
    QMap<QString,int> senseKillMap;
    QMap<QString,QString> senseRangeMap;
    QMap<QString,QString> senseCommMap;

    SensorComp();
    ~SensorComp();
    SensorComp(const SensorComp&);
    SensorComp & operator=(const SensorComp &);
};

#endif // SENSORCOMP_H
