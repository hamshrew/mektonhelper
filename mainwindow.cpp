#include "mainwindow.h"
#include "mecha.h"
#include "servo.h"
#include "servoentry.h"
#include "weaponentry.h"
#include "ammoentry.h"
#include "controlentry.h"
#include "propentry.h"
#include "mektoncomp.h"
#include "optionentry.h"
#include "shieldentry.h"
#include "mechafile.h"
#include "splitentry.h"
#include "costmultentry.h"
#include "poolentry.h"
#include "transentry.h"
#include "matedentry.h"
#include "htmlconverter.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QTabWidget>
#include <QProgressBar>
#include <QPalette>
//#include <QColorGroup>
#include <QColor>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QApplication>
#include <QFileDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPainter>
#include <QPrinter>
#include <QPdfWriter>
#include <QPrintDialog>
#include <QTextDocument>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mecha(new Mecha())
{
    QHBoxLayout* mainLayout = new QHBoxLayout(parent);

    QVBoxLayout* vitalStatBox = new QVBoxLayout(parent);

    QVBoxLayout* rightSide = new QVBoxLayout();

    addVitalStatWidgets(vitalStatBox);

    QTabWidget* workerTabs = new QTabWidget();

    ServoEntry* servoBox = new ServoEntry(mecha);

    connect(servoBox,SIGNAL(updatedServos()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),servoBox,SLOT(refresh(Mecha*)));

    WeaponEntry* weaponBox = new WeaponEntry(mecha);

    connect(this,SIGNAL(refreshAll(Mecha*)),weaponBox,SLOT(refresh(Mecha*)));

    ammoBox = new AmmoEntry(mecha);

    connect(ammoBox,SIGNAL(updatedAmmo()),SLOT(updateCall()));
    connect(weaponBox,SIGNAL(updatedWeapons()),SLOT(weaponUpdate()));
    connect(this,SIGNAL(refreshAll(Mecha*)),ammoBox,SLOT(refresh(Mecha*)));

    PoolEntry* poolBox = new PoolEntry(mecha);
    connect(poolBox,SIGNAL(updatedPools()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),poolBox,SLOT(refresh(Mecha*)));

    controlBox = new ControlEntry(mecha);

    connect(controlBox,SIGNAL(updatedControls()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),controlBox,SLOT(refresh(Mecha*)));

    propBox = new PropEntry(mecha);

    connect(propBox,SIGNAL(propChanged()),SLOT(updateCall()));
    connect(servoBox,SIGNAL(updatedServos()),propBox,SLOT(adjustSizes()));
    connect(this,SIGNAL(refreshAll(Mecha*)),propBox,SLOT(refresh(Mecha*)));

    optionBox = new OptionEntry(mecha);

    connect(optionBox,SIGNAL(optionsUpdated()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),optionBox,SLOT(refresh(Mecha*)));

    shieldBox = new ShieldEntry(mecha);

    connect(shieldBox,SIGNAL(shieldsUpdated()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),shieldBox,SLOT(refresh(Mecha*)));

    SplitEntry* splitBox = new SplitEntry(mecha);

    connect(splitBox,SIGNAL(splitsUpdated()),SLOT(updateCall()));
    connect(this,SIGNAL(refreshAll(Mecha*)),splitBox,SLOT(refresh(Mecha*)));
    connect(weaponBox,SIGNAL(updatedWeapons()),splitBox,SLOT(updateTables()));
    connect(controlBox,SIGNAL(updatedControls()),splitBox,SLOT(updateTables()));
    connect(shieldBox,SIGNAL(shieldsUpdated()),splitBox,SLOT(updateTables()));
    connect(poolBox,SIGNAL(updatedPools()),splitBox,SLOT(updateTables()));

    MatedEntry* matedBox = new MatedEntry(mecha);
    connect(this,SIGNAL(refreshAll(Mecha*)),matedBox,SLOT(refresh(Mecha*)));
    connect(matedBox,SIGNAL(matedChange()),SLOT(updateCall()));
    connect(matedBox,SIGNAL(matedChange()),splitBox,SLOT(updateTables()));

    CostMultEntry* multBox = new CostMultEntry(mecha);

    connect(multBox,SIGNAL(updatedMult()),SLOT(updateCall()));
    connect(multBox,SIGNAL(updatedMult()),splitBox,SLOT(updateTables()));
    connect(this,SIGNAL(refreshAll(Mecha*)),multBox,SLOT(refresh(Mecha*)));

    TransEntry* tfBox = new TransEntry(mecha);
    connect(tfBox,SIGNAL(tfChange()),SLOT(tfUpdate()));
    connect(this,SIGNAL(refreshAll(Mecha*)),tfBox,SLOT(refresh(Mecha*)));

    workerTabs->addTab(servoBox,QIcon(),tr("Servos"));
    workerTabs->addTab(weaponBox,QIcon(),tr("Weapons"));
    workerTabs->addTab(ammoBox,QIcon(),tr("Ammo"));
    workerTabs->addTab(poolBox,QIcon(),tr("Energy Pools"));
    workerTabs->addTab(controlBox,QIcon(),tr("Controls"));
    workerTabs->addTab(propBox,QIcon(),tr("Propulsion"));
    workerTabs->addTab(optionBox,QIcon(),tr("Options"));
    workerTabs->addTab(shieldBox,QIcon(),tr("Shields"));
    workerTabs->addTab(splitBox,QIcon(),tr("Splits"));
    workerTabs->addTab(matedBox,QIcon(),tr("Mating"));
    workerTabs->addTab(multBox,QIcon(),tr("Multipliers"));
    workerTabs->addTab(tfBox,QIcon(),tr("Transform"));

    QHBoxLayout* hRow = new QHBoxLayout();

    QLabel* label = new QLabel(tr("Mecha Name"));
    mechaName = new QLineEdit("");

    hRow->addWidget(label,0);
    hRow->addWidget(mechaName,1);

    rightSide->addLayout(hRow,0);
    rightSide->addWidget(workerTabs,1);

    mainLayout->addLayout(vitalStatBox,0);
    mainLayout->addLayout(rightSide,1);

    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);

    createMenus();
}

