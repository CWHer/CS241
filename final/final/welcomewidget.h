#ifndef WIDGET_H
#define WIDGET_H

#include "common.h"
#include "database.h"
#include "mythread.hpp"
#include <QWidget>

class WelcomeWidget : public QWidget {
    Q_OBJECT
  public:
    explicit WelcomeWidget(QWidget *parent = nullptr);
    void getPath();
    void setBarValue(double);
    QString folder_path;
    DataBase *db;

  private:
    static const auto WIDTH = 400;
    static const auto HEIGHT = 500;
    QPushButton *selectfolder;
    QProgressBar *loadbar;
    void setupLayouts();
    void setupConnects();
    void loadData();
    void loadDone();

  signals:
    void switchWindow();
};

#endif // WIDGET_H
