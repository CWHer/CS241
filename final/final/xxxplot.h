#ifndef XXXPLOT_H
#define XXXPLOT_H

#include "database.h"
#include <QObject>

class xxxPlot : public QObject {
    Q_OBJECT
  public:
    DataBase *db;
    virtual void setupLayouts() = 0;
    virtual void setupConnects() {}
    virtual void plotMap() = 0;

  signals:
    void plotDone(xxxPlot *);
};

#endif // XXXPLOT_H
