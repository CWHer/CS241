#include "spatialplot.h"

SpatialPlot::SpatialPlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

void SpatialPlot::setupLayouts() {
    QFont font = QFont("consolas", 10);
    main_widget = new QWidget();
    auto outer_layout = new QHBoxLayout(main_widget);

    // left part begin
    auto left_part = new QVBoxLayout();

    // initialize data_filter
    data_filter = new QTreeWidget();
    data_filter->setHeaderLabel("Select");
    data_filter->setFont(font);
    // add date
    QTreeWidgetItem *date = new QTreeWidgetItem(data_filter);
    date->setText(0, "Date");
    date->setFont(0, font);
    date->setFlags(date->flags() | Qt::ItemIsAutoTristate);
    for (auto i = 1; i <= DAY_NUM; ++i) {
        auto ch = new QTreeWidgetItem(date);
        ch->setText(0, "11-" + QString::number(i));
        ch->setFont(0, font);
        ch->setCheckState(0, Qt::Unchecked);
    }
    // add item
    QTreeWidgetItem *item = new QTreeWidgetItem(data_filter);
    item->setText(0, "Item");
    item->setFont(0, font);
    item->setFlags(date->flags() | Qt::ItemIsAutoTristate);
    QStringList tag_list = {"aaa", "bbb", "ccc"};
    for (const auto &tag : tag_list) {
        auto ch = new QTreeWidgetItem(item);
        ch->setText(0, tag);
        ch->setFont(0, font);
        ch->setCheckState(0, Qt::Unchecked);
    }

    data_filter->setMinimumWidth(WIDTH / 5);
    data_filter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addWidget(data_filter);

    // progress bar and apply button
    auto progress_part = new QHBoxLayout();

    bar = new QProgressBar();
    bar->setRange(0, 100);
    bar->setValue(0);
    bar->setTextVisible(false);
    bar->setMinimumWidth(WIDTH / 5 / 4 * 3);
    bar->setMinimumHeight(20);
    bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    progress_part->addWidget(bar);

    auto button = new QPushButton();
    button->setText("apply");
    button->setFont(font);
    button->setMinimumWidth(60);
    progress_part->addWidget(button);

    left_part->addLayout(progress_part);
    outer_layout->addLayout(left_part);
    // left part end

    // right part begin
    auto right_part = new QVBoxLayout();

    // initialize plot area
    plot_area = new QChartView();
    plot_area->setMinimumSize(WIDTH / 5 * 4, HEIGHT / 4 * 3);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    right_part->addWidget(plot_area);

    // initialize button group
    auto button_group = new QHBoxLayout();
    auto time_button = new QPushButton();
    button_group->addWidget(time_button);
    auto place_button = new QPushButton();
    button_group->addWidget(place_button);

    right_part->addLayout(button_group);
    outer_layout->addLayout(right_part);
    // right part end
}

void SpatialPlot::plotMap() {}
