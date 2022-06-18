#include "loclistdelegate.h"
#include "mecha.h"
#include "servo.h"
#include <QComboBox>

LocListDelegate::LocListDelegate(Mecha* mecha, bool useHands, bool useMounts, bool useBinders, bool useSplit, QObject *parent) :
    QStyledItemDelegate(parent),
    mecha(mecha),
    useHands(useHands),
    useMounts(useMounts),
    useBinders(useBinders),
    useSplit(useSplit)
{
}

QWidget* LocListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItem("None");
    if (useHands)
    {
        int hands = mecha->getNumHands();
        for (int i = 1;i <= hands;++i)
        {
            comboBox->addItem(QString::number(i) + "H");
        }
    }

    QList<Servo*>& servos = mecha->getServoRef();

    Servo* ser;

    foreach (ser,servos)
    {
        comboBox->addItem(ser->getDesignation());
        if (!useMounts)
            continue;
        for (int i = 1;i <= ser->getMounts();++i)
        {
            QString str(ser->getDesignation() + " Mount");
            if (ser->getType() == TORSO)
                str += " " + QString::number(i);

            comboBox->addItem(str);
        }
    }

    if (useBinders)
    {
        QStringList shList = mecha->getShieldNames();
        QString sh;

        foreach (sh,shList)
        {
            if (mecha->getBinderSpace(sh + " Binder"))
                comboBox->addItem(sh + " Binder");
        }
    }

    if (useSplit)
        comboBox->addItem("Split");

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void LocListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

    comboBox->clear();
    comboBox->addItem("None");
    if (useHands)
    {
        int hands = mecha->getNumHands();
        for (int i = 1;i <= hands;++i)
        {
            comboBox->addItem(QString::number(i) + "H");
        }
    }
    QList<Servo*>& servos = mecha->getServoRef();

    Servo* ser;

    foreach (ser,servos)
    {
        comboBox->addItem(ser->getDesignation());
        if (!useMounts)
            continue;
        for (int i = 1;i <= ser->getMounts();++i)
        {
            QString str(ser->getDesignation() + " Mount");
            if (ser->getType() == TORSO)
                str += " " + QString::number(i);

            comboBox->addItem(str);
        }
    }


    if (useBinders)
    {
        QStringList shList = mecha->getShieldNames();
        QString sh;

        foreach (sh,shList)
        {
            if (mecha->getBinderSpace(sh + " Binder"))
                comboBox->addItem(sh + " Binder");
        }
    }

    if (useSplit)
        comboBox->addItem("Split");

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);
}

void LocListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    model->setData(index, comboBox->currentText(), Qt::EditRole);

}

void LocListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
