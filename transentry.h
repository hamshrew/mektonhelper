#ifndef TRANSENTRY_H
#define TRANSENTRY_H

#include "transformation.h"

#include <QWidget>
#include <QList>


class QComboBox;
class QGroupBox;
class QGridLayout;
class QCheckBox;
class QSignalMapper;
class Mecha;

class TransEntry : public QWidget
{
    Q_OBJECT
public:
    explicit TransEntry(Mecha* mecha,QWidget *parent = 0);
    virtual ~TransEntry();

signals:
    void tfChange();

public slots:
    void toggleForm(QString name);
    void refresh(Mecha* newMech);
    void defaultChange();

private:
    Mecha* mecha;
    QList<Transformation> transSpecList;
    QComboBox* defaultSelect;
    QGridLayout* gridLayout;
    QCheckBox* mekBox;
    QSignalMapper* mekMapper;

    // private methods
    void loadTransformations();
    QGroupBox* makeTFBox(const Transformation& tf);
    void resetDefaultBox();

};

#endif // TRANSENTRY_H
