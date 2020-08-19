#include "EngineWrapper.h"
#include <QDebug>
#include <QThread>
#include <stdio.h>

EngineWrapper::EngineWrapper(QObject *parent) : QObject(parent), brain_proc_(this) {
    QObject::connect(&brain_proc_, &QProcess::started,
                     this, &EngineWrapper::ProcessStarted);
    QObject::connect(&brain_proc_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                     this, &EngineWrapper::ProcessFinished);
    QObject::connect(&brain_proc_, &QProcess::readyReadStandardOutput,
                     this, &EngineWrapper::ReadyStandardOutput);
    wrapper_epoch_id_ = brain_epoch_id_ = 0;
}


/// process-manager methods:

void EngineWrapper::Start() {
    brain_proc_.start("/usr/bin/wine", {"/home/max/qt_projects/kodoi/engine.exe"});
}

void EngineWrapper::Stop() {
    brain_epoch_id_ = wrapper_epoch_id_ = 0;
    cur_read_string_.clear();
    cur_nbest_update_ = {};
    brain_proc_.write("END\n");
}

// TODO: ForceStop


/// async thinking/setup interface methods:

void EngineWrapper::Setup(EngineWrapper::EngineSettings new_settings) {
    settings_ = new_settings;
    std::string tmpltee = "INFO show_detail 1\n"
                      "INFO max_node %d\n"
                      "INFO timeout_match %d\n"
                      "INFO time_left %d\n"
                      "INFO timeout_turn %d\n"
                      "INFO max_depth %d\n"
                      "INFO time_increment %d\n"
                      "INFO caution_factor %d\n"
                      "INFO thread_num %d\n"
                      "INFO hash_size %d\n"
                      "INFO rule %d\n";
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
    brain_proc_.write(dims.toLocal8Bit());

    brain_proc_.write("yxboard\r\n");
    qDebug() << "pld:\n" << EngineWrapper::PositionToText(position_) << endl;
    brain_proc_.write(EngineWrapper::PositionToText(position_).toUtf8());
    brain_proc_.write("done\r\n");
    brain_proc_.write("yxnbest3\r\n");
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

#include <iostream>

void EngineWrapper::ReadyStandardOutput() {
    //qDebug() << "readystd\n";
    int prev_ptr = 0;
    int ptr = cur_read_string_.size();
    cur_read_string_ += QString(brain_proc_.readAllStandardOutput());
    //std::cerr << cur_read_string_.toStdString() << std::endl;
    while (ptr < cur_read_string_.size()) {
        while (ptr < cur_read_string_.size() && cur_read_string_[ptr] != '\n') {
            ++ptr;
        }
        if (ptr < cur_read_string_.size()) {
            // found a line;
            ProcessStdLine(cur_read_string_.midRef(prev_ptr, ptr - prev_ptr - 1));
            ++ptr;
            prev_ptr = ptr;
        }
    }
    cur_read_string_.remove(0, prev_ptr);
}


/// main function: process engine output

void EngineWrapper::ProcessStdLine(QStringRef line) {
    //qDebug() << line << endl;
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
            emit NbestUpdated(cur_nbest_update_);
            cur_nbest_update_ = {};
        }
    }
}


///  ==================== utility ========================


QString EngineWrapper::PositionToText(const Position &pos) {
    QString result;
    // TODO: add a check on whether the engine can use the given position notation
    int side = 1 + (pos.sequence.size() % 2);
    for (int i = 0; i < pos.sequence.size(); ++i) {
        auto loc = pos.sequence[i];
        side = 3 - side;
        result += QString::number(loc.first + 1);
        result += ",";
        result += QString::number(loc.second + 1);
        result += ",";
        result += QString::number(side);
        result += "\r\n";
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
