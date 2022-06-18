#ifndef PROPCOMP_H
#define PROPCOMP_H

#include <QString>
#include <QMap>

class PropComp
{
public:
    static PropComp* instance();

    double calcCostMul(const QString& type) {return costMulMap[type];}
    const QString& getMoveMode(const QString& type) {return moveModeMap[type];}
    bool usesFuel(const QString& type) {return fuelMap[type];}

private:
    QMap<QString,double> costMulMap;
    QMap<QString,QString> moveModeMap;
    QMap<QString,bool> fuelMap;

    PropComp();
    ~PropComp();
    PropComp(const PropComp&);
    PropComp& operator=(const PropComp&);
};

#endif // PROPCOMP_H
