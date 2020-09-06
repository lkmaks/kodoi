#include "InfoWidget.h"
#include <QVBoxLayout>

InfoWidget::InfoWidget(QWidget *parent) : QWidget(parent)
{
    auto lt = new QVBoxLayout(this);
    engine_state_ = new QLabel();
    lt->addWidget(engine_state_);
}

void InfoWidget::SetEngineStateText(const QString &text) {
    engine_state_->setText(text);
}
