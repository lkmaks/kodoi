#ifndef CREATEFORM_H
#define CREATEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "../types.h"

class CreateForm : public QWidget
{
    Q_OBJECT
public:
    explicit CreateForm(QWidget *parent = nullptr);

signals:
    void CreateRoom(RoomId room_id);

private:
    QLineEdit *line_edit_;
    QPushButton *submit_button_;

    void ButtonPressed();
};

#endif // CREATEFORM_H
