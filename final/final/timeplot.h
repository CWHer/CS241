#ifndef TIMEPLOT_H
#define TIMEPLOT_H

#include "common.h"
#include <QWidget>

// data vs time
//  line charts and pie charts
class TimePlot : public QWidget {
    Q_OBJECT
  public:
    explicit TimePlot(const int, const int, QWidget *parent = nullptr);
    QWidget *main_widget;
    QChartView *plot_area;
    void setupLayouts();

  private:
    const int WIDTH, HEIGHT;
  signals:
};

#endif // TIMEPLOT_H
