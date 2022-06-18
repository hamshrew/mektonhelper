#ifndef PROPENTRY_H
#define PROPENTRY_H

#include <QWidget>

class Mecha;
class QSpinBox;
class QGroupBox;
class QLabel;
class QTableView;
class QComboBox;
class QDoubleSpinBox;
class PropModel;
class FuelModel;
class GTModel;
class LocListDelegate;

class PropEntry : public QWidget
{
    Q_OBJECT
public:
    explicit PropEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void propChanged();
public slots:
    void updatePropulsion();
    void refreshEntry();
    void addFuel();
    void remFuel();
    void someDataChanged();
    void adjustSizes();
    void resetPropTable(const QString& propName);
    void resetGTTable(const QString& gtName);
    void refresh(Mecha* newMech);
private:
    Mecha* mecha;

    QSpinBox* wtEff;
    QSpinBox* wtInEff;

    LocListDelegate* llDelegate;

    QComboBox* wheels;
    QComboBox* treads;

    QLabel* wheelWeight;
    QLabel* treadWeight;

    QSpinBox* GESBox;
    QSpinBox* HydroJetBox;
    QSpinBox* HydroThrustBox;
    QSpinBox* MHDBox;
    QSpinBox* ThrusterBox;
    QSpinBox* GravBox;

    QLabel* GESCost;
    QLabel* HydroJetCost;
    QLabel* HydroThrustCost;
    QLabel* MHDCost;
    QLabel* ThrusterCost;
    QLabel* GravCost;

    QSpinBox* fuelWeight;
    QLabel* maxDistance;
    QTableView* fuelTable;
    FuelModel* fModel;

    QTableView* propTable;
    PropModel* pModel;
    QComboBox* propChoice;
    QDoubleSpinBox* propEff;
    QLabel* propUnSpace;

    QTableView* gtTable;
    GTModel* gtModel;
    QComboBox* gtChoice;
    QLabel* gtUnKills;

    QGroupBox* buildPropMABox();
    QGroupBox* buildPropSpaceBox();
    QGroupBox* buildFuelBox();
    QGroupBox* buildGTBox();
    QStringList calcValidSizes() const;
    void refillValues();
};

#endif // PROPENTRY_H
