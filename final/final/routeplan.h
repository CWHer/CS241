#ifndef ROUTEPLAN_H
#define ROUTEPLAN_H

#include "common.h"
#include "heatmap.h"
#include "xxxplot.h"

class RoutePlan : public xxxPlot {
    Q_OBJECT
  public:
    explicit RoutePlan(const int, const int, DataBase *, QThread *);
    void setupLayouts() override;
    void setupConnects() override;
    void plotMap() override;
    QWidget *main_widget;
    QLineEdit *start_lng_edit, *start_lat_edit;
    QLineEdit *end_lng_edit, *end_lat_edit;
    QDateTimeEdit *time_edit;
    QPushButton *apply_button;

  private:
    const int WIDTH, HEIGHT;

  signals:
};

#endif // ROUTEPLAN_H
