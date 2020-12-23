#include "routeplan.h"

RoutePlan::RoutePlan(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

void RoutePlan::setupLayouts() {
    QFont font("consolas", 10);
    QDateTime default_date(QDate(2016, 11, 1), QTime(0, 0));
    main_widget = new RouteMap(WIDTH, HEIGHT);
    auto outer_layout = new QHBoxLayout(main_widget);

    // left part begin
    auto left_part = new QVBoxLayout();

    // start
    auto start_label = new QLabel();
    start_label->setText("Start Position");
    start_label->setFont(font);
    left_part->addWidget(start_label);

    auto start_lng = new QHBoxLayout();
    auto start_lng_label = new QLabel();
    start_lng_label->setText("lng = ");
    start_lng_label->setFont(font);
    start_lng->addWidget(start_lng_label);
    start_lng_edit = new QLineEdit();
    start_lng_edit->setFont(font);
    start_lng_edit->setText("104.00");
    start_lng->addWidget(start_lng_edit);
    left_part->addLayout(start_lng);

    auto start_lat = new QHBoxLayout();
    auto start_lat_label = new QLabel();
    start_lat_label->setText("lat = ");
    start_lat_label->setFont(font);
    start_lat->addWidget(start_lat_label);
    start_lat_edit = new QLineEdit();
    start_lat_edit->setText("30.60");
    start_lat_edit->setFont(font);
    start_lat->addWidget(start_lat_edit);
    left_part->addLayout(start_lat);

    auto v_spacer1 = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addItem(v_spacer1);

    // end
    auto end_label = new QLabel();
    end_label->setText("End Position");
    end_label->setFont(font);
    left_part->addWidget(end_label);

    auto end_lng = new QHBoxLayout();
    auto end_lng_label = new QLabel();
    end_lng_label->setText("lng = ");
    end_lng_label->setFont(font);
    end_lng->addWidget(end_lng_label);
    end_lng_edit = new QLineEdit();
    end_lng_edit->setText("104.14");
    end_lng_edit->setFont(font);
    end_lng->addWidget(end_lng_edit);
    left_part->addLayout(end_lng);

    auto end_lat = new QHBoxLayout();
    auto end_lat_label = new QLabel();
    end_lat_label->setText("lat = ");
    end_lat_label->setFont(font);
    end_lat->addWidget(end_lat_label);
    end_lat_edit = new QLineEdit();
    end_lat_edit->setText("30.73");
    end_lat_edit->setFont(font);
    end_lat->addWidget(end_lat_edit);
    left_part->addLayout(end_lat);

    // place end

    auto v_spacer2 = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addItem(v_spacer2);

    auto time_label = new QLabel();
    time_label->setText("Travel Time");
    time_label->setFont(font);
    left_part->addWidget(time_label);
    time_edit = new QDateTimeEdit();
    time_edit->setFont(font);
    time_edit->setDateTime(default_date);
    left_part->addWidget(time_edit);

    auto v_spacer3 = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addItem(v_spacer3);

    // apply button
    auto button_part = new QHBoxLayout();
    auto h_spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);
    button_part->addItem(h_spacer);
    apply_button = new QPushButton();
    apply_button->setText("Apply");
    apply_button->setFont(font);
    apply_button->setMinimumWidth(60);
    button_part->addWidget(apply_button);
    left_part->addLayout(button_part);

    outer_layout->addLayout(left_part);
    // left part end

    // plot area
    auto spacer = new QSpacerItem(HEIGHT, HEIGHT, QSizePolicy::Expanding, QSizePolicy::Expanding);
    outer_layout->addItem(spacer);

    const int PIXEL_SIZE = main_widget->IMG_SIZE;
    pixel_cnt.resize(PIXEL_SIZE);
    dist.resize(PIXEL_SIZE);
    used.resize(PIXEL_SIZE);
    pre.resize(PIXEL_SIZE);
    for (auto i = 0; i < PIXEL_SIZE; ++i) {
        pixel_cnt[i].resize(PIXEL_SIZE);
        dist[i].resize(PIXEL_SIZE);
        used[i].resize(PIXEL_SIZE);
        pre[i].resize(PIXEL_SIZE);
    }
}

void RoutePlan::setupConnects() {
    //
    connect(apply_button, &QPushButton::clicked, this, &RoutePlan::plotMap);
}

