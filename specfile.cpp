#include "specfile.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

SpecFile::SpecFile()
{
}

bool SpecFile::parseFile(QFile *file)
{
    if (!file)
        return false;

    if (!file->isOpen())
    {
        return false;
    }

    QDomDocument doc;
    if (!doc.setContent(file))
    {
        file->close();
        return false;
    }

    fillSpecList(doc);

    file->close();
    return true;
}

bool SpecFile::writeFile(const QString &filename,
                         const QString& specName,
                         const QString& authorName)
{
    if (specList.isEmpty())
    {
        return false;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return false;
    }

    QDomDocument doc;

    doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-16\""));
    QDomElement top = doc.createElement("MektonWeaponSpec");
    top.setAttribute("name",specName);
    top.setAttribute("author",authorName);
    doc.appendChild(top);

    Weapon wp;

    foreach (wp,specList)
    {
        top.appendChild(makeWeaponNode(wp,doc));
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-16");
    QString test = doc.toString();
    stream << doc.toString();

    file.close();

    return true;
}

bool SpecFile::setSpecs(const QList<Weapon> &specList)
{
    this->specList = specList;
    return true;
}

QDomNode SpecFile::makeWeaponNode(const Weapon &wp,QDomDocument& doc)
{
    QDomElement node = doc.createElement("Weapon");
    node.setAttribute("Name",wp.getName());
    node.setAttribute("WA",wp.getAcc());

    node.setAttribute("Damage",wp.getDamage());
    node.setAttribute("Range",wp.getRange());
    node.setAttribute("Kills",wp.getKills());
    node.setAttribute("ShotPrepend",wp.getShotPrepend());
    node.setAttribute("Shots",wp.getShots());
    node.setAttribute("Cost",wp.getBaseCost());
    node.setAttribute("Space",wp.getSpace() + wp.getEff());
    node.setAttribute("Eff",wp.getEff());
    node.setAttribute("Type",wp.getType());
    node.setAttribute("Notes",wp.getAllNotes());

    if (wp.isUseAmmo())
        node.setAttribute("UseAmmo","YES");

    if (wp.isMelee())
        node.setAttribute("Melee","YES");

    if (wp.isHandy())
        node.setAttribute("Handy","YES");

    if (wp.isClumsy())
        node.setAttribute("Clumsy","YES");

    if (wp.isUseClips())
        node.setAttribute("UseClips","YES");

    QDomText text = doc.createTextNode(wp.getComments());
    node.appendChild(text);

    return node;
}

void SpecFile::fillSpecList(const QDomDocument &doc)
{
    Weapon* wp;

    QDomElement top = doc.documentElement();

    specTitle = top.attribute("name","None");
    specAuthor = top.attribute("author","None");

    QDomNode node = top.firstChild();
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement el = node.toElement();

            if (node.nodeName() != "Weapon"){
                node = node.nextSibling();
                continue;
            }
            wp = new Weapon();

            wp->setName(el.attribute("Name","Bad Data"));
            wp->setAcc(el.attribute("WA","0").toInt());
            wp->setDamage(el.attribute("Damage","1").toUInt());
            wp->setRange(el.attribute("Range","1").toUInt());
            wp->setKills(el.attribute("Kills","1").toUInt());
            wp->setShotPrepend(el.attribute("ShotPrepend",""));
            wp->setShots(el.attribute("Shots","Bad"));
            wp->setCost(el.attribute("Cost","0.0").toDouble());
            wp->setType(el.attribute("Type",""));
            wp->setSpace(el.attribute("Space","0.0").toDouble());
            wp->setEff(el.attribute("Eff","0.0").toDouble());

            if (el.attribute("UseAmmo","NO") == "YES")
                wp->setUseAmmo(true);

            if (el.attribute("UseClips","NO") == "YES")
                wp->setUseClips(true);

            if (el.attribute("Clumsy","NO") == "YES")
                wp->setClumsy(true);

            if (el.attribute("Melee","NO") == "YES")
                wp->setMelee(true);

            if (el.attribute("Handy","NO") == "YES")
                wp->setHandy(true);

            wp->addNote(el.attribute("Notes","Bad Data"));

            // Check for text
            QDomNode nn = node.firstChild();

            if (!nn.isNull() && nn.isText())
            {
                wp->setComments(nn.toText().data());
            }

            specList.push_back(*wp);

            delete wp;
        }

        node = node.nextSibling();
    }
}
