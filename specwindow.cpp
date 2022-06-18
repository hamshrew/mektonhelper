#include "specwindow.h"
#include "basespecentry.h"
#include "beamspecentry.h"
#include "meleespecentry.h"
#include "missilespecentry.h"
#include "projspecentry.h"
#include "specmodel.h"
#include <QComboBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QLineEdit>
#include <QLabel>
#include <QTextEdit>
#include "specfile.h"
#include "emwspecentry.h"

SpecWindow::SpecWindow(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* firstLine = new QHBoxLayout();
    QLabel* label = new QLabel(tr("Title: "));
    firstLine->addWidget(label);
    specTitle = new QLineEdit("");
    label = new QLabel(tr("Author"));
    specAuthor = new QLineEdit("");
    firstLine->addWidget(specTitle);
    firstLine->addWidget(label);
    firstLine->addWidget(specAuthor);
    layout->addLayout(firstLine);

    specTable = new QTableView();

    model = new SpecModel(weaponList);

    specTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    specTable->setSelectionMode(QAbstractItemView::SingleSelection);
    specTable->setModel(model);

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

    layout->addWidget(specTable);

    QHBoxLayout* removeLine = new QHBoxLayout();
    QPushButton* removeButton = new QPushButton(tr("Remove Weapon"));

    connect(removeButton,SIGNAL(clicked()),SLOT(removeWeapon()));

    removeLine->addWidget(removeButton);
    layout->addLayout(removeLine);

    QHBoxLayout* buttonLine = new QHBoxLayout();

    QPushButton* loadButton = new QPushButton(tr("Load Spec"));
    QPushButton* saveButton = new QPushButton(tr("Save Spec"));

    connect(loadButton,SIGNAL(clicked()),SLOT(loadNewSpec()));
    connect(saveButton,SIGNAL(clicked()),SLOT(writeNewSpec()));

    buttonLine->addWidget(loadButton);
    buttonLine->addWidget(saveButton);

    layout->addLayout(buttonLine);

    selectType = new QComboBox(this);

    selectType->addItem("Beam Weapon");
    selectType->addItem("Energy Melee Weapon");
    selectType->addItem("Melee Weapon");
    selectType->addItem("Missiles");
    selectType->addItem("Projectile Weapon");
    selectType->setCurrentIndex(0);
    layout->addWidget(selectType);

    connect(selectType,SIGNAL(currentIndexChanged(int)),SLOT(changeEntryType(int)));

    specBox = new QGroupBox(selectType->currentText());
    needLayout = new QVBoxLayout();
    wSpec = new BeamSpecEntry(this);
    needLayout->addWidget(wSpec);
    specBox->setLayout(needLayout);
    layout->addWidget(specBox);

    QGroupBox* comments = new QGroupBox("Comments");

    QVBoxLayout* comLayout = new QVBoxLayout();

    commentBox = new QTextEdit();
    commentBox->setAcceptRichText(false);

    comLayout->addWidget(commentBox);
    comments->setLayout(comLayout);

    layout->addWidget(comments);

    QPushButton* addWeaponButton = new QPushButton(tr("Add Weapon"));

    connect(addWeaponButton,SIGNAL(clicked()),SLOT(addCurrentWeapon()));

    layout->addWidget(addWeaponButton);

    this->setLayout(layout);
}

void SpecWindow::changeEntryType(int type)
{
    specBox->setTitle(selectType->currentText());
    qDeleteAll(specBox->children());
    switch (type)
    {
    case 0: wSpec = new BeamSpecEntry(this);
        break;
    case 1: wSpec = new EMWSpecEntry(this);
        break;
    case 2: wSpec = new MeleeSpecEntry(this);
        break;
    case 3: wSpec = new MissileSpecEntry(this);
        break;
    case 4: wSpec = new ProjSpecEntry(this);
        break;
    default: wSpec = new BeamSpecEntry(this);
        break;
    }

    needLayout = new QVBoxLayout();
    needLayout->addWidget(wSpec);
    specBox->setLayout(needLayout);
    wSpec->show();
    return;
}

void SpecWindow::addCurrentWeapon()
{
    Weapon* newWeapon = wSpec->createWeaponPtr();

    if (newWeapon == NULL)
    {
        return;
    }

    newWeapon->setComments(commentBox->document()->toPlainText());

    weaponList.push_back(*newWeapon);

    qSort(weaponList);

    delete newWeapon;

    model->insertRows(model->rowCount(),1);
}

void SpecWindow::loadNewSpec()
{
    currentSpecName = QFileDialog::getOpenFileName(this,"Load Spec File...","","Spec File (*.spec);;All Files (*.*)");
    if (currentSpecName.isEmpty())
    {
        return;
    }

    QFile file(currentSpecName);
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

    weaponList = specFile.getSpecs();

    qSort(weaponList);

    delete model;
    model = new SpecModel(weaponList);
    specTable->setModel(model);

    specTitle->setText(specFile.getTitle());
    specAuthor->setText(specFile.getAuthor());

    file.close();
}

void SpecWindow::writeNewSpec()
{
    if (weaponList.isEmpty())
    {
        QMessageBox msgBox;
        msgBox.setText("No Specs to save!");
        msgBox.exec();
        return;
    }



    currentSpecName = QFileDialog::getSaveFileName(this,"Save Spec File as...","","*.spec");

    if (currentSpecName.isEmpty())
    {
        return;
    }

    SpecFile specFile;
    specFile.setSpecs(weaponList);
    if (!specFile.writeFile(currentSpecName,specTitle->text(),specAuthor->text()))
    {
        QMessageBox msgBox;
        msgBox.setText("Problem writing to file. Specs not saved.");
        msgBox.exec();
    }
}

void SpecWindow::removeWeapon()
{
    if (!specTable->selectionModel()->hasSelection())
    {
        return;
    }

    QItemSelection selection( specTable->selectionModel()->selection() );

    QModelIndex index = selection.indexes()[0];

    // we only allow one selection
    weaponList.removeAt(index.row());

    model->removeRows(index.row(),1);
}
