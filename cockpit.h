#ifndef COCKPIT_H
#define COCKPIT_H

#include "cockpitcomp.h"
#include "mektonfuncs.h"
#include "splittablepart.h"

#include <QString>

class Cockpit : public SplittablePart
{
public:
    Cockpit();

    // Accessors
    unsigned int getCrew() const {return crew;}
    unsigned int getPassengers() const {return pass;}
    double getBaseSpace() const {return (enc == "Saddle") ? 0 : (crew + pass);}
    double getBaseCost() const {return pass + ((crew-1)*2);}
    const QString& getEnc() const {return enc;}

    // Setters
    void setCrew(unsigned int newVal) {crew = newVal; if (eff > (crew + pass)) eff = crew + pass;}
    void setPassengers(unsigned int newVal) {pass = newVal; if (eff > (crew + pass)) eff = crew + pass;}
    void setEnc(const QString& newVal) {enc = newVal;}

private:
    unsigned int crew;
    unsigned int pass;

    QString enc;
};

#endif // COCKPIT_H
