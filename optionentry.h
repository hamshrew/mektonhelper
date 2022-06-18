#ifndef OPTIONENTRY_H
#define OPTIONENTRY_H

#include <QWidget>

class QTableView;
class QLineEdit;
class QLabel;
class QSpinBox;
class QComboBox;
class QGroupBox;
class QCheckBox;
class QDoubleSpinBox;
class OptionModel;
class Mecha;
class LocListDelegate;
class RemoteModel;

class OptionEntry : public QWidget
{
    Q_OBJECT
public:
    explicit OptionEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void optionsUpdated();
public slots:
    void someDataChanged();
    void remoteDataChanged();
    void updatePrev();
    void addOption();
    void remOption();
    void addDrone();
    void remDrone();
    void refresh(Mecha* newMech);
    void updateServos();
private:
    Mecha* mecha;
    QTableView* optionTable;
    OptionModel* model;
    LocListDelegate* locDel;

    QComboBox* optionChoice;
    QLineEdit* notes;
    QSpinBox* number;
    QLabel* cost;
    QLabel* space;

    // Remote stuff
    QComboBox* remoteClass;
    QComboBox* controlRange;
    QLabel* opRange;
    QCheckBox* wired;
    QLabel* rCost;
    QLabel* rSpace;
    QComboBox* remoteLoc;
    QCheckBox* remStored;
    QDoubleSpinBox* remEff;

    QTableView* droneTable;
    RemoteModel* droneModel;

    // Builders
    QGroupBox* buildOptionBox();
    QGroupBox* buildRemoteBox();
};

#endif // OPTIONENTRY_H
