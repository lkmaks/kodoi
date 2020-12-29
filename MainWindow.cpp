#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QDebug>

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    settings_ = new Settings();
    main_widget_ = new MainWidget(settings_, this);

    menu_ = menuBar()->addMenu("Settings");
    QAction *engine_setup = new QAction("Engine", this);
    menu_->addAction(engine_setup);
    connect(engine_setup, &QAction::triggered, this, &MainWindow::EngineSetup);

    connect(this, &MainWindow::SettingsUpdated, main_widget_, &MainWidget::AppSettingsUpdated);

    setCentralWidget(main_widget_);
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