void RoutePlan::plotMap() {
    // get settings
    time_now = time_edit->dateTime();
    src_pos = make_pair(start_lng_edit->text().toDouble(), start_lat_edit->text().toDouble());
    dst_pos = make_pair(end_lng_edit->text().toDouble(), end_lat_edit->text().toDouble());
    calcArea();

    // find shortest path with pixel_cnt
    db->IMG_SIZE = main_widget->IMG_SIZE;
    pair<int, int> src = db->pos2pixel(src_pos.first, src_pos.second);
    pair<int, int> dst = db->pos2pixel(dst_pos.first, dst_pos.second);
    Dijkstra(src);
    //    qDebug() << dist[dst.first][dst.second] << " ";
    path.clear();
    for (auto x = dst; x != src; x = pre[x.first][x.second])
        path.push_back(x);

    main_widget->updateMap(pixel_cnt, path);
}

bool RoutePlan::validPixel(pair<int, int> pos) {
    const int PIXEL_SIZE = main_widget->IMG_SIZE;
    return (pos.first >= 0 && pos.first < PIXEL_SIZE) && (pos.second >= 0 && pos.second < PIXEL_SIZE);
}

void RoutePlan::Dijkstra(pair<int, int> src) {
    // init
    const int BASE = 10;
    const int PIXEL_SIZE = main_widget->IMG_SIZE;
    for (int i = 0; i < PIXEL_SIZE; ++i)
        for (int j = 0; j < PIXEL_SIZE; ++j) {
            // regular pixel_cnt
            pixel_cnt[i][j] += BASE;
            pixel_cnt[i][j] = sqrt(pixel_cnt[i][j]);
            dist[i][j] = std::numeric_limits<double>::max();
            used[i][j] = false;
        }

    dist[src.first][src.second] = 0;
    Q.push((node){src, 0});
    while (!Q.empty()) {
        pair<int, int> x = Q.top().x;
        double dis = Q.top().dist;
        Q.pop(), used[x.first][x.second] = true;
        for (int i = 0; i < 8; ++i) {
            pair<int, int> nxt = make_pair(x.first + dx[i], x.second + dy[i]);
            if (!validPixel(nxt)) continue;
            if (dis + pixel_cnt[nxt.first][nxt.second] < dist[nxt.first][nxt.second]) {
                dist[nxt.first][nxt.second] = dis + pixel_cnt[nxt.first][nxt.second];
                pre[nxt.first][nxt.second] = x;
                Q.push((node){nxt, dist[nxt.first][nxt.second]});
            }
        }
        while (!Q.empty() && used[Q.top().x.first][Q.top().x.second])
            Q.pop();
    }
}

void RoutePlan::calcArea() {
    const int PIXEL_SIZE = main_widget->IMG_SIZE;
    const int step_min = 60;
    auto start_time = time_now.toSecsSinceEpoch() - step_min / 2 * 60;
    auto end_time = time_now.toSecsSinceEpoch() + step_min / 2 * 60;
    for (int i = 0; i < PIXEL_SIZE; ++i)
        for (int j = 0; j < PIXEL_SIZE; ++j)
            pixel_cnt[i][j] = 0;
    db->startCount(pixel_cnt, start_time, end_time);

    double max_value = 0;
    for (const auto &wi : pixel_cnt)
        for (const auto &num : wi)
            max_value = std::max(max_value, num);

    // draw alpha map
    QImage alpha_img(PIXEL_SIZE, PIXEL_SIZE, QImage::Format_Alpha8);
    alpha_img.fill(Qt::transparent);
    QPainter painter(&alpha_img);
    painter.setPen(Qt::transparent);
    const int RADIUS = 8;
    for (int i = 0; i < PIXEL_SIZE; ++i) {
        for (int j = 0; j < PIXEL_SIZE; ++j)
            if (pixel_cnt[i][j] > eps) {
                double alpha = pixel_cnt[i][j] / max_value * 255;
                QRadialGradient grad(i, j, RADIUS);
                grad.setColorAt(0, QColor(0, 0, 0, alpha));
                grad.setColorAt(1, QColor(0, 0, 0, 0));
                painter.setBrush(grad);
                painter.drawEllipse(QPoint(i, j), RADIUS, RADIUS);
            }
    }

    for (int i = 0; i < PIXEL_SIZE; ++i)
        for (int j = 0; j < PIXEL_SIZE; ++j) {
            pixel_cnt[i][j] = alpha_img.pixel(i, j) >> 24;
        }
}
