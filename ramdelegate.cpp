#include "ramdelegate.h"
#include "servocomp.h"
#include <QComboBox>

RAMDelegate::RAMDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* RAMDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItems(ServoComp::instance()->getRAMTypes());

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void RAMDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

   comboBox->clear();
   comboBox->addItems(ServoComp::instance()->getRAMTypes());

   comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());

}

void RAMDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    int selectedItem = comboBox->findText(comboBox->currentText());

    model->setData(index, selectedItem, Qt::EditRole);

}

void RAMDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
