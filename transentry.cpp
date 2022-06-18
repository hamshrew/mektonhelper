#include "transentry.h"
#include "mecha.h"
#include "tffile.h"

#include <QGroupBox>
#include <QLabel>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSignalMapper>
#include <QGridLayout>
#include <QScrollArea>
#include <QComboBox>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QResource>

TransEntry::TransEntry(Mecha* mecha, QWidget *parent) :
    QWidget(parent),
    mecha(mecha)
{
    loadTransformations();
    QVBoxLayout* mainLayout = new QVBoxLayout();
    QScrollArea* scrollArea = new QScrollArea();
    QWidget* baseWidget = new QWidget();
    gridLayout = new QGridLayout();

    // Set up default form selector
    defaultSelect = new QComboBox();
    defaultSelect->addItems(mecha->getFormList());
    defaultSelect->setCurrentIndex(defaultSelect->findText(mecha->getDefaultFormName()));
    if (mecha->hasForm("Humanoid"))
        defaultSelect->setDisabled(true);

    connect(defaultSelect,SIGNAL(currentIndexChanged(int)),SLOT(defaultChange()));

    QLabel* label = new QLabel("Default Form");

    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(label,0);
    row->addWidget(defaultSelect,1);
    gridLayout->addLayout(row,0,0,1,4);

    Transformation tf;

    int rows = 1;
    int columns = 0;

    foreach (tf,transSpecList)
    {
        gridLayout->addWidget(makeTFBox(tf),rows,columns);
        ++columns;
        if (columns > 3)
        {
            columns = 0;
            ++rows;
        }
    }

    baseWidget->setLayout(gridLayout);
    scrollArea->setWidget(baseWidget);

    mainLayout->addWidget(scrollArea);

    this->setLayout(mainLayout);
}

TransEntry::~TransEntry()
{
}

void TransEntry::defaultChange()
{
    mecha->setDefaultForm(defaultSelect->currentText());
    emit tfChange();
}

void TransEntry::resetDefaultBox()
{
    disconnect(defaultSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(defaultChange()));
    defaultSelect->clear();
    defaultSelect->addItems(mecha->getFormList());
    defaultSelect->setCurrentIndex(defaultSelect->findText(mecha->getDefaultFormName()));

    if (mecha->getDefaultFormName() == "Humanoid")
        defaultSelect->setDisabled(true);
    else defaultSelect->setDisabled(false);

    connect(defaultSelect,SIGNAL(currentIndexChanged(int)),SLOT(defaultChange()));
}

QGroupBox* TransEntry::makeTFBox(const Transformation &tf)
{
    QGroupBox* tfBox = new QGroupBox(tf.getName());

    QVBoxLayout* boxLayout = new QVBoxLayout();

    QHBoxLayout* row = new QHBoxLayout();

    QLabel* label = new QLabel("Cost: ");
    row->addWidget(label);
    label = new QLabel(QString("x") + QString::number(tf.getCost()));
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("MV Modifier: ");
    row->addWidget(label);
    label = new QLabel(QString::number(tf.getMV()));
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("MA Modifiers: ");
    row->addWidget(label);
    label = new QLabel(tf.getAllMABonus());
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("MA Multipliers: ");
    row->addWidget(label);
    label = new QLabel(tf.getAllMAMult());
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("Propulsion: ");
    row->addWidget(label);
    label = new QLabel(tf.getProp().join(", "));
    label->setWordWrap(true);
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("Hardpoints: ");
    row->addWidget(label);
    label = new QLabel(tf.getHardPoints());
    label->setWordWrap(true);
    row->addWidget(label);
    boxLayout->addLayout(row);

    row = new QHBoxLayout();
    label = new QLabel("Special: ");
    row->addWidget(label);
    label = new QLabel(tf.getSpecial());
    label->setWordWrap(true);
    row->addWidget(label);
    boxLayout->addLayout(row);

    QCheckBox* hasForm = new QCheckBox("Select: ");

    QSignalMapper* mapper = new QSignalMapper(this);

    // handle special case if Humanoid
    if (tf.getName() == "Humanoid")
    {
        mekBox = hasForm;
        mekMapper = mapper;
    }

    boxLayout->addWidget(hasForm);

    if (mecha->hasForm(tf.getName()))
    {
        hasForm->setChecked(true);
    }

    mapper->setMapping(hasForm,tf.getName());

    connect(hasForm,SIGNAL(clicked()),mapper,SLOT(map()));
    connect(mapper,SIGNAL(mapped(QString)),this,SLOT(toggleForm(QString)));

    tfBox->setLayout(boxLayout);

    tfBox->setMaximumWidth(200);

    return tfBox;
}

