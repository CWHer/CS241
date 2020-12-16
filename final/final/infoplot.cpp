#include "infoplot.h"

InfoPlot::InfoPlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {
    selector = new GridSelector(true);
}

void InfoPlot::setupLayouts() {
    QFont font = QFont("consolas", 10);
    main_widget = new QWidget();
    auto outer_layout = new QHBoxLayout(main_widget);

    // left part begin
    auto left_part = new QVBoxLayout();

    // info filter
    info_filter = new QTreeWidget();
    info_filter->setHeaderLabel("Infomation");
    info_filter->setFont(font);

    // add important information
    //  xxx's average vs time
    // when interval == DAY
    //  gather data of same hour each day together
    auto ch1 = new QTreeWidgetItem(info_filter);
    ch1->setText(0, "travel time overall");
    ch1->setFont(0, font);
    ch1->setCheckState(0, Qt::Unchecked);
    auto ch2 = new QTreeWidgetItem(info_filter);
    ch2->setText(0, "travel time a day");
    ch2->setFont(0, font);
    ch2->setCheckState(0, Qt::Unchecked);
    auto ch3 = new QTreeWidgetItem(info_filter);
    ch3->setText(0, "order fees overall");
    ch3->setFont(0, font);
    ch3->setCheckState(0, Qt::Unchecked);
    auto ch4 = new QTreeWidgetItem(info_filter);
    ch4->setText(0, "order fees a day");
    ch4->setFont(0, font);
    ch4->setCheckState(0, Qt::Unchecked);
    auto ch5 = new QTreeWidgetItem(info_filter);
    ch5->setText(0, "orders in/out a day");
    ch5->setFont(0, font);
    ch5->setCheckState(0, Qt::Unchecked);

    info_filter->setMinimumWidth(WIDTH / 4);
    info_filter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addWidget(info_filter);
    // filter end

    // select and apply button
    auto button_part = new QHBoxLayout();

    select_button = new QPushButton();
    select_button->setText("Select");
    select_button->setFont(font);
    select_button->setMinimumWidth(60);
    button_part->addWidget(select_button);

    apply_button = new QPushButton();
    apply_button->setText("apply");
    apply_button->setFont(font);
    apply_button->setMinimumWidth(60);
    button_part->addWidget(apply_button);

    left_part->addLayout(button_part);

    // progress bar
    auto progress_part = new QHBoxLayout();
    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 5 / 4 * 3);
    progress_bar->setMinimumHeight(20);
    progress_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    progress_part->addWidget(progress_bar);

    left_part->addLayout(progress_part);
    outer_layout->addLayout(left_part);
    // left part end

    // right part begin
    plot_area = new QChartView();
    plot_area->setRubberBand(QChartView::HorizontalRubberBand);
    plot_area->setRenderHint(QPainter::Antialiasing);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // right part end

    outer_layout->addWidget(plot_area);
}

void InfoPlot::setupConnects() {
    connect(apply_button, &QPushButton::clicked, this, &InfoPlot::plotMap);
    connect(select_button, &QPushButton::clicked, selector, &GridSelector::selectGrid);
}

void InfoPlot::plotMap() {
    progress_bar->setValue(0);
    // get settings
    QString plot_type;
    for (int i = 0; i < info_filter->topLevelItemCount(); ++i)
        if (info_filter->topLevelItem(i)->checkState(0) == Qt::Checked) {
            plot_type = info_filter->topLevelItem(i)->text(0);
            break;
        }

    if (plot_type.isEmpty()) return;
    info = plot_type.indexOf("time") != -1 ? TIME : FEE;
    interval = plot_type.indexOf("day") != -1 ? DAY : ALL;
    grid_id = selector->grid_id;
    progress_bar->setValue(33);

    // tend to use another thread to plot
    //  however fast enough to only use main
    if (plot_type.indexOf("in/out") != -1) {
        plotArea();
        return;
    }
    plotSeries();
}

void InfoPlot::plotSeries() {
    QFont font("consolas", 10);
    auto series = new QSplineSeries();
    vector<pair<long long, double>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(90);
    double max_value = 0;
    for (const auto &wi : data_series) {
        series->append(wi.first, wi.second);
        max_value = std::max(max_value, wi.second);
    }

    auto chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    //    chart->setTitle("Number of Orders");
    chart->setFont(font);

    // x_axis
    auto x_axis = new QDateTimeAxis();
    x_axis->setTickCount(10);

    if (interval == DAY) {
        x_axis->setFormat("hh:mm");
        x_axis->setRange(QDateTime(QDate(2016, 11, 1), QTime(0, 0)), QDateTime(QDate(2016, 11, 2), QTime(0, 0)));
    } else {
        x_axis->setFormat("d hh:mm");
        x_axis->setRange(QDateTime(QDate(2016, 11, 1), QTime(0, 0)), QDateTime(QDate(2016, 11, 16), QTime(0, 0)));
    }
    x_axis->setTitleText("time");
    x_axis->setGridLineVisible(true);
    x_axis->setLabelsAngle(-45);
    x_axis->setLabelsFont(font);
    x_axis->setLineVisible(true);
    chart->addAxis(x_axis, Qt::AlignBottom);

    // y_axis
    auto y_axis = new QValueAxis();
    y_axis->setLabelFormat("%.2f");
    y_axis->setTitleText(info == FEE ? "average fee (RMB)" : "average time (min)");
    y_axis->setRange(0, max_value);
    y_axis->setLabelsFont(font);
    chart->addAxis(y_axis, Qt::AlignLeft);

    chart->addSeries(series);
    series->attachAxis(x_axis);
    series->attachAxis(y_axis);
    plot_area->setChart(chart);
    progress_bar->setValue(100);
}

