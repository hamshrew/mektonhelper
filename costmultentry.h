#ifndef COSTMULTENTRY_H
#define COSTMULTENTRY_H

#include <QWidget>

class Mecha;
class QCheckBox;
class QGroupBox;
class QComboBox;
class QSpinBox;

class CostMultEntry : public QWidget
{
    Q_OBJECT
public:
    explicit CostMultEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void updatedMult();
public slots:
    void someDataChanged();
    void refresh(Mecha* newMech);

private:
    Mecha* mecha;

    // Environmentals
    QCheckBox* arctic;
    QCheckBox* underwater;
    QCheckBox* highPressure;
    QCheckBox* space;
    QCheckBox* EMprot;
    QCheckBox* desert;
    QCheckBox* reEntry;

    // Cloaking
    QComboBox* cloakLevel;
    QCheckBox* stealth;
    QCheckBox* combat;
    QCheckBox* fireControl;
    QCheckBox* pulse;
    QCheckBox* magnetic;
    QCheckBox* beam;

    // Cockpit Controls
    QComboBox* controls;

    // Maneuver Verniers
    QSpinBox* verniers;

    // Hydraulics
    QComboBox* hydraulics;

    // Power Plant
    QComboBox* plantCharge;
    QComboBox* plantSource;
    QCheckBox* plantHot;

    // ACE
    QComboBox* ACE;

    // useful functions
    void connectAll();
    void disconnectAll();

    // Builder functions
    QGroupBox* buildEnviroBox();
    QGroupBox* buildControlBox();
    QGroupBox* buildVernierBox();
    QGroupBox* buildHydraulicsBox();
    QGroupBox* buildPlantBox();
    QGroupBox* buildACEBox();
    QGroupBox* buildCloakBox();

    // Set functions
    void setEnviro();
    void setCloaking();
};

#endif // COSTMULTENTRY_H
