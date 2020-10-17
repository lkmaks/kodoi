#ifndef ENGINEVIEWER_H
#define ENGINEVIEWER_H

#include "BoardTools.h"
#include <QObject>

class EngineViewer : public QObject
{
    Q_OBJECT

public:
    EngineViewer(const BoardExplorerTools &tools);

    void PonderingStarted();
    void PonderingStopped();
    void EngineErrorOccured();
    void NbestUpdated(const EngineWrapper::NbestUpdate &upd);


    static qreal ValueToPortionLevel(int val, int val_max);
    static QColor ValueToEvalCircleColor(int val, int val_max);

private:
    BoardExplorerTools tools_;
};

#endif // ENGINEVIEWER_H