void InfoPlot::calcSeries(vector<pair<long long, double>> &series) {
    const int step_min = 20;
    if (interval == DAY) {
        auto offset = QDateTime(QDate(2016, 11, 1), QTime(0, 0)).toMSecsSinceEpoch();
        series.resize(24 * 60 / step_min);
        for (long long i = 0; i < 24 * 60; i += step_min)
            series[i / step_min].first = offset + i * 60 * 1000;
    }
    for (auto day = 1; day <= DAY_NUM; ++day) {
        for (auto i = 0; i + step_min <= 24 * 60; i += step_min) {
            QDateTime time_now(QDate(2016, 11, day), QTime(i / 60, i % 60));
            auto start_time = time_now.toSecsSinceEpoch();
            auto end_time = start_time + step_min * 60;
            double num = 0;
            for (auto &id : grid_id)
                num += info == FEE ? db->feeCount(id, start_time, end_time) : db->timeCount(id, start_time, end_time);
            if (info == TIME) num /= 60;
            if (interval == DAY)
                series[i / step_min].second += num / GRID_NUM;
            else
                series.push_back(make_pair(time_now.toMSecsSinceEpoch(), num / GRID_NUM));
        }
    }
    if (interval == DAY)
        for (auto &wi : series)
            wi.second /= DAY_NUM;
}

void InfoPlot::calcArea(vector<pair<long long, pair<double, double>>> &area) {
    const int step_min = 20;
    auto offset = QDateTime(QDate(2016, 11, 1), QTime(0, 0)).toMSecsSinceEpoch();
    area.resize(24 * 60 / step_min);
    for (auto i = 0; i < 24 * 60; i += step_min)
        area[i / step_min].first = offset + i * 60 * 1000;
    for (auto day = 1; day <= DAY_NUM; ++day) {
        for (long long i = 0; i + step_min <= 24 * 60; i += step_min) {
            QDateTime time_now(QDate(2016, 11, day), QTime(i / 60, i % 60));
            auto start_time = time_now.toSecsSinceEpoch();
            auto end_time = start_time + step_min * 60;
            double in_num = 0, out_num = 0;
            for (auto &id : grid_id) {
                in_num += db->endCount(id, start_time, end_time);
                out_num += db->startCount(id, start_time, end_time);
            }
            area[i / step_min].second.first += in_num;
            area[i / step_min].second.second += out_num;
        }
    }
    for (auto &wi : area) {
        wi.second.first /= DAY_NUM;
        wi.second.second /= DAY_NUM;
    }
}

void InfoPlot::plotArea() {
    QFont font("consolas", 10);
    vector<pair<long long, pair<double, double>>> data_area;
    calcArea(data_area);
    progress_bar->setValue(90);

    double max_value = 0;
    auto base_line = new QLineSeries();
    auto in_line = new QSplineSeries();
    auto in_out_line = new QSplineSeries();
    for (const auto &wi : data_area) {
        base_line->append(wi.first, 0);
        in_line->append(wi.first, wi.second.first);
        in_out_line->append(wi.first, wi.second.first + wi.second.second);
        max_value = std::max(max_value, wi.second.first + wi.second.second);
    }
    auto in_area = new QAreaSeries(base_line, in_line);
    auto out_area = new QAreaSeries(in_line, in_out_line);

    auto chart = new QChart();
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->hide();
    //    chart->setTitle("Number of Orders");
    chart->setFont(font);

    // x_axis
    auto x_axis = new QDateTimeAxis();
    x_axis->setTickCount(10);
    x_axis->setFormat("hh:mm");
    x_axis->setRange(QDateTime(QDate(2016, 11, 1), QTime(0, 0)), QDateTime(QDate(2016, 11, 2), QTime(0, 0)));
    x_axis->setTitleText("time");
    x_axis->setGridLineVisible(true);
    x_axis->setLabelsAngle(-45);
    x_axis->setLabelsFont(font);
    x_axis->setLineVisible(true);
    chart->addAxis(x_axis, Qt::AlignBottom);

    // y_axis
    auto y_axis = new QValueAxis();
    y_axis->setLabelFormat("%i");
    y_axis->setTitleText("orders number (in/out)");
    y_axis->setRange(0, max_value);
    y_axis->setLabelsFont(font);
    chart->addAxis(y_axis, Qt::AlignLeft);

    chart->addSeries(in_area);
    chart->addSeries(out_area);
    // can only attach after add
    //    in_area->attachAxis(x_axis);
    //    out_area->attachAxis(x_axis);
    in_area->attachAxis(y_axis);
    out_area->attachAxis(y_axis);

    plot_area->setChart(chart);
    progress_bar->setValue(100);
}
