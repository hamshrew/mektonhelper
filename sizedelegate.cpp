#include "sizedelegate.h"
#include "servocomp.h"
#include <QComboBox>

SizeDelegate::SizeDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* SizeDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItems(ServoComp::instance()->getSizes());

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void SizeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

   comboBox->clear();
   comboBox->addItems(ServoComp::instance()->getSizes());

   comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());

}

void SizeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    int selectedItem = comboBox->findText(comboBox->currentText());

    model->setData(index, selectedItem, Qt::EditRole);

}

void SizeDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
