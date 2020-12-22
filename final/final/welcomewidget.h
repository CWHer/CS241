#ifndef WIDGET_H
#define WIDGET_H

#include "common.h"
#include "database.h"
#include "mythread.hpp"
#include <QWidget>

class WelcomeWidget : public QWidget {
    Q_OBJECT
  public:
    void setupLayouts();
    void setupConnects();
    DataBase *db;

  private:
    static const auto WIDTH = 600;
    static const auto HEIGHT = 400;
    QPushButton *select_button, *load_button;
    QProgressBar *progress_bar;
    void getPath();
    void setBarValue(double);
    void loadData();
    void loadDone();

  signals:
    void switchWindow();
};

#endif // WIDGET_H
