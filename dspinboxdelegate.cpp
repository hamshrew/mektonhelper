#include "dspinboxdelegate.h"
#include <QDoubleSpinBox>
#include <cmath>

DSpinBoxDelegate::DSpinBoxDelegate(double min, double max, int prec, QObject *parent) :
    QStyledItemDelegate(parent),
    min(min),
    max(max),
    prec(prec)
{
}

QWidget* DSpinBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QDoubleSpinBox *spinBox = new QDoubleSpinBox(parent);
    spinBox->setMinimum(min);
    spinBox->setMaximum(max);
    spinBox->setDecimals(prec);
    spinBox->setSingleStep((prec ? 1/pow(10,prec):1));

    return spinBox;

}

void DSpinBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
   QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox*>(editor);

   spinBox->clear();

   spinBox->setValue(index.model()->data(index, Qt::DisplayRole).toDouble());

}

void DSpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(editor);

    spinBox->interpretText();
    double value = spinBox->value();

    model->setData(index, value, Qt::EditRole);
}

void DSpinBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
