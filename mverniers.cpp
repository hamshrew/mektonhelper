#include "mverniers.h"

MVerniers::MVerniers():
    rating(0)
{
    // Verniers are always Split
    setLoc("Split");
    setName("Maneuver Verniers");
}
