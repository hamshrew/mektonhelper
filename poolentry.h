#ifndef POOLENTRY_H
#define POOLENTRY_H

#include <QWidget>
#include <QModelIndex>

class Mecha;
class QTableView;
class QComboBox;
class QLabel;
class QCheckBox;
class QGroupBox;
class QLineEdit;
class QDoubleSpinBox;
class QSpinBox;
class QRadioButton;
class LocListDelegate;
class PoolModel;
class PoolWeaponModel;
class PoolShieldModel;
class BaseSpecEntry;

class PoolEntry : public QWidget
{
    Q_OBJECT
public:
    explicit PoolEntry(Mecha* mecha,QWidget *parent = 0);

signals:
    void updatedPools();

public slots:
    void someDataChanged();
    void poolSelected(QModelIndex index);
    void updatePoolStats();
    void updateShieldValues();
    void addPool();
    void remPool();
    void addWeapon();
    void remWeapon();
    void addShield();
    void remShield();
    void changeEntryType(int type);
    void refresh(Mecha* newMech);

private:
    Mecha* mecha;

    QTableView* poolTable;
    PoolModel* poolModel;
    QTableView* weaponTable;
    PoolWeaponModel* weaponModel;
    QTableView* shieldTable;
    PoolShieldModel* shieldModel;
    LocListDelegate* llDel;

    // Pool Controls
    QLineEdit* poolName;
    QComboBox* poolPower;
    QComboBox* poolSize;
    QCheckBox* morphable;
    QLabel* poolCost;
    QLabel* poolSpace;
    QDoubleSpinBox* poolEff;

    // Weapon Entry
    QComboBox* selectType;
    QGroupBox* specBox;
    BaseSpecEntry* wSpec;

    // Shield Entry
    QLineEdit* sName;
    QComboBox* sType;
    QComboBox* sClass;
    QComboBox* sArmor;
    QLabel* cost;
    QLabel* SP;
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

    // Builders
    QGroupBox* buildPoolBox();
    QGroupBox* buildWeaponView();
    QGroupBox* buildShieldView();
};

#endif // POOLENTRY_H
