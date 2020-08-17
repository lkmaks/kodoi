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

    void StartThinking(const Position &position, int nbest_num);
    void StopThinking();


    // utility methods
    static QString PositionToText(const Position &pos_);
    bool TermKill(int msecs = 1000);
    QPair<int, int> HumanCoordsToNormalCoords(QPair<char, int> pos, int board_height);


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
            QVector<QPair<QChar, int> > line;
        };

        int epoch_id; // which start_thinking is this update for
        int value;
        QPair<int, int> depth_range;
        QVector<PlayLine> play_lines;
    };

signals:
    void EngineStarted();
    void EngineStopped();
    void NbestUpdated(const NbestUpdate &update);

private:
    QProcess brain_proc_;

    // set up by user
    EngineSettings settings_;
    Position position_;

    // internal state
    int brain_epoch_id_; // have to know which StartThinking() brain sends update for
    int wrapper_epoch_id_; // have to know if epoch of the brain is still on
                           // in order to store only O(1) information to decide when
                           // to collect nbest update (as we will know how many lines it should contain)
                           // so, if brain epoch is yet older, we will simply discard any updates;
                           // otherwise, we know how to treat them
    int nbest_num_; // the actual parameter of nbest of current wrapper epoch
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
