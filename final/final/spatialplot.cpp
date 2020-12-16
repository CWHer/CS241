#include "spatialplot.h"

SpatialPlot::SpatialPlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

void SpatialPlot::setupLayouts() {
    QFont font("consolas", 10);
    QDateTime default_date(QDate(2016, 11, 1), QTime(0, 0));
    main_widget = new HeatMap(WIDTH, HEIGHT);
    auto outer_layout = new QHBoxLayout(main_widget);
    // left part begin
    auto left_part = new QVBoxLayout();

    // initialize data_filter
    data_filter = new QTreeWidget();
    data_filter->setHeaderLabel("Item");
    data_filter->setFont(font);
    //    item->setFlags(item->flags() | Qt::ItemIsAutoTristate);
    QStringList tag_list = {"orders in", "orders out", "fee", "time"};
    for (const auto &tag : tag_list) {
        auto ch = new QTreeWidgetItem(data_filter);
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

    // plot area
    auto spacer = new QSpacerItem(HEIGHT, HEIGHT, QSizePolicy::Expanding, QSizePolicy::Expanding);
    right_part->addItem(spacer);
    outer_layout->addLayout(right_part);
    // right part end
}
void SpatialPlot::setupConnects() {
    //
    connect(apply_button, &QPushButton::clicked, this, &SpatialPlot::plotMap);
}

void SpatialPlot::plotMap() {
    main_widget->type = HeatMap::FREE;
    progress_bar->setValue(0);
    // get settings
    start_datetime = start_edit->dateTime();
    end_datetime = end_edit->dateTime();
    bool is_checked = false;
    for (int i = 0; i < data_filter->topLevelItemCount(); ++i)
        if (data_filter->topLevelItem(i)->checkState(0) == Qt::Checked) {
            item_type = (TYPE)i;
            is_checked = true;
            break;
        }
    if (!is_checked) return;

    progress_bar->setValue(20);
    plotHeat();
}

void SpatialPlot::calcHeat(vector<vector<double>> &pixel_cnt) {
    const int PIXEL_SIZE = main_widget->IMG_SIZE;
    pixel_cnt.resize(PIXEL_SIZE);
    auto start_time = start_datetime.toSecsSinceEpoch();
    auto end_time = end_datetime.toSecsSinceEpoch();
    for (auto &wi : pixel_cnt)
        wi.resize(PIXEL_SIZE);
    db->IMG_SIZE = PIXEL_SIZE;

    switch (item_type) {
        case IN:
            db->startCount(pixel_cnt, start_time, end_time);
            break;
        case OUT:
            db->endCount(pixel_cnt, start_time, end_time);
            break;
        case FEE:
            db->feeCount(pixel_cnt, start_time, end_time);
            break;
        case TIME:
            db->timeCount(pixel_cnt, start_time, end_time);
            break;
    }
}

void SpatialPlot::plotHeat() {
    vector<vector<double>> pixel_cnt;
    calcHeat(pixel_cnt);
    progress_bar->setValue(40);
    main_widget->updateMap(pixel_cnt);
    progress_bar->setValue(100);
}
