#include "servoentry.h"
#include "mecha.h"
#include "servo.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QScrollArea>
#include "servomodel.h"
#include "typedelegate.h"
#include "sizedelegate.h"
#include "armordelegate.h"
#include "ramdelegate.h"
#include "spinboxdelegate.h"

ServoEntry::ServoEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* entryLayout = new QVBoxLayout();

    servoTable = new QTableView(this);

    QList<Servo*>& servoList = mecha->getServoRef();

    model = new ServoModel(servoList.size(),15,servoList);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    servoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    servoTable->setSelectionMode(QAbstractItemView::SingleSelection);
    servoTable->setModel(model);

    // Set column widths
    servoTable->setColumnWidth(0,50);
    servoTable->setColumnWidth(1,40);
    servoTable->setColumnWidth(3,55);
    servoTable->setColumnWidth(4,40);
    servoTable->setColumnWidth(5,40);
    servoTable->setColumnWidth(6,60);
    servoTable->setColumnWidth(8,30);
    servoTable->setColumnWidth(9,70);
    servoTable->setColumnWidth(10,75);
    servoTable->setColumnWidth(11,30);
    servoTable->setColumnWidth(12,40);
    servoTable->setColumnWidth(13,75);

    TypeDelegate* typeDel = new TypeDelegate();
    SizeDelegate* sizeDel = new SizeDelegate();
    SpinBoxDelegate* mountDel = new SpinBoxDelegate(0,2);
    ArmorDelegate* armorDel = new ArmorDelegate();
    RAMDelegate* ramDel = new RAMDelegate();
    SpinBoxDelegate* spinDel = new SpinBoxDelegate(-22,11);

    servoTable->setItemDelegateForColumn(1,typeDel);
    servoTable->setItemDelegateForColumn(2,sizeDel);
    servoTable->setItemDelegateForColumn(6,mountDel);
    servoTable->setItemDelegateForColumn(7,sizeDel);
    servoTable->setItemDelegateForColumn(10,armorDel);
    servoTable->setItemDelegateForColumn(12,ramDel);
    servoTable->setItemDelegateForColumn(14,spinDel);

    QWidget* addServo = buildAddServo();

    QPushButton* delServoButton = new QPushButton(tr("Delete Servo"));
    delServoButton->setToolTip(tr("Delete Currently Selected Servo"));

    connect(delServoButton,SIGNAL(clicked()),SLOT(delServoClicked()));

    entryLayout->addWidget(servoTable);
    entryLayout->addWidget(addServo);
    entryLayout->addWidget(delServoButton);

    baseWidget->setLayout(entryLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

QWidget* ServoEntry::buildAddServo()
{
    QWidget* addServo = new QWidget();

    QHBoxLayout* addServoLayout = new QHBoxLayout();

    QPushButton* addButton = new QPushButton(tr("Add Servo"));

    desigText = new QLineEdit("");

    typeChoice = new QComboBox();
    typeChoice->addItems(ServoComp::instance()->getTypes());

    sizeChoice = new QComboBox();
    sizeChoice->addItems(ServoComp::instance()->getSizes());

    reinforce = new QSpinBox();
    reinforce->setValue(0);
    reinforce->setMinimum(-22);
    reinforce->setMaximum(11);

    connect(addButton,SIGNAL(clicked()),this,SLOT(addServoClicked()));

    addServoLayout->addWidget(addButton);
    addServoLayout->addWidget(desigText);
    addServoLayout->addWidget(typeChoice);
    addServoLayout->addWidget(sizeChoice);
    addServoLayout->addWidget(reinforce);

    QLabel* armorLabel = new QLabel(tr("Armor:"));
    armorChoice = new QComboBox();
    armorChoice->addItems(ServoComp::instance()->getSizes());

    armorType = new QComboBox();
    armorType->addItems(ServoComp::instance()->getArmorTypes());
    armorType->setCurrentIndex(1);
    armorType->setToolTip("Armor Type to determine Damage Coefficient.");

    ramType = new QComboBox();
    ramType->addItems(ServoComp::instance()->getRAMTypes());
    ramType->setToolTip("Energy Absorption Amount");

    addServoLayout->addWidget(armorLabel);
    addServoLayout->addWidget(armorChoice);
    addServoLayout->addWidget(armorType);
    addServoLayout->addWidget(ramType);
    addServo->setLayout(addServoLayout);

    return addServo;
}

void ServoEntry::addServoClicked()
{
    if ((desigText->text() == "") || mecha->getServoNames().contains(desigText->text())
        || (desigText->text() == "None") || (desigText->text() == "Split"))
    {
        QMessageBox msgBox;
        msgBox.setText("Your Servo Name is either empty, reserved(None or Split), or already being used.");
        msgBox.exec();
        return;
    }

    QRegExp rx;
    rx.setPattern("^\\d+*");

    if (rx.exactMatch(desigText->text()))
    {
        QMessageBox msgBox;
        msgBox.setText("Your Servo Name cannot begin with a number.");
        msgBox.exec();
        return;
    }

    rx.setPattern("* Mount*");

    if (rx.exactMatch(desigText->text()))
    {
        QMessageBox msgBox;
        msgBox.setText("Your Servo Name should not contain the word 'Mount'.");
        msgBox.exec();
        return;
    }

    Servo* servo = new Servo();

    servo->setDesignation(desigText->text());
    servo->setType((ServoTypes)typeChoice->currentIndex());
    servo->setSize((ServoSizes)sizeChoice->currentIndex());
    servo->setReinforce(reinforce->value());
    servo->setArmorSize((ServoSizes)armorChoice->currentIndex());
    servo->setArmorType((ArmorTypes)armorType->currentIndex());
    servo->setRAM((RAMTypes)ramType->currentIndex());
    mecha->addServo(servo);
    model->insertRows(model->rowCount(),1,QModelIndex());

    desigText->setText("");
    emit updatedServos();
}

void ServoEntry::refresh(Mecha* mecha)
{
    this->mecha = mecha;
    delete model;
    model = new ServoModel(mecha->getServoRef().length(),15,mecha->getServoRef());
    servoTable->setModel(model);
    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}

void ServoEntry::someDataChanged()
{
    emit updatedServos();
}

void ServoEntry::delServoClicked()
{
    if (!servoTable->selectionModel()->hasSelection())
    {
        return;
    }
    QItemSelection selection( servoTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];
    // we only allow one selection
    mecha->removeServo(index.row());

    model->removeRows(index.row(),1);

    emit updatedServos();
}
