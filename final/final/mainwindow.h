#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "database.h"
#include "utils.h"
#include "welcomewidget.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setWelcome();

  private:
    // a welcome widget
    //  also load data
    WelcomeWidget *welcome;
    DataBase *db;
    Ui::MainWindow *ui;
    void setupLayouts();
    void setupConnects();
    void switchwindow();
};
#endif // MAINWINDOW_H
