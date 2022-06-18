#ifndef OPTION_H
#define OPTION_H

#include <QString>
#include "mektonfuncs.h"
#include "optioncomp.h"

class Option
{
public:
    Option();

    // Accessors
    const QString& getName() const {return name;}
    const QString& getNotes() const {return notes;}
    double getCost(double wt) const {return OptionComp::instance()->getCost(name)*number * (useWeight() ? wt/10.0 : 1) + mekround(eff/2.0);}
    double getSpace(double wt) const {return OptionComp::instance()->getSpace(name) * number * (useWeight() ? wt/10.0 : 1) - eff;}
    int getNumber() const {return number;}
    bool useWeight() const {return OptionComp::instance()->useWeight(name);}
    double getEff() const {return eff;}
    const QString& getLoc() const {return loc;}

    // Setters
    void setName(const QString& newVal) {name = newVal;}
    void setNotes(const QString& newVal) {notes = newVal;}
    void setNumber(int newVal) {number = newVal; resetEff();}
    void setEff(double newVal) {eff = newVal; resetEff();}
    void setLoc(const QString& newVal) {loc = newVal;}

private:
    QString name;
    QString notes;
    int number;
    double eff;
    QString loc;

    void resetEff() {eff = (eff > (OptionComp::instance()->getSpace(name) * number)) ? (OptionComp::instance()->getSpace(name) * number) : eff;}
};

#endif // OPTION_H
