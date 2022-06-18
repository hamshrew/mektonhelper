#ifndef SERVOENTRY_H
#define SERVOENTRY_H

#include <QWidget>

class Mecha;
class QLineEdit;
class QComboBox;
class QTableView;
class QSpinBox;
class ServoModel;

class ServoEntry : public QWidget
{
    Q_OBJECT
public:
    explicit ServoEntry(Mecha* mecha, QWidget *parent = 0);

signals:
    void updatedServos();

public slots:
    void addServoClicked();
    void delServoClicked();
    void someDataChanged();
    void refresh(Mecha* mecha);

private:
    Mecha* mecha;
    QLineEdit* desigText;
    QComboBox* typeChoice;
    QComboBox* sizeChoice;
    QComboBox* armorChoice;
    QComboBox* armorType;
    QComboBox* ramType;
    QTableView* servoTable;
    QSpinBox* reinforce;
    ServoModel* model;

    QWidget* buildAddServo();
};

#endif // SERVOENTRY_H
