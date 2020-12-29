#include "InfoWidget.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QObject>

InfoWidget::InfoWidget(Config *config, QWidget *parent) : QWidget(parent), config_(config)
{
    auto lt = new QVBoxLayout(this);

    engine_state_ = new QLabel();
    lt->addWidget(engine_state_);

    auto nbest_n_section = new QGroupBox(this);
    nbest_n_section->setFixedSize(100, 100);

    auto nbest_n_mod_section = new QGroupBox(nbest_n_section);
    nbest_n_mod_section->setFixedSize(50, 100);
    nbest_n_mod_section->move(50, 0);

    nbest_n_label_ = new QLabel(nbest_n_section);
    nbest_n_label_->setFixedSize(50, 100);
    nbest_n_label_->move(0, 0);
    nbest_n_label_->setAlignment(Qt::AlignCenter);

    nbest_n_plus_ = new QPushButton("+", nbest_n_mod_section);
    nbest_n_plus_->setFixedSize(50, 50);
    nbest_n_plus_->move(0, 0);

    nbest_n_minus_ = new QPushButton("-", nbest_n_mod_section);
    nbest_n_minus_->setFixedSize(50, 50);
    nbest_n_minus_->move(0, 50);

    lt->addWidget(nbest_n_section);

    nbest_n_ = config_->default_nbest_value;
    nbest_n_label_->setNum(nbest_n_);

    nbest_n_min_ = 1;
    nbest_n_max_ = 20;

    connect(nbest_n_plus_, &QPushButton::pressed,
            this, &InfoWidget::NbestNPlusPressed);
    connect(nbest_n_minus_, &QPushButton::pressed,
            this, &InfoWidget::NbestNMinusPressed);
}


void InfoWidget::UpdateNbestNValue(int val) {
    nbest_n_label_->setNum(val);
    emit NbestValueChanged(val);
}

void InfoWidget::NbestNPlusPressed() {
    if (nbest_n_ < nbest_n_max_) {
        ++nbest_n_;
        UpdateNbestNValue(nbest_n_);
    }
}

void InfoWidget::NbestNMinusPressed() {
    if (nbest_n_ > nbest_n_min_) {
        --nbest_n_;
        UpdateNbestNValue(nbest_n_);
    }
}


void InfoWidget::SetEngineStateText(const QString &text) {
    engine_state_->setText(text);
}
