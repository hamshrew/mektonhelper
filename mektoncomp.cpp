#include "mektoncomp.h"

MektonComp::MektonComp()
{
}

MektonComp::~MektonComp()
{

}

double MektonComp::convMAtoKPH(int MA)
{
    return (MA < 11) ? MA*18.0 : (MA/5 -1)*MA*18.0;
}

double MektonComp::convMAtoMPH(int MA)
{
    return (MA < 11) ? MA *11.2 : (MA/5 -1)*MA*11.2;
}
