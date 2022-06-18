#include "weaponentry.h"
#include "weaponmodel.h"
#include "mecha.h"
#include "specwindow.h"
#include "specfile.h"
#include "specmodel.h"
#include "loclistdelegate.h"

#include <QTableView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QScrollArea>

WeaponEntry::WeaponEntry(Mecha* mecha,QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout();
    QVBoxLayout* entryLayout = new QVBoxLayout();
    weaponTable = new QTableView();

    QList<Weapon*>& weaponList = mecha->getWeaponsRef();
    model = new WeaponModel(mecha, weaponList);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));

    weaponTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    weaponTable->setSelectionMode(QAbstractItemView::ExtendedSelection);
    weaponTable->setModel(model);

    // Set column widths
    weaponTable->setColumnWidth(0,200);
    weaponTable->setColumnWidth(1,35);
    weaponTable->setColumnWidth(2,55);
    weaponTable->setColumnWidth(3,50);
    weaponTable->setColumnWidth(4,40);
    weaponTable->setColumnWidth(5,40);
    weaponTable->setColumnWidth(6,100);
    weaponTable->setColumnWidth(7,50);
    weaponTable->setColumnWidth(8,40);
    weaponTable->setColumnWidth(9,150);

    locDelegate = new LocListDelegate(mecha);

    weaponTable->setItemDelegateForColumn(6,locDelegate);

    entryLayout->addWidget(weaponTable);

    QHBoxLayout* manipLine = new QHBoxLayout();
    QPushButton* addWeapon = new QPushButton(tr("Add Weapon ^"));
    QPushButton* removeWeapon = new QPushButton(tr("v Remove Weapon(s)"));

    manipLine->addWidget(addWeapon);
    manipLine->addWidget(removeWeapon);

    connect(addWeapon,SIGNAL(clicked()),SLOT(addSelectedWeapon()));
    connect(removeWeapon,SIGNAL(clicked()),SLOT(removeSelectedWeapon()));

    QPushButton* linkWeapon = new QPushButton(tr("Link Weapon(s)"));
    QPushButton* xlinkWeapon = new QPushButton(tr("Cross-Link Weapon(s)"));
    QPushButton* unlinkWeapon = new QPushButton(tr("Unlink Weapon(s)"));

    manipLine->addWidget(linkWeapon);
    manipLine->addWidget(xlinkWeapon);
    manipLine->addWidget(unlinkWeapon);

    connect(linkWeapon,SIGNAL(clicked()),SLOT(linkWeapons()));
    connect(xlinkWeapon,SIGNAL(clicked()),SLOT(crossLinkWeapons()));
    connect(unlinkWeapon,SIGNAL(clicked()),SLOT(unlinkWeapons()));

    entryLayout->addLayout(manipLine);

    specTable = new QTableView();

    specModel = new SpecModel(specList);

    specTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    specTable->setSelectionMode(QAbstractItemView::SingleSelection);
    specTable->setModel(specModel);

    // Set column widths
    specTable->setColumnWidth(0,200);
    specTable->setColumnWidth(1,30);
    specTable->setColumnWidth(2,50);
    specTable->setColumnWidth(3,55);
    specTable->setColumnWidth(4,40);
    specTable->setColumnWidth(5,40);
    specTable->setColumnWidth(6,30);
    specTable->setColumnWidth(7,45);
    specTable->setColumnWidth(8,150);

    QHBoxLayout* specLayout = new QHBoxLayout();
    specLayout->addWidget(specTable);
    entryLayout->addLayout(specLayout);

    QHBoxLayout* specButtons = new QHBoxLayout();

    QPushButton* loadSpec = new QPushButton(tr("Load Weapon Spec"));

    connect(loadSpec,SIGNAL(clicked()),SLOT(openSpecFile()));

    QPushButton* editSpec = new QPushButton(tr("Edit/Create Weapon Spec"));

    connect(editSpec,SIGNAL(clicked()),SLOT(createSpecWindow()));

    specButtons->addWidget(loadSpec);
    specButtons->addWidget(editSpec);

    entryLayout->addLayout(specButtons);

    baseWidget->setLayout(entryLayout);
    baseWidget->adjustSize();
    scrollArea->setWidget(baseWidget);
    layout->addWidget(scrollArea);
    this->setLayout(layout);
}

