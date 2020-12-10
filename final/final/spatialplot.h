#ifndef SPATICALPLOT_H
#define SPATICALPLOT_H

#include "common.h"
#include "database.h"

// data filter + graph
class SpatialPlot : public QWidget {
    Q_OBJECT
  public:
    explicit SpatialPlot(const int, const int, QWidget *parent = nullptr);
    QVBoxLayout *plot_layout;
    QTreeWidget *data_filter;
    QProgressBar *bar;
    QChartView *plot_area;
    QWidget *main_widget;
    void setupLayouts();

  private:
    const int WIDTH, HEIGHT;
  signals:
};

#endif // SPATICALPLOT_H
