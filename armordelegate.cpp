#include "armordelegate.h"
#include "servocomp.h"
#include <QComboBox>

ArmorDelegate::ArmorDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget* ArmorDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItems(ServoComp::instance()->getArmorTypes());

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void ArmorDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

   comboBox->clear();
   comboBox->addItems(ServoComp::instance()->getArmorTypes());

   comboBox->setEditText(index.model()->data(index, Qt::DisplayRole).toString());

}

void ArmorDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    int selectedItem = comboBox->findText(comboBox->currentText());

    model->setData(index, selectedItem, Qt::EditRole);

}

void ArmorDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
