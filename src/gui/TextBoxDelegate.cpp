#include <QtGui>
#include <QSpinBox>
#include <QLineEdit>
#include "TextBoxDelegate.h"

TextBoxDelegate::TextBoxDelegate(QValidator *validator,QObject *parent)
     : QItemDelegate(parent)
 {
	 this->validator = validator;
 }

 QWidget *TextBoxDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &/* option */,
     const QModelIndex &/* index */) const
 {
	 QLineEdit *editor = new QLineEdit(parent);
	 editor->setValidator(validator);

     return editor;
 }

 void TextBoxDelegate::setEditorData(QWidget *editor,
                                     const QModelIndex &index) const
 {
     double value = index.model()->data(index, Qt::EditRole).toDouble();

     QLineEdit *edit = static_cast<QLineEdit*>(editor);
	 edit->setText(tr("%1").arg(value));
 }

 void TextBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                    const QModelIndex &index) const
 {
     QLineEdit *edit = static_cast<QLineEdit*>(editor);
	 double value = edit->text().toDouble();

     model->setData(index, value, Qt::EditRole);
 }

 void TextBoxDelegate::updateEditorGeometry(QWidget *editor,
     const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
 {
     editor->setGeometry(option.rect);
 }