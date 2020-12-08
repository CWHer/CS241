#ifndef LOADTHREAD_H
#define LOADTHREAD_H

#include "common.h"
#include "database.h"
#include "utils.h"

class LoadThread : public QThread {
    Q_OBJECT
  private:
    QString folder_path;
    DataBase *db;

  public:
    // initialize loadthread with file_path and db
    void init(QString, DataBase *);
    void run() override;

  signals:
    void setValue(double value);
    void switchwindow();
};

#endif // LOADTHREAD_H
