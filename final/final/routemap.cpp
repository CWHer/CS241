#include "routemap.h"

RouteMap::RouteMap(const int &w, const int &h)
    : IMG_SIZE(h - 2 * EDGE_SIZE) {
    //
    this->resize(w, h);
    route_img = QImage(IMG_SIZE, IMG_SIZE, QImage::Format_ARGB32);
    alpha_img = QImage(IMG_SIZE, IMG_SIZE, QImage::Format_Alpha8);
    type = INIT;

    // generate gradient color list
    QLinearGradient col_grad = QLinearGradient(QPoint(0, 0), QPoint(255, 0));
    col_grad.setColorAt(0, Qt::blue);
    col_grad.setColorAt(0.4, Qt::blue);
    col_grad.setColorAt(0.5, Qt::cyan);
    col_grad.setColorAt(0.6, Qt::green);
    col_grad.setColorAt(0.8, Qt::yellow);
    col_grad.setColorAt(0.95, Qt::red);

    QImage grad_img(COL_MAX, 1, QImage::Format_ARGB32);
    QPainter painter(&grad_img);
    painter.fillRect(grad_img.rect(), col_grad);

    // a method to generate gradient color
    //  from https://blog.csdn.net/gongjianbo1992/article/details/104566768
    //  MIT lisence
    unsigned alpha = 0;
    for (unsigned i = 0; i < COL_MAX; ++i) {
        alpha = ROUTE_ALPHA / 255 * i;
        col_list[i] = (grad_img.pixel(i, 0) & 0x00FFFFFF) | (alpha << 24);
    }
}

pair<int, int> RouteMap::pos2pixel(pair<int, int> pos) {
    //
    return make_pair(width() - height() + EDGE_SIZE + pos.first, EDGE_SIZE + pos.second);
}
pair<int, int> RouteMap::pixel2pos(pair<int, int> pixel_pos) {
    //
    return make_pair(pixel_pos.first - EDGE_SIZE - height() + width(), pixel_pos.second - EDGE_SIZE);
}

void RouteMap::paintEvent(QPaintEvent *event) {
    QPainter pen;
    pen.begin(this);
    if (type != FREE)
        pen.drawPixmap(width() - height() + EDGE_SIZE, EDGE_SIZE, IMG_SIZE, IMG_SIZE,
                       std::move(QPixmap("../background.png")));
    if (type == UPDATE) {
        pen.drawImage(width() - height() + EDGE_SIZE, EDGE_SIZE, route_img);
    }
    pen.end();
}

void RouteMap::updateMap(const vector<vector<double>> &pixel_cnt, const vector<pair<int, int>> &path) {
    alpha_img.fill(Qt::transparent);
    double max_value = 0;
    for (const auto &wi : pixel_cnt)
        for (const auto &num : wi)
            max_value = std::max(max_value, num);

    // avoid divide by zero
    if (std::fabs(max_value) < eps) return;
    // baseline
    max_value = std::max(max_value, 10.0);

    // draw alpha route map
    QPainter painter(&alpha_img);
    painter.setPen(Qt::transparent);
    for (const auto &pi : path) {
        double alpha = pixel_cnt[pi.first][pi.second] / max_value * 255;
        QRadialGradient grad(pi.first, pi.second, RADIUS);
        grad.setColorAt(0, QColor(0, 0, 0, alpha));
        grad.setColorAt(1, QColor(0, 0, 0, 0));
        painter.setBrush(grad);
        painter.drawEllipse(QPoint(pi.first, pi.second), RADIUS, RADIUS);
    }

    // alpha map->heat route map
    for (int i = 0; i < alpha_img.height(); i++) {
        const uchar *line_data = alpha_img.scanLine(i);
        QRgb *line_heat = reinterpret_cast<QRgb *>(route_img.scanLine(i));
        for (int j = 0; j < alpha_img.width(); j++)
            line_heat[j] = col_list[line_data[j]];
    }
    type = UPDATE;
    update();
}