void TransEntry::toggleForm(QString name)
{
    if (mecha->hasForm(name))
    {
        mecha->remForm(name);
    }
    else
    {
        Transformation tf;
        foreach (tf,transSpecList)
        {
            if (tf.getName() == name)
            {
                mecha->addForm(tf);
                break;
            }
        }
    }

    if (mecha->getFormList().size() == 1 && mecha->getDefaultFormName() == "Humanoid")
    {
        disconnect(mekBox,SIGNAL(clicked()),mekMapper,SLOT(map()));
        mekBox->setChecked(true);
        connect(mekBox,SIGNAL(clicked()),mekMapper,SLOT(map()));
        mekBox->setDisabled(true);
    }
    else mekBox->setEnabled(true);

    resetDefaultBox();
    emit tfChange();
}

void TransEntry::loadTransformations()
{
    Transformation tf("Humanoid");

    QStringList mekForms = mecha->getFormList();

    QString form;

    // First add any custom Mekton forms
    foreach (form,mekForms)
    {
        transSpecList.append(mecha->getForm(form));
    }

    // Load XML file here
    QFile file(":/support/forms.mtf");
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText(tr(QString("Could not open Transformations file for reading. The program will use defaults.").toStdString().c_str()));
        msgBox.exec();
        if (!mekForms.contains("Humanoid"))
        {
            tf.addProp("Legs");
            tf.addProp("GES");
            tf.addProp("Thrusters");
            tf.addProp("Gravitics");
            tf.addProp("Hydrojets");
            tf.addProp("Wheels");
            tf.addProp("Treads");
            transSpecList.append(tf);
        }
        return;
    }

    TFFile tfFile;
    if (!tfFile.parseFile(&file))
    {
        QMessageBox msgBox;
        msgBox.setText("Transformations File chosen is not valid or corrupted. The program will use defaults.");
        msgBox.exec();
        file.close();
        if (!mekForms.contains("Humanoid"))
        {
            tf.addProp("Legs");
            tf.addProp("GES");
            tf.addProp("Thrusters");
            tf.addProp("Gravitics");
            tf.addProp("Hydrojets");
            tf.addProp("Wheels");
            tf.addProp("Treads");
            transSpecList.append(tf);
        }
        return;
    }

    QList<Transformation> tfList = tfFile.getForms();

    foreach (tf,tfList)
    {
        if (!mekForms.contains(tf.getName()))
        {
            transSpecList.append(tf);
        }
    }

    file.close();

}

void TransEntry::refresh(Mecha *newMech)
{
    mecha = newMech;

    disconnect(defaultSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(defaultChange()));

    qDeleteAll(gridLayout->children());

    // Set up default form selector
    defaultSelect = new QComboBox();
    defaultSelect->addItems(mecha->getFormList());
    defaultSelect->setCurrentIndex(defaultSelect->findText(mecha->getDefaultFormName()));
    if (mecha->hasForm("Humanoid"))
        defaultSelect->setDisabled(true);

    connect(defaultSelect,SIGNAL(currentIndexChanged(int)),SLOT(defaultChange()));

    QLabel* label = new QLabel("Default Form");

    QHBoxLayout* row = new QHBoxLayout();
    row->addWidget(label,0);
    row->addWidget(defaultSelect,1);
    gridLayout->addLayout(row,0,0,1,4);

    Transformation tf;

    int rows = 1;
    int columns = 0;

    foreach (tf,transSpecList)
    {
        gridLayout->addWidget(makeTFBox(tf),rows,columns);
        ++columns;
        if (columns > 3)
        {
            columns = 0;
            ++rows;
        }
    }
}
