#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "common.h"
#include "database.h"
#include "infoplot.h"
#include "spatialplot.h"
#include "timeplot.h"
#include "utils.h"
#include "welcomewidget.h"
#include "xxxplot.h"
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

    void setWelcome();

  private:
    static const int WIDTH = 1200;
    static const int HEIGHT = 900;
    // a welcome widget
    //  also load data
    WelcomeWidget *welcome;
    DataBase *db;
    QTabWidget *centralwidget;
    // main widget
    //  can be intialized by xxxPlot
    SpatialPlot *spatialplot;
    TimePlot *timeplot;
    InfoPlot *infoplot;

  public:
    void setupLayouts();
    void setupConnects();
    void switchWindow();
    void resetPlot(xxxPlot *);
};
#endif // MAINWINDOW_H
