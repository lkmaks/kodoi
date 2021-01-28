#ifndef ROOMLABEL_H
#define ROOMLABEL_H

#include <QLabel>
#include <QPushButton>

class RoomLabel : public QWidget
{
    Q_OBJECT
public:
    explicit RoomLabel(QString room_id, QWidget *parent = nullptr);
private:
    QLabel *room_id_;
    QPushButton *enter_button_;
};

#endif // ROOMLABEL_H
