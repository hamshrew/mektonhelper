#include "sensor.h"
#include "recon.h"

Sensor::Sensor()
{
}

bool Sensor::addRecon(Recon *rec)
{
    if (reconMap.contains(rec->getName()))
    {
        if (rec->isHasLevels())
        {
            reconMap[rec->getName()]->setLevels(reconMap[rec->getName()]->getLevels() +1);
        }
        delete rec;
        return false;
    }

    reconMap[rec->getName()] = rec;
    return true;
}

void Sensor::remRecon(const QString &recName)
{
    if (reconMap.contains(recName))
    {
        if (reconMap[recName]->getLevels() > 1)
        {
            reconMap[recName]->setLevels(reconMap[recName]->getLevels() - 1);
            return;
        }
        delete reconMap[recName];
        reconMap.remove(recName);
    }

    resetEff();
}

QStringList Sensor::getReconList() const
{
    QStringList list;

    Recon* recon;

    foreach (recon,reconMap)
    {
        if (recon->isHasLevels())
        {
            list.push_back(QString(recon->getName() + ": Rank " + QString::number(recon->getLevels())));
        }
        else
        {
            list.push_back(recon->getName());
        }
    }

    return list;
}

double Sensor::getReconCost() const
{
    double tCost = 0;

    Recon* recon;

    foreach (recon,reconMap)
    {
        tCost += recon->getCost();
    }

    return tCost;
}

int Sensor::getReconSpaces() const
{
    int spaces = 0;

    Recon* recon;

    foreach (recon,reconMap)
    {
        spaces += recon->getSpace();
    }

    return spaces;
}

int Sensor::getReconKills() const
{
    int kills = 0;

    Recon* recon;

    foreach (recon,reconMap)
    {
        kills += recon->getKills();
    }

    return kills;
}
