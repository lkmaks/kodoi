#ifndef ENGINEVIEWER_H
#define ENGINEVIEWER_H

#include "BoardTools.h"
#include <QObject>

class EngineViewer : public QObject
{
    Q_OBJECT

public:
    EngineViewer(const BoardTools &tools);

    void NbestUpdated(const EngineWrapper::NbestUpdate &upd);

    qreal ValueToPortionLevel(int val, int val_max);

private:
    BoardTools tools_;
};

#endif // ENGINEVIEWER_H
