#ifndef TIMEPLOT_H
#define TIMEPLOT_H

#include "common.h"
#include "mythread.hpp"
#include "xxxplot.h"
#include <QWidget>

// data vs time
//  line charts and pie charts
class TimePlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit TimePlot(const int, const int);
    QWidget *main_widget;
    QChartView *plot_area;
    QComboBox *place_combo;
    // store grids chosen
    vector<int> grid_id;
    QComboBox *date_combo;
    QComboBox *start_combo;
    QComboBox *end_combo;
    QComboBox *step_combo;
    QComboBox *type_combo;
    QProgressBar *progress_bar;
    QPushButton *plot_button;
    void setupLayouts() override;
    void setupConnects() override;
    // plot map
    //  using below plotxxxMap with another thread
    void plotMap() override;
    void plotLineMap();
    void plotPieMap();

  private:
    const int WIDTH, HEIGHT;

    // plot settings
  private:
    int day, start_hour, end_hour, step_min;
};

#endif // TIMEPLOT_H
