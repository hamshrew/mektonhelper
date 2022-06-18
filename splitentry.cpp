#include "splitentry.h"
#include "splittablemodel.h"
#include "splitmodel.h"
#include "dspinboxdelegate.h"
#include "mecha.h"

#include <QTableView>
#include <QHBoxLayout>
#include <QScrollArea>

SplitEntry::SplitEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    splitList = mecha->getSplittableParts();

    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* mainLayout = new QHBoxLayout();

    partsTable = new QTableView();
    partsModel = new SplittableModel(splitList);

    partsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    partsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    partsTable->setModel(partsModel);

    connect(partsTable,SIGNAL(clicked(QModelIndex)),SLOT(newSelect(QModelIndex)));

    mainLayout->addWidget(partsTable);

    splitTable = new QTableView();

    splitTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    splitTable->setSelectionMode(QAbstractItemView::SingleSelection);

    splitModel = 0;

    DSpinBoxDelegate* spDel = new DSpinBoxDelegate(0,500);

    splitTable->setItemDelegateForColumn(1,spDel);

    mainLayout->addWidget(splitTable);

    baseWidget->setLayout(mainLayout);
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

void SplitEntry::someDataChanged()
{
    emit splitsUpdated();
}

void SplitEntry::refresh(Mecha* mecha)
{
    this->mecha = mecha;
    if (splitModel)
        delete splitModel;

    delete partsModel;

    splitList = mecha->getSplittableParts();
    partsModel = new SplittableModel(splitList);
    partsTable->setModel(partsModel);
}

void SplitEntry::newSelect(QModelIndex index)
{
    if (splitModel)
        delete splitModel;

    splitModel = new SplitModel(mecha,splitList[index.row()]);
    connect(splitModel,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    splitTable->setModel(splitModel);
}

void SplitEntry::updateTables()
{
    delete partsModel;

    splitList = mecha->getSplittableParts();
    partsModel = new SplittableModel(splitList);
    partsTable->setModel(partsModel);
}
