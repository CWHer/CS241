#ifndef TIMEPLOT_H
#define TIMEPLOT_H

#include "common.h"
#include "gridselector.h"
#include "mythread.hpp"
#include "xxxplot.h"
#include <QWidget>

// data vs time
//  line charts and pie charts
class TimePlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit TimePlot(const int, const int, DataBase *, QThread *);
    QWidget *main_widget;

    void setupLayouts() override;
    void setupConnects() override;
    // plot map
    //  using below plotxxxMap with another thread
    void plotMap() override;

  private:
    const int WIDTH, HEIGHT;

    // plot settings
  private:
    int day, start_hour, end_hour, step_min;

  private:
    QChartView *plot_area;
    QPushButton *place_button;
    GridSelector *selector;
    // store grids chosen
    vector<int> grid_id;
    QComboBox *date_combo;
    QComboBox *start_combo;
    QComboBox *end_combo;
    QComboBox *step_combo;
    QComboBox *type_combo;
    QProgressBar *progress_bar;
    QPushButton *plot_button;
    //    QLineSeries *line;
    //    QChart *chart;
    void plotSeriesMap();
    void plotPieMap();
    // time + num
    void calcSeries(vector<pair<int, int>> &);
};

#endif // TIMEPLOT_H
