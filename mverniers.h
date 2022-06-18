#ifndef MVERNIERS_H
#define MVERNIERS_H

#include "splittablepart.h"

class MVerniers : public SplittablePart
{
public:
    MVerniers();

    // accessors
    int getRating() const {return rating;}
    double getCostMult() const {return rating * 0.1;}
    double getBaseSpace() const {return rating * 5;}
    double getBaseCost() const {return 0;}
    double getSplitCost() const {return 0;} // Verniers have no cost for splitting

    // setters
    void setRating(int newVal) {rating = newVal;resetEff();}

private:
    int rating;
};

#endif // MVERNIERS_H
