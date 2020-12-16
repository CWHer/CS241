#ifndef ROUTEPLAN_H
#define ROUTEPLAN_H

#include "common.h"
#include "routemap.h"
#include "xxxplot.h"

class RoutePlan : public xxxPlot {
    Q_OBJECT
  public:
    explicit RoutePlan(const int, const int, DataBase *, QThread *);
    void setupLayouts() override;
    void setupConnects() override;
    void plotMap() override;
    RouteMap *main_widget;
    QLineEdit *start_lng_edit, *start_lat_edit;
    QLineEdit *end_lng_edit, *end_lat_edit;
    QDateTimeEdit *time_edit;
    QPushButton *apply_button;

  private:
    const int WIDTH, HEIGHT;
    QDateTime time_now;
    Pos src_pos, dst_pos;

    bool validPixel(pair<int, int> pos);
    const int dx[8] = {1, 1, 1, -1, -1, -1, 0, 0};
    const int dy[8] = {1, 0, -1, 1, 0, -1, 1, -1};
    const double eps = 1e-8;
    struct node {
        pair<int, int> x;
        double dist;
    };
    struct cmp {
        bool operator()(const node &a, const node &b) { return a.dist > b.dist; }
    };
    std::priority_queue<node, vector<node>, cmp> Q;
    vector<vector<double>> pixel_cnt;
    vector<vector<double>> dist;
    vector<vector<bool>> used;
    vector<vector<pair<int, int>>> pre;
    vector<pair<int, int>> path;

    void calcArea();
    void Dijkstra(pair<int, int>);

  signals:
};

#endif // ROUTEPLAN_H
