#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QDebug>

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    config_ = new Config();
    settings_ = new Settings();
    online_widget_ = new OnlineWidget(config_, settings_, this);

    auto menu_settings = menuBar()->addMenu("Settings");
    QAction *engine_setup = new QAction("Engine", this);
    menu_settings->addAction(engine_setup);
    connect(engine_setup, &QAction::triggered, this, &MainWindow::EngineSetup);

    auto menu_online = menuBar()->addMenu("Online");

    connect(this, &MainWindow::SettingsUpdated, online_widget_, &OnlineWidget::AppSettingsUpdated);

    setCentralWidget(online_widget_);
    setMinimumSize(1200, 800);
    setStyleSheet("background-color:#dddddd;");
}

void MainWindow::EngineSetup() {
    bool ok;
    QString text = QInputDialog::getText(this, tr("Engine"),
                                             tr("Engine script/path:"), QLineEdit::Normal,
                                             "", &ok);
    if (ok) {
        settings_->engine_cmd = text;
        emit SettingsUpdated(SettingsField::ENGINE_CMD);
    }
}


MainWindow::~MainWindow()
{

}
