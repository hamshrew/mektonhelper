#ifndef TFFILE_H
#define TFFILE_H

#include "transformation.h"
#include <QList>
#include <QDomNode>
#include <QString>

class QFile;
class QDomDocument;

class TFFile
{
public:
    TFFile();

    bool parseFile(QFile* file);
    bool writeFile(const QString& filename,
                   const QString& specName,
                   const QString& authorName);
    bool setForms(const QList<Transformation>& formList);
    const QList<Transformation>& getForms() const {return formList;}
    const QString& getTitle() const {return fileTitle;}
    const QString& getAuthor() const {return fileAuthor;}

private:
    QList<Transformation> formList;
    QString fileTitle;
    QString fileAuthor;

    QDomNode makeFormNode(const Transformation& tf, QDomDocument& doc);
    void fillFormList(const QDomDocument& doc);
};

#endif // TFFILE_H
