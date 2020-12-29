#ifndef ENGINEVIEWER_H
#define ENGINEVIEWER_H

#include "common/ColorBar.h"
#include "common/BoardPainter.h"
#include "common/InfoWidget.h"
#include <QObject>

class EngineViewer : public QObject
{
    Q_OBJECT

public:
    EngineViewer(ColorBar *, BoardPainter *, InfoWidget *, EngineEpochId *);

    void PonderingStarted();
    void PonderingStopped();
    void EngineErrorOccured();
    void NbestUpdated(const EngineWrapper::NbestUpdate &upd);


    static qreal ValueToPortionLevel(int val, int val_max);
    static QColor ValueToEvalCircleColor(int val, int val_max);

private:
    ColorBar *color_bar_;
    BoardPainter *painter_;
    InfoWidget *info_widget_;
    QVector<QGraphicsItem*> eval_circles_;
    EngineEpochId *epoch_id_;
};

#endif // ENGINEVIEWER_H
