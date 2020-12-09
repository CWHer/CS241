#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "database.h"
#include "infoplot.h"
#include "spatialplot.h"
#include "timeplot.h"
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
    QTabWidget *centralwidget;
    // main widget
    //  can be intialized by xxxPlot
    SpatialPlot *spatialplot;
    TimePlot *timeplot;
    InfoPlot *infoplot;

  public:
    void setupLayouts();
    void setupConnects();
    void switchwindow();
};
#endif // MAINWINDOW_H
