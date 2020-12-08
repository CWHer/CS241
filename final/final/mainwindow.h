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
    static const int WIDTH = 800;
    static const int HEIGHT = 600;
    // a welcome widget
    //  also load data
    WelcomeWidget *welcome;
    DataBase *db;
    Ui::MainWindow *ui;
    // elements in mainwindow gui
    QWidget *centralwidget;
    // main layout for plot
    //  can be intialized by setxxxPlot
    QVBoxLayout *mainlayout;

  public:
    void setupLayouts();
    void setupConnects();
    void switchwindow();
    // data vs space
    void setSpaticalPlot();
    // data vs time
    void setTimePlot();
    // some important infomation
    void setInfoPlot();
};
#endif // MAINWINDOW_H
