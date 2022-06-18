#ifndef CONTROLENTRY_H
#define CONTROLENTRY_H

#include <QWidget>

class CockpitModel;
class SensorModel;
class Mecha;
class QTableView;
class QSpinBox;
class QComboBox;
class QLineEdit;
class QGroupBox;
class QLabel;
class QDoubleSpinBox;
class LocListDelegate;

class ControlEntry : public QWidget
{
    Q_OBJECT
public:
    explicit ControlEntry(Mecha* mecha, QWidget *parent = 0);

    void refreshLocs();

signals:
    void updatedControls();
public slots:
    void someDataChanged();
    void addCockpit();
    void remCockpit();
    void addSensor();
    void remSensor();
    void addReconSys();
    void remReconSys();
    void updateECM();
    void refresh(Mecha* newMech);

private:
    Mecha* mecha;
    QTableView* cockpitTable;
    CockpitModel* cpModel;
    LocListDelegate* locDel;

    QLineEdit* cpName;
    QComboBox* cpLoc;
    QSpinBox* cpCrew;
    QSpinBox* cpPass;
    QComboBox* cpEnc;
    QComboBox* cpEject;

    QTableView* sensorTable;
    SensorModel* sModel;

    QComboBox* reconChoice;

    QLineEdit* sName;
    QComboBox* sLoc;
    QComboBox* sClass;

    QComboBox* ecmLoc;
    QLabel* ecmTotCost;
    QLabel* ecmSpace;
    QDoubleSpinBox* ecmEff;

    QSpinBox* sensJamVal;
    QComboBox* sensJamRad;
    QComboBox* sensJamBeam;

    QSpinBox* missJamVal;
    QComboBox* missJamRad;
    QComboBox* missJamBeam;

    QSpinBox* radJamVal;
    QComboBox* radJamRad;
    QComboBox* radJamBeam;

    QSpinBox* eccmVal;
    QComboBox* eccmRad;
    QComboBox* eccmBeam;

    QGroupBox* buildECMBox();
};

#endif // CONTROLENTRY_H
