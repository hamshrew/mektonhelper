#include "matedmodel.h"
#include "matedentry.h"
#include "mecha.h"
#include "loclistdelegate.h"
#include "matedelegate.h"
#include "matedsystem.h"

#include <QTableView>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QScrollArea>

MatedEntry::MatedEntry(Mecha* mecha,QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(buildMatedBox(),1);
//    layout->addWidget(buildWeaponView());
//    layout->addWidget(buildShieldView());

    baseWidget->setLayout(layout);
    scrollArea->setWidget(baseWidget);
    mainLayout->addWidget(scrollArea);
    this->setLayout(mainLayout);
}

QGroupBox* MatedEntry::buildMatedBox()
{
    QGroupBox* newBox = new QGroupBox(tr("Mated Systems"));

    QVBoxLayout* layout = new QVBoxLayout();

    matedTable = new QTableView();
    matedModel = new MatedModel(mecha);
    matedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    matedTable->setSelectionMode(QAbstractItemView::SingleSelection);

    matedTable->setModel(matedModel);

    LocListDelegate* llDel = new LocListDelegate(mecha);
    MateDelegate* matedDel = new MateDelegate(mecha);

    matedTable->setItemDelegateForColumn(1,llDel);
    matedTable->setItemDelegateForColumn(2,matedDel);
    matedTable->setItemDelegateForColumn(3,matedDel);

    matedTable->setColumnWidth(0,150);
    matedTable->setColumnWidth(1,70);
    matedTable->setColumnWidth(2,150);
    matedTable->setColumnWidth(3,150);
    matedTable->setColumnWidth(4,70);
    matedTable->setColumnWidth(5,70);

    connect(matedModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    layout->addWidget(matedTable);

    QHBoxLayout* row = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Name"));
    sysName = new QLineEdit("");

    row->addWidget(label);
    row->addWidget(sysName);

    layout->addLayout(row);

    row = new QHBoxLayout();
    QPushButton* addButton = new QPushButton(tr("Add System"));
    QPushButton* remButton = new QPushButton(tr("Remove System"));

    connect(addButton,SIGNAL(clicked()),SLOT(addSystem()));
    connect(remButton,SIGNAL(clicked()),SLOT(remSystem()));

    row->addWidget(addButton);
    row->addWidget(remButton);

    layout->addLayout(row);

    newBox->setLayout(layout);
    newBox->setMinimumWidth(700);
    return newBox;
}

void MatedEntry::someDataChanged()
{
    emit matedChange();
}

void MatedEntry::addSystem()
{
    if (sysName->text().isEmpty())
    {
        return;
    }

    MatedSystem* ms = new MatedSystem();
    ms->setName(sysName->text());

    mecha->addMatedSystem(ms);

    matedModel->insertRows(matedModel->rowCount(),1,QModelIndex());

    emit matedChange();
}

void MatedEntry::remSystem()
{
    if (!matedTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(matedTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    // we only allow one selection

    QString msName(matedModel->data(index).toString());

    matedModel->removeRows(index.row(),1);
    mecha->remMatedSystem(msName);
    emit matedChange();
}

void MatedEntry::refresh(Mecha* newMech)
{
    mecha = newMech;

    delete matedModel;

    matedModel = new MatedModel(mecha);
    matedTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    matedTable->setSelectionMode(QAbstractItemView::SingleSelection);

    matedTable->setModel(matedModel);

    LocListDelegate* llDel = new LocListDelegate(mecha);
    MateDelegate* matedDel = new MateDelegate(mecha);

    matedTable->setItemDelegateForColumn(1,llDel);
    matedTable->setItemDelegateForColumn(2,matedDel);
    matedTable->setItemDelegateForColumn(3,matedDel);

    matedTable->setColumnWidth(0,150);
    matedTable->setColumnWidth(1,70);
    matedTable->setColumnWidth(2,150);
    matedTable->setColumnWidth(3,150);
    matedTable->setColumnWidth(4,70);
    matedTable->setColumnWidth(5,70);

    connect(matedModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}
