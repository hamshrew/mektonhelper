#ifndef RECONCOMP_H
#define RECONCOMP_H

#include <QMap>
#include <QString>
#include <QStringList>

class ReconComp
{
public:
    static ReconComp* instance();

    double calcCost(const QString& recon) const {return costMap[recon];}
    int calcSpace(const QString& recon) const {return spaceMap[recon];}
    bool checkHasLevels(const QString& recon) const {return (recon == "Resolution Intensifiers") ? true : false;}
    int calcKills(const QString& recon) const {return (recon == "Spotting Radar") ? 4 : 0;}

    QStringList getReconList() const {return costMap.keys();}

private:
    QMap<QString,double> costMap;
    QMap<QString,int> spaceMap;

    ReconComp();
    ~ReconComp();
    ReconComp(const ReconComp&);
    ReconComp & operator=(const ReconComp &);
};

#endif // RECONCOMP_H
