#ifndef SPATICALPLOT_H
#define SPATICALPLOT_H

#include "common.h"
#include "heatmap.h"
#include "xxxplot.h"

// data filter + graph
class SpatialPlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit SpatialPlot(const int, const int, DataBase *, QThread *);
    QVBoxLayout *plot_layout;
    QTreeWidget *data_filter;
    QDateTimeEdit *start_edit, *end_edit;
    QDateTime start_datetime, end_datetime;
    QProgressBar *progress_bar;
    QPushButton *apply_button;
    HeatMap *main_widget;
    void setupLayouts() override;
    void setupConnects() override;
    void plotMap() override;

  private:
    const int WIDTH, HEIGHT;
    void plotHeat();
    void calcHeat(vector<vector<double>> &);
    enum TYPE { IN, OUT, FEE, TIME } item_type;
};

#endif // SPATICALPLOT_H
