#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include "Config.h"

class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(Config *config, QWidget *parent = nullptr);

    void UpdateNbestNValue(int val);

    // slots for inner widgets
    void NbestNPlusPressed();
    void NbestNMinusPressed();

    // interface slots
    void SetEngineStateText(const QString &text);

signals:
    void NbestValueChanged(int new_n);

private:
    Config *config_;

    int nbest_n_;
    int nbest_n_min_;
    int nbest_n_max_;
    QLabel *engine_state_;
    QLabel *nbest_n_label_;
    QPushButton *nbest_n_plus_;
    QPushButton *nbest_n_minus_;
};

#endif // INFOWIDGET_H
