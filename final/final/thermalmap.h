#ifndef THERMALMAP_H
#define THERMALMAP_H

#include "common.h"
#include <QWidget>

class ThermalMap : public QWidget {
    Q_OBJECT
  public:
    explicit ThermalMap(const int &, const int &);
    void paintEvent(QPaintEvent *) override;

  private:
    static const int EDGE_SIZE = 20;
  signals:
};

#endif // THERMALMAP_H
