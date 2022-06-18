#include "matedelegate.h"
#include "mecha.h"
#include "matedsystem.h"

#include <QComboBox>

MateDelegate::MateDelegate(Mecha* mecha, QObject *parent) :
    QStyledItemDelegate(parent),
    mecha(mecha)
{
}

QWidget* MateDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItem("None");

    QList<SplittablePart*> msList = mecha->getMateableSystems();

    foreach (SplittablePart* sp,msList)
    {
        comboBox->addItem(sp->getName());
    }

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void MateDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

    comboBox->clear();
    comboBox->addItem("None");

    QList<SplittablePart*> msList = mecha->getMateableSystems();

    foreach (SplittablePart* sp,msList)
    {
        comboBox->addItem(sp->getName());
    }

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);
}

void MateDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    model->setData(index, comboBox->currentIndex(), Qt::EditRole);

}

void MateDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
