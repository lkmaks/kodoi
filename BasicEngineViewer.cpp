#include "BasicEngineViewer.h"

BasicEngineViewer::BasicEngineViewer(QWidget *parent) : QWidget(parent)
{
    label_ = new QLabel(this);
    label_->setText("pondering off");
    setMinimumWidth(200);
    setMinimumHeight(200);
    //label_->show();
}

void BasicEngineViewer::PonderingStarted() {
    label_->setText("pondering on");
}

void BasicEngineViewer::PonderingStopped() {
    label_->setText("pondering off");
}

void BasicEngineViewer::NbestUpdated(const EngineWrapper::NbestUpdate &upd) {
    EvalChanged(upd.value);
}

void BasicEngineViewer::EvalChanged(int val) {
    label_->setNum(val);
}
