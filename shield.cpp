#include "shield.h"
#include <cmath>

Shield::Shield():
    size(SUPERLIGHT),
    type(STANDARD),
    armor(NORMAL),
    binderMult(0),
    optMult(1.0)
{
}

QString Shield::typeName(ShieldTypes t)
{
    switch (t)
    {
    case STANDARD:
        return "Standard";
    case ACTIVE:
        return "Active";
    case REACTIVE:
        return "Reactive";
    }

    return "None";
}

double Shield::calcBinderMult() const
{
    if (!binderMult)
        return 1;
    if (binderMult == 0.5)
        return 1.3;

    if (binderMult < 0.5)
        return (9 + trunc(binderMult*10))/10.0;

    return (18 - trunc(binderMult*10))/10.0;
}
