#include "LeaveWidget.h"

LeaveWidget::LeaveWidget(QWidget *parent) : QWidget(parent)
{
    button_ = new QPushButton("<-   ", this);
    connect(button_, &QPushButton::pressed, this, [this](){emit Leave();});
}

