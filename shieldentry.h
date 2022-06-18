#ifndef SHIELDENTRY_H
#define SHIELDENTRY_H

#include <QWidget>

class Mecha;
class QTableView;
class ShieldModel;
class QLabel;
class QLineEdit;
class QComboBox;
class QSpinBox;
class QDoubleSpinBox;
class QCheckBox;
class QRadioButton;
class LocListDelegate;
class ReflectorModel;

class ShieldEntry : public QWidget
{
    Q_OBJECT
public:
    explicit ShieldEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void shieldsUpdated();
public slots:
    void refresh(Mecha* mecha);
    void updateValues();
    void someDataChanged();
    void addShield();
    void remShield();
    void addReflect();
    void remReflect();

private:
    Mecha* mecha;

    QTableView* shieldTable;
    ShieldModel* model;

    LocListDelegate* locDel;

    QLineEdit* sName;
    QComboBox* sType;
    QComboBox* sClass;
    QComboBox* sArmor;
    QLabel* cost;
    QLabel* space;
    QLabel* binderSpace;
    QLabel* SP;
    QComboBox* binder;
    QDoubleSpinBox* eff;
    QSpinBox* DA;
    QComboBox* reset;
    QComboBox* tiu;
    QRadioButton* wkNo;
    QRadioButton* wkEn;
    QRadioButton* wkMat;
    QRadioButton* wkRng;
    QCheckBox* ablative;
    QCheckBox* enclosing;
    QCheckBox* surge;

    QTableView* refTable;
    ReflectorModel* rModel;

    LocListDelegate* refLocDel;

    QLineEdit* refName;
    QSpinBox* refVal;
    QLabel* refCost;
    QLabel* refSpace;
    QDoubleSpinBox* refEff;
};

#endif // SHIELDENTRY_H
