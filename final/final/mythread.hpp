#ifndef MYTHREAD_H
#define MYTHREAD_H

#include "common.h"
#include "utils.h"

class myThread : public QThread {
    Q_OBJECT

  public:
    void run() override { emit funcStart(); }

  signals:
    void funcStart();
};

#endif // MYTHREAD_H