MainWindow::~MainWindow()
{

}

void MainWindow::addVitalStatWidgets(QVBoxLayout *layout)
{
    QVBoxLayout* boxStack = new QVBoxLayout;
    QHBoxLayout* boxPair = new QHBoxLayout;

    formChoice = new QComboBox();
    formChoice->addItems(mecha->getFormList());
    formChoice->setCurrentIndex(formChoice->findText(mecha->getDefaultFormName()));

    connect(formChoice,SIGNAL(currentIndexChanged(int)),SLOT(updateCall()));

    QLabel* label = new QLabel(tr("Form: "));

    boxPair->addWidget(label);
    boxPair->addWidget(formChoice);
    boxStack->addLayout(boxPair);

    QString s;

    boxPair = new QHBoxLayout;
    label = new QLabel(tr("Total Cost:"));
    s.sprintf("%.1f",mecha->getCost());
    totalCost = new QLabel(s);

    boxPair->addWidget(label);
    boxPair->addWidget(totalCost);
    boxStack->addLayout(boxPair);

    boxPair = new QHBoxLayout;
    label = new QLabel(tr("Frame Weight:"));
    s.sprintf("%.1f tons",mecha->getFrameWeight());
    frameWeight = new QLabel(s);

    boxPair->addWidget(label);
    boxPair->addWidget(frameWeight);
    boxStack->addLayout(boxPair);

    boxPair = new QHBoxLayout;
    label = new QLabel(tr("Total Weight:"));
    s.sprintf("%.1f tons",mecha->getWeight());
    totalWeight = new QLabel(s);

    boxPair->addWidget(label);
    boxPair->addWidget(totalWeight);
    boxStack->addLayout(boxPair);

    moveBox = new QGroupBox(tr("Movement"));
    QFont labelFont(label->font());
    labelFont.setUnderline(true);
    labelFont.setBold(true);
    moveBox->setFont(labelFont);
    boxStack->addWidget(moveBox);

    labelFont.setUnderline(false);
    labelFont.setBold(false);

    QList<QPair<QString,double> > moveList = mecha->getMoveList();

    QPair<QString,double> move;

    QVBoxLayout* boxLayout = new QVBoxLayout;

    Transformation tf = mecha->getForm(formChoice->currentText());

    foreach (move,moveList)
    {
        boxPair = new QHBoxLayout;
        label = new QLabel(move.first);
        label->setFont(labelFont);
        boxPair->addWidget(label);
        double modMove = move.second;
        modMove += tf.getMABonus(move.first);
        if (tf.getMAMult(move.first))
        {
            modMove *= tf.getMAMult(move.first);
        }
        int intMove = static_cast<int>(modMove);
        s.sprintf("%i MA (%.1fkph/%.1fmph)",intMove,
                MektonComp::convMAtoKPH(intMove),
                MektonComp::convMAtoMPH(intMove));
        label = new QLabel(s);
        label->setFont(labelFont);

        boxPair->addWidget(label);
        boxLayout->addLayout(boxPair);
    }

    moveBox->setLayout(boxLayout);

    boxPair = new QHBoxLayout;
    label = new QLabel(tr("Maneuver Value:"));
    s.sprintf("%i MV",mecha->getMV() + tf.getMV());
    mechMV = new QLabel(s);

    boxPair->addWidget(label);
    boxPair->addWidget(mechMV);
    boxStack->addLayout(boxPair);

    boxPair = new QHBoxLayout;
    label = new QLabel(tr(" Loaded MV:"));
    label->setToolTip(tr("The total MV after all Balance Modifiers from drop tanks, command armor, etc."));
    s.sprintf("%i MV",(int)(mecha->getMV() + tf.getMV() - ceil(mecha->getBMod())));
    totalMV = new QLabel(s);

    boxPair->addWidget(label);
    boxPair->addWidget(totalMV);
    boxStack->addLayout(boxPair);

    boxPair = new QHBoxLayout();
    label = new QLabel(tr("Maneuver Pool: "));
    label->setToolTip(tr("Maneuver Pool Adjustment to apply to the pilot's MP."));
    mechMP = new QLabel("+0%");
    if (mecha->getPoolBonus() != 0)
        mechMP->setText(QString::number(mecha->getPoolBonus()) + " +" + QString::number(mecha->getPoolMultiplier()) + "%");
    else mechMP->setText("+" + QString::number(mecha->getPoolMultiplier()) + "%");
    boxPair->addWidget(label);
    boxPair->addWidget(mechMP);
    boxStack->addLayout(boxPair);

    spaceBox = new QGroupBox(tr("Spaces Status"));

    boxStack->addWidget(spaceBox);

    fillSpaceBox();

    QGroupBox* group = new QGroupBox();
    group->setTitle("Computed Values");
    group->setLayout(boxStack);
    group->setMinimumWidth(250);
    layout->addWidget(group);
}

