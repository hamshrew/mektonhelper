#ifndef SPECFILE_H
#define SPECFILE_H

#include "weapon.h"
#include <QList>
#include <QDomNode>
#include <QString>

class QFile;
class QDomDocument;

class SpecFile
{
public:
    SpecFile();

    bool parseFile(QFile* file);
    bool writeFile(const QString& filename,
                   const QString& specName,
                   const QString& authorName);
    bool setSpecs(const QList<Weapon>& specList);
    const QList<Weapon>& getSpecs() const {return specList;}
    const QString& getTitle() const {return specTitle;}
    const QString& getAuthor() const {return specAuthor;}

private:
    QList<Weapon> specList;
    QString specTitle;
    QString specAuthor;

    QDomNode makeWeaponNode(const Weapon& wp, QDomDocument& doc);
    void fillSpecList(const QDomDocument& doc);
};

#endif // SPECFILE_H
