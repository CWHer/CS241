#ifndef SPATICALPLOT_H
#define SPATICALPLOT_H

#include "common.h"
#include "xxxplot.h"

// data filter + graph
class SpatialPlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit SpatialPlot(const int, const int, DataBase *, QThread *);
    QVBoxLayout *plot_layout;
    QTreeWidget *data_filter;
    QProgressBar *bar;
    QChartView *plot_area;
    QWidget *main_widget;
    void setupLayouts() override;
    void plotMap() override;

  private:
    const int WIDTH, HEIGHT;
};

#endif // SPATICALPLOT_H