void MainWindow::fillSpaceBox()
{
    qDeleteAll(spaceBox->children());

    QString s;

    QVBoxLayout* spaceLayout = new QVBoxLayout();

    QHBoxLayout* hRow = new QHBoxLayout();

    QList<Servo*>& servos = mecha->getServoRef();

    QLabel* label = new QLabel(tr("Unallocated: "));
    s.sprintf("%.1f",mecha->getUnallocatedSpace());
    hRow->addWidget(label);
    label = new QLabel(s);
    hRow->addWidget(label);

    spaceLayout->addLayout(hRow);

    int hands = mecha->getNumHands();

    for (int i = 1;i <= hands;++i)
    {
        QString hStr(QString::number(i) + "H");
        hRow = new QHBoxLayout();
        label = new QLabel(hStr + ": ");
        label->setMinimumWidth(60);
        label->setMaximumWidth(150);
        hRow->addWidget(label);
        double used = mecha->getHeldWeaponSpace(i);
        QProgressBar* bar = new QProgressBar();
        QPalette pal = bar->palette();
        int hSpace = mecha->getMaxHandSpace(i);
        bar->setMaximum((int)(hSpace*10));
        bar->setMinimum(0);
        bar->setValue((int)((used*10 > bar->maximum() ? bar->maximum(): used*10)));
        s.sprintf("%.1f",used);
        bar->setFormat(s + "/" + QString::number(hSpace));
        bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:yellow}");
        if (used > (double)hSpace)
        {
            bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:red}");
        }
        bar->setPalette(pal);
        hRow->addWidget(bar);
        spaceLayout->addLayout(hRow);
    }

    Servo* ser;

    foreach (ser,servos)
    {
        if (!ser->getSpace())
            continue;

        hRow = new QHBoxLayout();
        label = new QLabel(ser->getDesignation() + ": ");
        label->setMinimumWidth(60);
        label->setMaximumWidth(150);
        hRow->addWidget(label);
        double used = mecha->getSpacesUsed(ser->getDesignation());
        QProgressBar* bar = new QProgressBar();
        QPalette pal = bar->palette();
        bar->setMaximum((int)((ser->getSpace() - mecha->getHydraulicsSpace())*10));
        // Make sure we haven't accidentally set a negative maximum
        if (bar->maximum() < 0)
            bar->setMaximum(0);
        bar->setMinimum(0);
        bar->setValue((int)((used*10 > bar->maximum() ? bar->maximum(): used*10)));
        s.sprintf("%.1f",used);
        bar->setFormat(s + "/" + QString::number(bar->maximum() / 10));
        bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:yellow}");
        if (used > (double)ser->getSpace())
        {
            bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:red}");
        }

        bar->setPalette(pal);
        hRow->addWidget(bar);
        spaceLayout->addLayout(hRow);
    }

    foreach (ser,servos)
    {
        for (int i = 1;i <= ser->getMounts();++i)
        {
            hRow = new QHBoxLayout();
            QString mStr(ser->getDesignation() + " Mount");
            if (ser->getType() == TORSO)
            {
                mStr += " " + QString::number(i);
            }
            label = new QLabel(mStr + ": ");
            label->setMinimumWidth(60);
            label->setMaximumWidth(150);
            hRow->addWidget(label);
            double used = mecha->getSpacesUsed(mStr);
            QProgressBar* bar = new QProgressBar();
            QPalette pal = bar->palette();
            bar->setMaximum((int)(ser->getKills()*10));
            bar->setMinimum(0);
            bar->setValue((int)((used*10 > bar->maximum() ? bar->maximum(): used*10)));
            s.sprintf("%.1f",used);
            bar->setFormat(s + "/" + QString::number(ser->getKills()));
            bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:yellow}");
            if (used > (double)ser->getKills())
            {
                bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:red}");
            }

            bar->setPalette(pal);
            hRow->addWidget(bar);
            spaceLayout->addLayout(hRow);
        }
    }

    QString sh;
    QStringList shList = mecha->getShieldNames();

    foreach (sh,shList)
    {
        if (!mecha->getBinderSpace(sh + " Binder"))
            continue;

        int bSpace = mecha->getBinderSpace(sh + " Binder");
        hRow = new QHBoxLayout();
        label = new QLabel(sh + " Binder" + ": ");
        label->setMinimumWidth(60);
        label->setMaximumWidth(150);
        hRow->addWidget(label);
        double used = mecha->getSpacesUsed(sh + " Binder");
        QProgressBar* bar = new QProgressBar();
        QPalette pal = bar->palette();
        bar->setMaximum((int)(bSpace*10));
        bar->setMinimum(0);
        bar->setValue((int)((used*10 > bar->maximum() ? bar->maximum(): used*10)));
        s.sprintf("%.1f",used);
        bar->setFormat(s + "/" + QString::number(bSpace));
        bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:yellow}");
        if (used > (double)bSpace)
        {
            bar->setStyleSheet("QProgressBar{text-align:center;border:1px solid gray;background:green}QProgressBar::chunk{background-color:red}");
        }

        bar->setPalette(pal);
        hRow->addWidget(bar);
        spaceLayout->addLayout(hRow);
    }

    spaceBox->setLayout(spaceLayout);
}

