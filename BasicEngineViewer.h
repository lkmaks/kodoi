#ifndef BASICENGINEVIEWER_H
#define BASICENGINEVIEWER_H

#include <QWidget>
#include <QLabel>
#include "EngineWrapper.h"

class BasicEngineViewer : public QWidget
{
    Q_OBJECT
public:
    explicit BasicEngineViewer(QWidget *parent = nullptr);

    void PonderingStarted();
    void PonderingStopped();

    void NbestUpdated(const EngineWrapper::NbestUpdate &upd);
    void EvalChanged(int val);
private:
    QLabel *label_;
};

#endif // BASICENGINEVIEWER_H
