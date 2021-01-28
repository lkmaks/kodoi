#include "CreateForm.h"
#include <QHBoxLayout>

CreateForm::CreateForm(QWidget *parent) : QWidget(parent)
{
    line_edit_ = new QLineEdit(this);
    submit_button_ = new QPushButton("Create", this);
    QHBoxLayout *lt = new QHBoxLayout(this);
    lt->addWidget(line_edit_);
    lt->addWidget(submit_button_);

    connect(submit_button_, &QPushButton::pressed, this, &CreateForm::ButtonPressed);
}

void CreateForm::ButtonPressed() {
    emit CreateRoom(line_edit_->text());
}
