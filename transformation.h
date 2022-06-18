#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H

#include <QString>
#include <QStringList>
#include <QMap>

class Transformation
{
public:
    Transformation();
    Transformation(const QString& name);
    virtual ~Transformation();

    const QString& getName() const {return name;}
    const QString& getSpecial() const {return special;}
    const QString& getHardPoints() const {return hardpoints;}
    const QStringList& getProp() const {return propulsion;}
    int getMV() const {return MVBonus;}
    int getMABonus(const QString& prop) const {return MABonusMap.value(prop);}
    double getMAMult(const QString& prop) const {return MAMultMap.value(prop);}
    double getCost() const {return cost;}
    QStringList getMoveList() const { QStringList res(MABonusMap.keys()); res.append(MAMultMap.keys()); res.removeDuplicates(); return res;}
    QString getAllMAMult() const;
    QString getAllMABonus() const;

    void setName(const QString& newVal) {name = newVal;}
    void setSpecial(const QString& newVal) {special = newVal;}
    void setHardpoints(const QString& newVal) {hardpoints = newVal;}
    void setMVBonus(int newVal) {MVBonus = newVal;}
    void addProp(const QString& newVal) {propulsion.append(newVal);}
    void addMABonus(const QString& newProp,int bonus) {MABonusMap.insert(newProp,bonus);}
    void addMAMult(const QString& newProp,double mult) {MAMultMap.insert(newProp,mult);}
    void setCost(double newVal) {cost = newVal;}

    void clean() {propulsion.clear();
                  MABonusMap.clear();
                  MAMultMap.clear();
                  cost = 0;
                  MVBonus = 0;
                  name = "";
                  special = "None";
                  hardpoints = "All";}

private:
    QString name;
    QString special;
    QString hardpoints;
    QStringList propulsion;
    QMap<QString,int> MABonusMap;
    int MVBonus;
    QMap<QString,double> MAMultMap;
    double cost;
};

#endif // TRANSFORMATION_H
