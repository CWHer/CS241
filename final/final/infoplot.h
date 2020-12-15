#ifndef INFOPLOT_H
#define INFOPLOT_H

#include "common.h"
#include "gridselector.h"
#include "xxxplot.h"

class InfoPlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit InfoPlot(const int, const int, DataBase *, QThread *);
    QWidget *main_widget;
    void setupLayouts() override;
    void setupConnects() override;
    void plotMap() override;

  private:
    GridSelector *selector;
    QTreeWidget *info_filter;
    QProgressBar *progress_bar;
    QPushButton *apply_button, *select_button;
    QChartView *plot_area;
    vector<int> grid_id;
    // time(Msec) + num
    void calcSeries(vector<pair<long long, double>> &);
    // time(Msec) + in_num + out_num
    void calcArea(vector<pair<long long, pair<double, double>>> &);
    void plotSeries();
    void plotArea();

  private:
    const int WIDTH, HEIGHT;
    enum InfoType { TIME, FEE, IO } info;
    enum IntervalType { ALL, DAY } interval;
};

#endif // INFOPLOT_H
