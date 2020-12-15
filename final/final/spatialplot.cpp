#include "spatialplot.h"

SpatialPlot::SpatialPlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

void SpatialPlot::setupLayouts() {
    QFont font = QFont("consolas", 10);
    QDateTime default_date(QDate(2016, 11, 1), QTime(0, 0));
    main_widget = new ThermalMap(WIDTH, HEIGHT);
    auto outer_layout = new QHBoxLayout(main_widget);
    // left part begin
    auto left_part = new QVBoxLayout();

    // initialize data_filter
    data_filter = new QTreeWidget();
    data_filter->setHeaderLabel("Select");
    data_filter->setFont(font);
    // add item
    QTreeWidgetItem *item = new QTreeWidgetItem(data_filter);
    item->setText(0, "Item");
    item->setFont(0, font);
    item->setFlags(item->flags() | Qt::ItemIsAutoTristate);
    QStringList tag_list = {"orders in", "orders out", "fee", "time"};
    for (const auto &tag : tag_list) {
        auto ch = new QTreeWidgetItem(item);
        ch->setText(0, tag);
        ch->setFont(0, font);
        ch->setCheckState(0, Qt::Unchecked);
    }

    data_filter->setMinimumWidth(WIDTH - HEIGHT);
    data_filter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addWidget(data_filter);
    // data filter end

    // date select
    auto selection_part = new QVBoxLayout();

    //  start time select
    auto start_time_label = new QLabel();
    start_time_label->setText("Start Time");
    start_time_label->setFont(font);
    start_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    selection_part->addWidget(start_time_label);
    start_edit = new QDateTimeEdit();
    start_edit->setFont(font);
    start_edit->setDateTime(default_date);
    selection_part->addWidget(start_edit);
    selection_part->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));

    //  end time select
    auto end_time_label = new QLabel();
    end_time_label->setText("End Time");
    end_time_label->setFont(font);
    end_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    selection_part->addWidget(end_time_label);
    end_edit = new QDateTimeEdit();
    end_edit->setFont(font);
    end_edit->setDateTime(default_date);
    selection_part->addWidget(end_edit);
    selection_part->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Expanding));

    left_part->addLayout(selection_part);
    // date select end

    // progress bar and apply button
    auto progress_part = new QHBoxLayout();

    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 5 / 4 * 3);
    progress_bar->setMinimumHeight(20);
    progress_bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    progress_part->addWidget(progress_bar);

    apply_button = new QPushButton();
    apply_button->setText("apply");
    apply_button->setFont(font);
    apply_button->setMinimumWidth(60);
    progress_part->addWidget(apply_button);

    left_part->addLayout(progress_part);
    outer_layout->addLayout(left_part);
    // left part end

    // right part begin
    auto right_part = new QVBoxLayout();

    auto spacer = new QSpacerItem(HEIGHT, HEIGHT, QSizePolicy::Expanding, QSizePolicy::Expanding);
    right_part->addItem(spacer);
    // initialize plot area
    //    plot_area = new QWidget();
    //    plot_area->setMinimumSize(WIDTH / 5 * 4, HEIGHT / 4 * 3);
    //    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    //    right_part->addWidget(plot_area);
    //********************
    //  under refactor
    // initialize button group
    auto button_group = new QHBoxLayout();
    auto time_button = new QPushButton();
    button_group->addWidget(time_button);
    auto place_button = new QPushButton();
    button_group->addWidget(place_button);

    //    right_part->addLayout(button_group);
    outer_layout->addLayout(right_part);
    // right part end

    //    plot_area->setStyleSheet("back.jpg");
}
void SpatialPlot::setupConnects() {
    //
    connect(apply_button, &QPushButton::clicked, this, &SpatialPlot::plotMap);
}

void SpatialPlot::plotMap() {
    progress_bar->setValue(0);
    // get settings
    start_datetime = start_edit->dateTime();
    end_datetime = end_edit->dateTime();
    for (int i = 0; i < data_filter->topLevelItemCount(); ++i)
        if (data_filter->topLevelItem(i)->checkState(0) == Qt::Checked) {
            item_type = data_filter->topLevelItem(i)->text(0);
            break;
        }
    progress_bar->setValue(33);

    plotThermal();
}

void SpatialPlot::plotThermal() {
    //
}
