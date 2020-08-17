#include "EngineWrapper.h"
#include <QDebug>
#include <stdio.h>

EngineWrapper::EngineWrapper(QObject *parent) : QObject(parent), brain_proc_(this) {
    QObject::connect(&brain_proc_, &QProcess::started,
                     this, &EngineWrapper::ProcessStarted);
    QObject::connect(&brain_proc_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, &EngineWrapper::ProcessFinished);
    QObject::connect(&brain_proc_, &QProcess::readyReadStandardOutput,
                     this, &EngineWrapper::ReadyStandardOutput);
}


/// process-manager methods:

void EngineWrapper::Start() {
    brain_proc_.start("/usr/bin/wine", {"/home/max/qt_projects/kodoi/engine.exe"});
}

void EngineWrapper::Stop() {
    // todo: reinitialization
    brain_proc_.write("END\n");
}

// TODO: ForceStop


/// async thinking/setup interface methods:

void EngineWrapper::Setup(EngineWrapper::EngineSettings new_settings) {
    settings_ = new_settings;
    std::string tmpltee = "INFO show_detail 1\n"
                      "INFO max_node %\n"
                      "INFO timeout_match %\n"
                      "INFO time_left %\n"
                      "INFO timeout_turn %\n"
                      "INFO max_depth %\n"
                      "INFO time_increment %\n"
                      "INFO caution_factor %\n"
                      "INFO thread_num %\n"
                      "INFO hash_size %\n"
                      "INFO rule %\n";
    char buf[50 * 12]; // line_length * lines_cnt
    int len = sprintf(buf,
            tmpltee.c_str(),
            settings_.max_node,
            settings_.timeout_match,
            settings_.time_left,
            settings_.timeout_turn,
            settings_.max_depth,
            settings_.time_increment,
            settings_.caution_factor,
            settings_.thread_num,
            settings_.hash_size,
            settings_.rule);
    qDebug() << "settings payload: \n";
    qDebug() << buf;
    brain_proc_.write(buf, len);
}

void EngineWrapper::StartThinking(const Position &position, int nbest_num) {
    nbest_num_ = nbest_num;
    position_ = position;
    if (wrapper_epoch_id_ == 0) {
        brain_proc_.write("START");
    }
    else {
        brain_proc_.write("RESTART");
    }
    ++wrapper_epoch_id_;
    QString dims = " " + QString::number(position_.board_height) +
                 + " " + QString::number(position_.board_width) + "\n";
    brain_proc_.write(dims.toUtf8());
    brain_proc_.write("yxboard\n");
    brain_proc_.write(EngineWrapper::PositionToText(position_).toUtf8());
}

void EngineWrapper::StopThinking() {
    brain_proc_.write("yxstop\n");
}


/// slots (to handle signals from process):

void EngineWrapper::ProcessStarted() {
    emit EngineStarted();
}

void EngineWrapper::ProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);
    emit EngineStopped();
}

void EngineWrapper::ReadyStandardOutput() {
    int prev_ptr = 0;
    int ptr = cur_read_string_.size();
    cur_read_string_ += QString(brain_proc_.readAllStandardOutput());
    while (ptr < cur_read_string_.size()) {
        while (ptr < cur_read_string_.size() && cur_read_string_[ptr] != '\n') {
            ++ptr;
        }
        ProcessStdLine(cur_read_string_.midRef(prev_ptr, ptr - prev_ptr - 1));
    }
}


/// main function: process engine output

void EngineWrapper::ProcessStdLine(QStringRef line) {
    if (line.contains("OK")) {
        // new epoch for brain
        // clear residual update data, since we don't know
        // if brain writes down the whole update before sending this
        cur_nbest_update_ = {};
        ++brain_epoch_id_;
        return;
    }

    if (brain_epoch_id_ < wrapper_epoch_id_) {
        // this line is from older epoch then we need, ignore it
        // we will be ignoring from now on until OK's won't make epoch_id's equal
        return;
    }

    if (line.startsWith("MESSAGE DETAIL DEPTH:") ||
        line.startsWith("MESSAGE DETAIL (NBEST) DEPTH:")) {
        // "MESSAGE DETAIL DEPTH:" - 21 chars
        // "MESSAGE DETAIL (NBEST) DEPTH:" - 29 chars
        int pref_len;
        if (line.startsWith("MESSAGE DETAIL DEPTH:")) {
            pref_len = 21;
        }
        else {
            pref_len = 29;
        }

        int dash_pos = line.indexOf('-', pref_len);
        int space_pos = line.indexOf(' ', dash_pos);
        cur_nbest_update_.depth_range.first = line.mid(pref_len, dash_pos - pref_len).toInt();
        cur_nbest_update_.depth_range.second = line.mid(dash_pos + 1, space_pos - dash_pos - 1).toInt();

        int val_pos = line.indexOf("VAL:") + 4;
        space_pos = line.indexOf(' ', val_pos);
        cur_nbest_update_.value = line.mid(val_pos, space_pos - val_pos).toInt();
    }
    else if (line.startsWith("MESSAGE (")) {
        int val_pos = line.indexOf("VAL:") + 4;
        int space_pos = line.indexOf(' ', val_pos);
        cur_nbest_update_.value = line.mid(val_pos, space_pos - val_pos).toInt();

        cur_nbest_update_.play_lines.push_back({});
        int from_pos = line.indexOf('[');
        int to_pos = line.indexOf(']');
        while (from_pos != -1) {
            QChar ch = line[from_pos + 1];
            int num = line.mid(from_pos + 3, to_pos - (from_pos + 3)).toInt();
            cur_nbest_update_.play_lines.back().line.push_back({ch, num});
            from_pos = line.indexOf('[', to_pos + 1);
            to_pos = line.indexOf(']', to_pos + 1);
        }

        auto check_num_slice = line.mid(9); // from the number in brackets
        if (check_num_slice.startsWith(QString::number(nbest_num_))) {
            emit NbestUpdate(cur_nbest_update_);
            cur_nbest_update_ = {};
        }
    }
}


///  ==================== utility ========================


QString EngineWrapper::PositionToText(const Position &pos) {
    QString result;
    // TODO: add a check on whether the engine can use the given position notation
    int side = 1 + (pos.seq.size() % 2);
    for (int i = 0; i < pos.seq.size(); ++i) {
        auto loc = pos.seq[i];
        side = 2 - side;
        result += QString::number(loc.first + 1);
        result += ",";
        result += QString::number(loc.second + 1);
        result += ",";
        result += QString::number(side);
        result += "\n";
    }
    return result;
}


bool EngineWrapper::TermKill(int msecs) {
    brain_proc_.terminate();
    if (brain_proc_.waitForFinished(msecs)) {
        return true;
    }
    else {
        brain_proc_.kill();
        brain_proc_.waitForFinished();
        return false;
    }
}
