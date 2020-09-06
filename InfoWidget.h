#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QLabel>

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);

    void SetEngineStateText(const QString &text);
private:
    QLabel *engine_state_;

};

#endif // INFOWIDGET_H
