#include <QVBoxLayout>

#include "LoginWidget.h"

#include "MainWindow.h"


LoginWidget::LoginWidget(OnlineSession *session, QWidget *parent) : QWidget(parent), session_(session)
{
    setGeometry(100, 100, 500, 500);
    name_ = new QLineEdit("", this);
    name_->setPlaceholderText("logina");
    password_ = new QLineEdit(this);
    password_->setPlaceholderText("passworda");
    login_ = new QPushButton("Login", this);
    as_guest_= new QPushButton("I just wanna play", this);

    setLayout(new QVBoxLayout());
    layout()->addWidget(name_);
    layout()->addWidget(password_);
    layout()->addWidget(login_);
    layout()->addWidget(as_guest_);

    connect(login_, &QPushButton::pressed, this, [this](){
        session_->Login(login_->text(), password_->text());
    });

    connect(as_guest_, &QPushButton::pressed, this, [this](){
        session_->LoginAsGuest();
    });

    connect(session_, &OnlineSession::ReceivedStatus, this, [this](QString status){
        if (status == "ok") {
            static_cast<MainWindow*>(this->parent())->LoginSuccessful(name_->text(), password_->text());
        }
        else {
            // show that login failed
        }
    });

    connect(session_, &OnlineSession::ReceivedGuestName, this, [this](QString name){
        static_cast<MainWindow*>(this->parent())->LoginSuccessful(name, "", true);
    });
}

