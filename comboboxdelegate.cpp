#include "comboboxdelegate.h"
#include <QComboBox>

ComboBoxDelegate::ComboBoxDelegate(const QStringList& data, QObject *parent) :
    QStyledItemDelegate(parent),
    source(data)
{
}

QWidget* ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    QComboBox *comboBox = new QComboBox(parent);

    comboBox->addItems(source);

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);

    return comboBox;

}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox = qobject_cast<QComboBox*>(editor);

    comboBox->clear();
    comboBox->addItems(source);

    QString currentText = index.model()->data(index, Qt::DisplayRole).toString();

    int selectedItem = comboBox->findText(currentText);

    if (selectedItem <0)
        selectedItem = 0;

    comboBox->setCurrentIndex(selectedItem);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox*>(editor);

    model->setData(index, comboBox->currentText(), Qt::EditRole);

}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
