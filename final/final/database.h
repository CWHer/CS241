#ifndef DATABASE_H
#define DATABASE_H

#include "common.h"
#include "utils.h"

class DataBase {
  public:
    // store data
  public:
    // day x tour_pre_day
    vector<tour> tours[DAY_NUM];

    // grid
    //*************************
    // perf: binary search
    vector<grid> grids;
    //
};

#endif // DATABASE_H
