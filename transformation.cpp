#include "transformation.h"

Transformation::Transformation():
    name("Humanoid"),
    special("None"),
    hardpoints("All"),
    MVBonus(0),
    cost(0)
{
}

Transformation::Transformation(const QString &name):
    name(name),
    special("None"),
    hardpoints("All"),
    MVBonus(0),
    cost(0)
{

}

Transformation::~Transformation()
{
}

QString Transformation::getAllMAMult() const
{
    QStringList allMults;

    for (QMap<QString,double>::const_iterator it = MAMultMap.begin();
         it != MAMultMap.end();++it)
    {
        allMults.append(QString(it.key() + QString(" x") + QString::number(it.value())));
    }

    return allMults.join(", ");
}

QString Transformation::getAllMABonus() const
{
    QStringList all;

    for (QMap<QString,int>::const_iterator it = MABonusMap.begin();
         it != MABonusMap.end();++it)
    {
        all.append(QString(it.key() + QString(" ")
                   + QString((it.value() >= 0) ? "+" : "") + QString::number(it.value())));
    }

    return all.join(", ");
}
