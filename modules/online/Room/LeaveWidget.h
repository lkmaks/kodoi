#ifndef LEAVEWIDGET_H
#define LEAVEWIDGET_H

#include <QWidget>
#include <QPushButton>

class LeaveWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeaveWidget(QWidget *parent = nullptr);
signals:
    void Leave();
private:
    QPushButton *button_;
};

#endif // LEAVEWIDGET_H
