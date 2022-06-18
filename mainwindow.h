#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward Declarations
class QVBoxLayout;
class Mecha;
class QLabel;
class QGroupBox;
class AmmoEntry;
class ControlEntry;
class PropEntry;
class OptionEntry;
class ShieldEntry;
class QMenu;
class QAction;
class QComboBox;
class QLineEdit;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Mecha* mecha;
    QLabel* totalCost;
    QLabel* frameWeight;
    QLabel* totalWeight;
    QGroupBox* moveBox;
    QLabel* mechMV;
    QLabel* totalMV;
    QLabel* mechMP;
    QLineEdit* mechaName;
    QComboBox* formChoice;

    AmmoEntry* ammoBox;
    ControlEntry* controlBox;
    PropEntry* propBox;
    OptionEntry* optionBox;
    ShieldEntry* shieldBox;

    QGroupBox* spaceBox;

    QMenu* fileMenu;
    QMenu* helpMenu;

    QAction* saveAction;
    QAction* openAction;
    QAction* printAction;
    QAction* exportPDFAction;
    QAction* exportHTMLAction;
    QAction* aboutAction;

    void addVitalStatWidgets(QVBoxLayout* layout);
    void updateStats();
    void fillSpaceBox();
    void createMenus();
    void createActions();

public slots:
    void updateCall();
    void weaponUpdate();
    void saveMecha();
    void loadMecha();
    void printMecha();
    void exportPDF();
    void exportHTML();
    void aboutWin();
    void tfUpdate();

signals:
    void refreshAll(Mecha*);
};

#endif // MAINWINDOW_H