void WeaponEntry::someDataChanged()
{
    emit updatedWeapons();
}

void WeaponEntry::openSpecFile()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Spec File"),"",tr("Spec Files (*.spec);;All Files (*.*)"));
    if (filename.isEmpty())
    {
        return;
    }

    loadFile(filename);
}

void WeaponEntry::loadFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not open spec file for reading.");
        msgBox.exec();
        return;
    }

    SpecFile specFile;
    if (!specFile.parseFile(&file))
    {
        QMessageBox msgBox;
        msgBox.setText("Spec File chosen is not valid or corrupted.");
        msgBox.exec();
        file.close();
        return;
    }

    delete specModel;

    specList = specFile.getSpecs();

    specModel = new SpecModel(specList);

    specTable->setModel(specModel);

    file.close();
}

void WeaponEntry::createSpecWindow()
{
    SpecWindow* newWindow = new SpecWindow(0);
    newWindow->show();
}

void WeaponEntry::addSelectedWeapon()
{
    if (!specTable->selectionModel()->hasSelection())
        return;

    QItemSelection selection(specTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    // we only allow one selection
    Weapon* wp = new Weapon(specList[index.row()]);
    mecha->addWeapon(wp);

    model->insertRows(model->rowCount(),1);

    emit updatedWeapons();
}

void WeaponEntry::removeSelectedWeapon()
{
    if (!weaponTable->selectionModel()->hasSelection())
        return;

    QModelIndexList selection(weaponTable->selectionModel()->selectedRows() );

    int numSelect = selection.length();

    for (int i =0;i < numSelect;++i)
    {
        int sel = selection.at(i).row();

        mecha->removeWeapon(sel-i);
    }

    model->removeRows(0,numSelect);
    emit updatedWeapons();
}

void WeaponEntry::linkWeapons()
{
    if (!weaponTable->selectionModel()->hasSelection())
        return;

    QModelIndexList selection(weaponTable->selectionModel()->selectedRows() );

    int numSelect = selection.length();

    if (numSelect < 2)
        return;

    // get the base weapon
    Weapon* wp = mecha->getWeaponsRef()[selection.at(0).row()];

    for (int i = 1;i < numSelect;++i)
    {
        int sel = selection.at(i).row();

        wp->addLink(mecha->getWeaponsRef()[sel],false);
    }

    model->update();

    emit updatedWeapons();
}

void WeaponEntry::crossLinkWeapons()
{
    if (!weaponTable->selectionModel()->hasSelection())
        return;

    QModelIndexList selection(weaponTable->selectionModel()->selectedRows() );

    int numSelect = selection.length();

    if (numSelect < 2)
        return;

    // get the base weapon
    Weapon* wp = mecha->getWeaponsRef()[selection.at(0).row()];

    for (int i = 1;i < numSelect;++i)
    {
        int sel = selection.at(i).row();

        wp->addLink(mecha->getWeaponsRef()[sel],true);
    }

    model->update();

    emit updatedWeapons();
}

void WeaponEntry::unlinkWeapons()
{
    if (!weaponTable->selectionModel()->hasSelection())
        return;

    QModelIndexList selection(weaponTable->selectionModel()->selectedRows() );

    int numSelect = selection.length();

    for (int i =0;i < numSelect;++i)
    {
        int sel = selection.at(i).row();

        mecha->getWeaponsRef()[sel]->clearLinks();
    }

    model->update();
    emit updatedWeapons();
}

void WeaponEntry::refresh(Mecha *newMech)
{
    mecha = newMech;

    delete model;
    model = new WeaponModel(mecha,mecha->getWeaponsRef());
    weaponTable->setModel(model);

    locDelegate = new LocListDelegate(mecha);

    weaponTable->setItemDelegateForColumn(6,locDelegate);

    connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),SLOT(someDataChanged()));
}
