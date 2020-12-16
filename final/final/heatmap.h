#ifndef HEATMAP_H
#define HEATMAP_H

#include "common.h"
#include <QWidget>

class HeatMap : public QWidget {
    Q_OBJECT
  public:
    explicit HeatMap(const int &, const int &);
    void paintEvent(QPaintEvent *) override;
    void updateMap(const vector<vector<double>> &);
    // map size
    const int IMG_SIZE;
    // map <---> widget
    static const int EDGE_SIZE = 20;
    enum TYPE { INIT, UPDATE, FREE } type;
    pair<int, int> pos2pixel(pair<int, int>);
    pair<int, int> pixel2pos(pair<int, int>);

  private:
    QImage heat_img, alpha_img;
    // point raduis
    static const int RADIUS = 10;
    static const int COL_MAX = 256;
    // heat map alpha
    static constexpr double HEAT_ALPHA = 200.0;
    QRgb col_list[COL_MAX];
};

#endif // HEATMAP_H
