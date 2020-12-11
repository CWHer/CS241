#ifndef UTILS_H
#define UTILS_H

#include "common.h"

inline void read_daily(QString &line, Tour &cur) {
    // 8 parameters each line
    const auto LINE_NUM = 8;
    QStringList data = line.split(',');
    if (data.size() != LINE_NUM) {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("error line");
        return;
    }
    // removed on 11.19 remark: redundancy and time consuming
    //    ret.id = std::move(data[0]);
    cur.start = data[1].toInt(), cur.end = data[2].toInt();
    cur.src = make_pair(data[3].toDouble(), data[4].toDouble());
    cur.dst = make_pair(data[5].toDouble(), data[6].toDouble());
    cur.fee = data[7].toDouble();
}

inline void read_grid(QString &line, Grid &cur) {
    // 9 parameters each line
    const auto LINE_NUM = 9;
    QStringList data = line.split(',');
    if (data.size() != LINE_NUM) {
        QErrorMessage *error = new QErrorMessage();
        error->showMessage("error line");
        return;
    }
    // *************************
    // not debuged yet
    cur.id = data[0].toInt();
    cur.y1 = make_pair(data[1].toDouble(), data[2].toDouble());
    cur.y2 = make_pair(data[3].toDouble(), data[4].toDouble());
    cur.x1 = make_pair(data[5].toDouble(), data[6].toDouble());
    cur.x2 = make_pair(data[7].toDouble(), data[8].toDouble());
}

#endif // UTILS_H
