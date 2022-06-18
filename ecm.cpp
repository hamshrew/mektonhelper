#include "ecm.h"

ECM::ECM(const QString& name)
{
    setName(name);
}

double ECM::getBaseCost() const
{
    double cost = 0;

    switch(value)
    {
    case 1: cost+=1;
        break;
    case 2: cost+=2;
        break;
    case 3: cost+=4;
        break;
    case 4: cost+=6;
        break;
    case 5: cost+=9;
        break;
    case 6: cost+=12;
        break;
    case 7: cost+=16;
        break;
    case 8: cost+=20;
        break;
    case 9: cost+=25;
        break;
    case 10: cost+=30;
        break;
    default: cost =0;
        break;
    }

    switch(radius)
    {
    case 1: cost*=1.5;
        break;
    case 2: cost*=1.75;
        break;
    case 3: cost*=2;
        break;
    case 4: cost*=2.25;
        break;
    case 6: cost*=2.75;
        break;
    case 8: cost*=3.2;
        break;
    case 10: cost*=3.5;
        break;
    case 20: cost*=3.9;
        break;
    case 60: cost*=4.5;
        break;
    case 100: cost*=5.25;
        break;
    default: cost *=1;
        break;
    }

    switch(beaming)
    {
    case 1: cost*=1.1;
        break;
    case 2: cost*=1.2;
        break;
    case 3: cost*=1.3;
        break;
    case 4: cost*=1.4;
        break;
    case 6: cost*=1.6;
        break;
    case 8: cost*=2;
        break;
    case 10: cost*=2.3;
        break;
    case 20: cost*=2.6;
        break;
    case 60: cost*=3.0;
        break;
    case 100: cost*=3.5;
        break;
    default: cost *=1;
        break;
    }

    return cost;
}
