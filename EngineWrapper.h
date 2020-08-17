#ifndef ENGINEWRAPPER_H
#define ENGINEWRAPPER_H

#include <QProcess>
#include <QVector>
#include <QPair>
#include "Enums.h"

class EngineWrapper : public QObject {
    Q_OBJECT
public:
    struct EngineSettings;
    struct Position;
    struct NbestUpdate;

    explicit EngineWrapper(QObject *parent = nullptr);

    // process-related
    void Start();
    void Stop();

    void Setup(EngineWrapper::EngineSettings new_settings);

    void SetPosition(Position &position);

    void StartThinking();
    void StopThinking();


    // utility methods
    static QString PositionToText(const Position &pos_);
    bool TermKill(int msecs = 1000);
    QPair<int, int> YixinOutputCoordsToInternalNormalCoords(QPair<char, int> pos);


    struct EngineSettings {
        Rule rule;
        int thread_num;
        int caution_factor;
        int max_node;
        int max_depth;
        int hash_size;

        // timeouts are not useful for pondering
        // so will have to set them so that they dont interfere
        // still can be set normal values for playing vs engine
        int timeout_match; // big
        int time_left; // big
        int timeout_turn; // big
        int time_increment; // 0
    };

    struct Position {
        int board_width, board_height;
        QVector<QPair<int, int> > seq;
    };

    struct NbestUpdate {
        struct PlayLine {
            int value;
            QVector<QPair<int, int> > line;
        };

        int value;
        QPair<int, int> depth_range;
        QVector<PlayLine> play_lines;
    };

signals:
    void EngineStarted();
    void EngineStopped();
    void SetupDone();
    void ThinkingStarted();
    void ThinkingStopped();
    void EvaluationUpdate(const QString udpate);

private:
    QProcess brain_proc_;

    // set up by user
    EngineSettings settings_;
    Position position_;

    // internal state
    bool is_noob_;
    QString cur_read_string_;
    NbestUpdate cur_nbest_update_;

    // parser for engine's output
    void ProcessStdLine(QStringRef line);

    // slots
    void ProcessStarted();
    void ProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void ReadyStandardOutput();

};

#endif // ENGINEWRAPPER_H
