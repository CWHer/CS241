#ifndef INFOPLOT_H
#define INFOPLOT_H

#include "common.h"
#include <QWidget>

class InfoPlot : public QWidget {
    Q_OBJECT
  public:
    explicit InfoPlot(const int, const int, QWidget *parent = nullptr);
    QWidget *main_widget;
    QTreeWidget *info_filter;
    void setupLayouts();

  private:
    const int WIDTH, HEIGHT;
  signals:
};

#endif // INFOPLOT_H
