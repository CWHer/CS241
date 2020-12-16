#ifndef ROUTEMAP_H
#define ROUTEMAP_H

#include "common.h"
#include <QWidget>

class RouteMap : public QWidget {
    Q_OBJECT
  public:
    explicit RouteMap(const int &, const int &);
    void paintEvent(QPaintEvent *) override;
    void updateMap(const vector<vector<double>> &, const vector<pair<int, int>> &);
    // map size
    const int IMG_SIZE;
    // map <---> widget
    static const int EDGE_SIZE = 20;
    enum TYPE { INIT, UPDATE, FREE } type;
    pair<int, int> pos2pixel(pair<int, int>);
    pair<int, int> pixel2pos(pair<int, int>);

  private:
    const double eps = 1e-8;
    QImage alpha_img, route_img;
    // point raduis
    static const int RADIUS = 6;
    static const int COL_MAX = 256;
    // heat map alpha
    static constexpr double ROUTE_ALPHA = 255;
    QRgb col_list[COL_MAX];

  signals:
};

#endif // ROUTEMAP_H
