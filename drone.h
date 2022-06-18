#ifndef DRONE_H
#define DRONE_H

#include <QString>

class Drone
{
public:
    Drone();
    virtual ~Drone();

    const QString& getName() const {return name;}
    double getCost() const {return cost;}
    double getWeight() const {return weight;}

    void setName(const QString& newVal) {name = newVal;}
    void setCost(double newVal) {cost = newVal;}
    void setWeight(double newVal) {weight = newVal;}

private:
    QString name;
    double cost;
    double weight;
};

#endif // DRONE_H