void MainWindow::updateStats()
{
    QString s;
    s.sprintf("%.1f",mecha->getCost());
    totalCost->setText(s);

    s.sprintf("%.1f tons",mecha->getFrameWeight());
    frameWeight->setText(s);

    s.sprintf("%.1f tons",mecha->getWeight());
    totalWeight->setText(s);

    qDeleteAll(moveBox->children());

    QList<QPair<QString,double> > moveList = mecha->getMoveList();

    QPair<QString,double> move;

    QVBoxLayout* boxLayout = new QVBoxLayout;

    QHBoxLayout* boxPair;

    QLabel* label;

    QFont labelFont;
    labelFont.setUnderline(false);
    labelFont.setBold(false);

    Transformation tf = mecha->getForm(formChoice->currentText());

    foreach (move,moveList)
    {
        double modMove = move.second;
        modMove += tf.getMABonus(move.first);
        if (tf.getMAMult(move.first))
        {
            modMove *= tf.getMAMult(move.first);
        }
        int intMove = static_cast<int>(modMove);
        boxPair = new QHBoxLayout;
        label = new QLabel(move.first);
        label->setFont(labelFont);
        boxPair->addWidget(label);
        s.sprintf("%i MA (%.1fkph/%.1fmph)",intMove,
                MektonComp::convMAtoKPH(intMove),
                MektonComp::convMAtoMPH(intMove));
        label = new QLabel(s);
        label->setFont(labelFont);

        boxPair->addWidget(label);
        boxLayout->addLayout(boxPair);
    }

    moveBox->setLayout(boxLayout);

    s.sprintf("%i MV",mecha->getMV() + tf.getMV());
    mechMV->setText(s);

    s.sprintf("%i MV",(int)(mecha->getMV() + tf.getMV() - ceil(mecha->getBMod())));
    totalMV->setText(s);

    if (mecha->getPoolBonus() != 0)
        mechMP->setText(QString::number(mecha->getPoolBonus()) + " +" + QString::number(mecha->getPoolMultiplier()) + "%");
    else mechMP->setText("+" + QString::number(mecha->getPoolMultiplier()) + "%");

    fillSpaceBox();
}

