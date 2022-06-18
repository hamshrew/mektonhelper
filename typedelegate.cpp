#include "typedelegate.h"
#include "servocomp.h"
#include <QComboBox>

TypeDelegate::TypeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* TypeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItems(ServoComp::instance()->getTypes());

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void TypeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

   comboBox->clear();
   comboBox->addItems(ServoComp::instance()->getTypes());

   comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());

}

void TypeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    int selectedItem = comboBox->findText(comboBox->currentText());

    model->setData(index, selectedItem, Qt::EditRole);

}

void TypeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
