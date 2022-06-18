#ifndef MEKTONCOMP_H
#define MEKTONCOMP_H

class MektonComp
{
public:
    static MektonComp* instance();

    static double convMAtoKPH(int MA);
    static double convMAtoMPH(int MA);
private:
    MektonComp();
    ~MektonComp();
    MektonComp(const MektonComp& rhs);
    MektonComp& operator=(const MektonComp& rhs);
};

#endif // MEKTONCOMP_H
