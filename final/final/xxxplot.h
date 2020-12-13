#ifndef XXXPLOT_H
#define XXXPLOT_H

#include "database.h"
#include <QObject>

class xxxPlot : public QObject {
    Q_OBJECT
  public:
    xxxPlot(DataBase *_db, QThread *_thread)
        : db(_db)
        , main_thread(_thread) {}
    virtual void setupLayouts() = 0;
    virtual void setupConnects() {}
    virtual void plotMap() = 0;
    DataBase *db;
    QThread *main_thread;

  signals:
    void resetPlot(xxxPlot *);
};

#endif // XXXPLOT_H
