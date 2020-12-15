#include "routeplan.h"

RoutePlan::RoutePlan(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

void RoutePlan::setupLayouts() {
    QFont font("consolas", 10);
    QDateTime default_date(QDate(2016, 11, 1), QTime(0, 0));
    main_widget = new HeatMap(WIDTH, HEIGHT);
    auto outer_layout = new QHBoxLayout(main_widget);

    // left part begin
    auto left_part = new QVBoxLayout();

    // start
    auto start_label = new QLabel();
    start_label->setText("Start Position");
    start_label->setFont(font);
    left_part->addWidget(start_label);
    start_lng_edit = new QLineEdit();
    start_lng_edit->setText("lng = ");
    start_lng_edit->setFont(font);
    left_part->addWidget(start_lng_edit);
    start_lat_edit = new QLineEdit();
    start_lat_edit->setText("lat = ");
    start_lat_edit->setFont(font);
    left_part->addWidget(start_lat_edit);

    auto v_spacer1 = new QSpacerItem(20, 20, QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addItem(v_spacer1);

    // end
    auto end_label = new QLabel();
    end_label->setText("End Position");
    end_label->setFont(font);
    left_part->addWidget(end_label);
    end_lng_edit = new QLineEdit();
    end_lng_edit->setText("lng = ");
    end_lng_edit->setFont(font);
    left_part->addWidget(end_lng_edit);
    end_lat_edit = new QLineEdit();
    end_lat_edit->setText("lat = ");
    end_lat_edit->setFont(font);
    left_part->addWidget(end_lat_edit);

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
    apply_button->setText("apply");
    apply_button->setFont(font);
    apply_button->setMinimumWidth(60);
    button_part->addWidget(apply_button);
    left_part->addLayout(button_part);

    outer_layout->addLayout(left_part);
    // left part end

    // plot area
    auto spacer = new QSpacerItem(HEIGHT, HEIGHT, QSizePolicy::Expanding, QSizePolicy::Expanding);
    outer_layout->addItem(spacer);
}

void RoutePlan::setupConnects() {
    //
}

void RoutePlan::plotMap() {
    //
}
