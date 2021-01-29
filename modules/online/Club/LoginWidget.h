#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "../OnlineSession.h"

class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(OnlineSession *session, QWidget *parent = nullptr);

private:
    OnlineSession *session_;

    QLineEdit *name_;
    QLineEdit *password_;
    QPushButton *login_;
    QPushButton *as_guest_;
};

#endif // LOGINWIDGET_H
