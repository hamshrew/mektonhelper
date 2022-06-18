#include "spinboxdelegate.h"
#include <QSpinBox>

SpinBoxDelegate::SpinBoxDelegate(int min, int max, QObject *parent) :
    QStyledItemDelegate(parent),
    min(min),
    max(max)
{
}

QWidget* SpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QSpinBox *spinBox = new QSpinBox(parent);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);

    return spinBox;

}

void SpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QSpinBox *spinBox = qobject_cast<QSpinBox*>(editor);

   spinBox->clear();

   spinBox->setValue(index.model()->data(index, Qt::DisplayRole).toInt());

}

void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QSpinBox* spinBox = qobject_cast<QSpinBox*>(editor);

    spinBox->interpretText();
    int value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
