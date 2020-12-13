#ifndef INFOPLOT_H
#define INFOPLOT_H

#include "common.h"
#include "xxxplot.h"

class InfoPlot : public xxxPlot {
    Q_OBJECT
  public:
    explicit InfoPlot(const int, const int, DataBase *, QThread *);
    QWidget *main_widget;
    QTreeWidget *info_filter;
    void setupLayouts() override;
    void plotMap() override;

  private:
    const int WIDTH, HEIGHT;
};

#endif // INFOPLOT_H
