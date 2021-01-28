#include "RoomLabel.h"
#include <QVBoxLayout>

RoomLabel::RoomLabel(QString room_id, QWidget *parent) : QWidget(parent)
{
    room_id_ = new QLabel(room_id, this);
    enter_button_ = new QPushButton("Enter", this);

    auto *vlt = new QVBoxLayout(this);
    vlt->addWidget(room_id_);
    vlt->addWidget(enter_button_);
}