void MainWindow::updateCall()
{
    mecha->cleanup();
    controlBox->refreshLocs();
    optionBox->updateServos();
    updateStats();
}

void MainWindow::weaponUpdate()
{
    ammoBox->weaponsUpdated();
    updateStats();
}

void MainWindow::tfUpdate()
{
    disconnect(formChoice,SIGNAL(currentIndexChanged(int)),this,SLOT(updateCall()));
    formChoice->clear();
    formChoice->addItems(mecha->getFormList());
    formChoice->setCurrentIndex(formChoice->findText(mecha->getDefaultFormName()));
    connect(formChoice,SIGNAL(currentIndexChanged(int)),SLOT(updateCall()));
    updateStats();
}

void MainWindow::createMenus()
{
    createActions();
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(printAction);

    QMenu* exportMenu = fileMenu->addMenu(tr("&Export"));
    exportMenu->addAction(exportPDFAction);
    exportMenu->addAction(exportHTMLAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createActions()
{
    saveAction = new QAction(tr("&Save As..."),this);
    saveAction->setStatusTip(tr("Save the current Mekton to file."));
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction,SIGNAL(triggered()),SLOT(saveMecha()));

    openAction = new QAction(tr("&Open"),this);
    openAction->setStatusTip(tr("Open a saved Mekton."));
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction,SIGNAL(triggered()),SLOT(loadMecha()));

    printAction = new QAction(tr("&Print"),this);
    printAction->setShortcuts(QKeySequence::Print);
    printAction->setStatusTip(tr("Print current Mekton."));
    connect(printAction,SIGNAL(triggered()),SLOT(printMecha()));

    exportPDFAction = new QAction(tr("...to &PDF"),this);
    exportPDFAction->setStatusTip(tr("Export Mekton to PDF."));
    connect(exportPDFAction,SIGNAL(triggered()),SLOT(exportPDF()));

    exportHTMLAction = new QAction(tr("...to &HTML"),this);
    exportHTMLAction->setStatusTip(tr("Export Mekton to HTML."));
    connect(exportHTMLAction,SIGNAL(triggered()),SLOT(exportHTML()));

    aboutAction = new QAction(tr("About &MektonHelper"),this);
    aboutAction->setStatusTip(tr("About this program."));
    aboutAction->setMenuRole(QAction::AboutRole);
    connect(aboutAction,SIGNAL(triggered()),SLOT(aboutWin()));
}

