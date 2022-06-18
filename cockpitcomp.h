#ifndef COCKPITCOMP_H
#define COCKPITCOMP_H

#include <QMap>
#include <QString>
#include <QStringList>

class CockpitComp
{
public:
    static CockpitComp* instance();

    const QStringList& getEncTypes() const {return encTypes;}

private:
    QStringList encTypes;

    CockpitComp();
    ~CockpitComp();
    CockpitComp(const CockpitComp &);             // intentionally undefined
    CockpitComp & operator=(const CockpitComp &); // intentionally undefined
};

#endif // COCKPITCOMP_H
