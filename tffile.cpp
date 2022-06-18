#include "tffile.h"
#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QTextStream>

TFFile::TFFile()
{
}

bool TFFile::parseFile(QFile *file)
{
    if (!file)
        return false;

    if (!file->isOpen())
    {
        return false;
    }

    QDomDocument doc;
    QString error;
    int errline;
    int errcol;
    if (!doc.setContent(file,&error,&errline,&errcol))
    {
        file->close();
        return false;
    }

    fillFormList(doc);

    file->close();
    return true;
}

bool TFFile::writeFile(const QString &filename,
                         const QString& specName,
                         const QString& authorName)
{
    if (formList.isEmpty())
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
    QDomElement top = doc.createElement("MektonTransformationSpec");
    top.setAttribute("name",specName);
    top.setAttribute("author",authorName);
    doc.appendChild(top);

    Transformation tf;

    foreach (tf,formList)
    {
        top.appendChild(makeFormNode(tf,doc));
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-16");

    stream << doc.toString();

    file.close();

    return true;
}

bool TFFile::setForms(const QList<Transformation> &formList)
{
    this->formList = formList;
    return true;
}

QDomNode TFFile::makeFormNode(const Transformation &tf,QDomDocument& doc)
{
    QDomElement node = doc.createElement("Form");
    node.setAttribute("Name",tf.getName());
    node.setAttribute("cost",tf.getCost());
    node.setAttribute("special",tf.getSpecial());
    node.setAttribute("hardpoints",tf.getHardPoints());
    node.setAttribute("MV",tf.getMV());

    QStringList props = tf.getProp();
    QString prop;

    foreach(prop,props)
    {
        QDomElement nn = doc.createElement("Prop");
        nn.setAttribute("name",prop);
        node.appendChild(nn);
    }

    props = tf.getMoveList();

    foreach (prop,props)
    {
        QDomElement nn = doc.createElement("Move");
        nn.setAttribute("name",prop);
        nn.setAttribute("MABonus",tf.getMABonus(prop));
        nn.setAttribute("MAMult",tf.getMAMult(prop));
        node.appendChild(nn);
    }

    return node;
}

void TFFile::fillFormList(const QDomDocument &doc)
{
    Transformation* tf;

    QDomElement top = doc.documentElement();

    fileTitle = top.attribute("name","None");
    fileAuthor = top.attribute("author","None");

    QDomNode node = top.firstChild();
    while (!node.isNull())
    {
        if (node.isElement())
        {
            QDomElement el = node.toElement();

            if (node.nodeName() != "Form"){
                node = node.nextSibling();
                continue;
            }
            tf = new Transformation();

            tf->setName(el.attribute("Name","Bad Data"));
            tf->setCost(el.attribute("cost","0").toDouble());
            tf->setSpecial(el.attribute("special","Bad Data"));
            tf->setHardpoints(el.attribute("hardpoints","Bad Data"));
            tf->setMVBonus(el.attribute("MV","0").toInt());

            // Get the propulsion modifiers
            QDomNode newNode = node.firstChild();

            while (!newNode.isNull())
            {
                if (newNode.isElement())
                {
                    if (newNode.nodeName() != "Prop" && newNode.nodeName() != "Move")
                    {
                        newNode = newNode.nextSibling();
                        continue;
                    }

                    QDomElement elem = newNode.toElement();

                    QString propName = elem.attribute("name","Bad Data");

                    if (newNode.nodeName() == "Prop")
                        tf->addProp(propName);
                    else
                    {
                        tf->addMABonus(propName,elem.attribute("MABonus","0").toInt());
                        tf->addMAMult(propName,elem.attribute("MAMult","0").toDouble());
                    }
                }

                newNode = newNode.nextSibling();
            }

            formList.push_back(*tf);

            delete tf;
        }

        node = node.nextSibling();
    }
}
