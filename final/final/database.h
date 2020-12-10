#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"
#include "mythread.hpp"
#include "utils.h"

// store data
//  need func 'movetoThread' from baseclass
class DataBase : public QObject {
    Q_OBJECT
  public:
    QString folder_path;
    QFileInfoList file_list;
    void parseFolder();
    void loadData();

  public:
    // name like 201611xx
    //  date + abs_path
    vector<QString> file_info[DAY_NUM];

    // day x tour_pre_day
    vector<tour> tours[DAY_NUM];

    // grid
    //*************************
    // perf: binary search
    vector<grid> grids;

  private:
    int total, cnt;

  signals:
    void setValue(double value);
    void loadDone();
};

#endif // DATABASE_H