void MainWindow::aboutWin()
{
    QMessageBox msg;
    msg.setText(tr("MektonHelper v0.8 (September 2013)\nWritten by Andrew Williams\nMekton is a trademark of R.Talsorian Games\nR.Talsorian is unaffiliated with this program.\nFor support, go to http://www.greylotus.org"));
    msg.exec();
}

void MainWindow::saveMecha()
{
    QString defaultName = mechaName->text();
    defaultName.append(".mek");
    QString filename = QFileDialog::getSaveFileName(this,"Save Mecha as...",defaultName,"*.mek");

    if (filename.isEmpty())
    {
        return;
    }

    MechaFile mFile;

    mecha->setName(mechaName->text());

    mFile.setMecha(mecha);

    if (!mFile.writeFile(filename))
    {
        QMessageBox msgBox;
        msgBox.setText("Problem writing to file. Mecha not saved.");
        msgBox.exec();
    }
}

void MainWindow::loadMecha()
{
    QString fileName = QFileDialog::getOpenFileName(this,"Load Mecha...","","Mek File (*.mek);;All Files (*.*)");
    if (fileName.isEmpty())
    {
        return;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Could not open Mecha file for reading.");
        msgBox.exec();
        return;
    }

    MechaFile mechFile;
    if (!mechFile.parseFile(&file))
    {
        QMessageBox msgBox;
        msgBox.setText("Mecha File chosen is not valid or corrupted.");
        msgBox.exec();
        file.close();
        return;
    }

    mechaName->setText(mechFile.getMecha()->getName());

    file.close();

    emit refreshAll(mechFile.getMecha());
    delete mecha;
    mecha = mechFile.getMecha();
    tfUpdate();
    updateCall();
}

void MainWindow::printMecha()
{
    mecha->setName(mechaName->text());

    QPrinter printer;

    if (!printer.isValid())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("No valid printer selected. Please set up your printer."));
        msgBox.exec();
    }

    QPrintDialog dlg(&printer);

    if (dlg.exec() == QDialog::Accepted)
    {
        QTextDocument sheet;

        sheet.setDocumentMargin(0.2);

        QSizeF paperSize;
        paperSize.setWidth(printer.width());
        paperSize.setHeight(printer.height());

        sheet.setPageSize(paperSize);

        QString html;

        HtmlConverter::htmlize(mecha,html);

        sheet.setHtml(html);

        sheet.print(&printer);
    }
}

void MainWindow::exportPDF()
{
    QString defaultName = mechaName->text();
    defaultName.append(".pdf");
    QString filename = QFileDialog::getSaveFileName(this,"Export Mecha as...",defaultName,"*.pdf");

    if (filename.isEmpty())
    {
        return;
    }

    mecha->setName(mechaName->text());

    QPrinter printer(QPrinter::ScreenResolution);

    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPaperSize(QPrinter::Letter);

//    QPrintDialog dlg(&printer);

//    if (dlg.exec() == QDialog::Accepted)
    {
        QTextDocument sheet;

        sheet.setDocumentMargin(0.2);

        QSizeF paperSize;
        paperSize.setWidth(printer.width());
        paperSize.setHeight(printer.height());

        sheet.setPageSize(paperSize);

        QString html;

        HtmlConverter::htmlize(mecha,html);

        sheet.setHtml(html);

        sheet.print(&printer);
    }
}

void MainWindow::exportHTML()
{
    QString defaultName = mechaName->text();
    defaultName.append(".html");
    QString filename = QFileDialog::getSaveFileName(this,"Save Mecha as...",defaultName,"*.html");

    if (filename.isEmpty())
    {
        return;
    }

    mecha->setName(mechaName->text());

    QString html;

    HtmlConverter::htmlize(mecha,html);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox msgBox;
        msgBox.setText("Problem writing to file. Mecha not export.");
        msgBox.exec();
        return;
    }

    QTextStream stream(&file);
    stream.setCodec("UTF-8");
    stream << html;

}
