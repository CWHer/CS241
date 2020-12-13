#include "infoplot.h"

InfoPlot::InfoPlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {}

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

    info_filter->setMinimumWidth(WIDTH / 4);
    info_filter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addWidget(info_filter);
    // filter end

    // progress bar and apply button
    auto progress_part = new QHBoxLayout();

    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setValue(0);
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 5 / 4 * 3);
    progress_bar->setMinimumHeight(20);
    progress_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
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
    plot_area = new QChartView();
    plot_area->setRenderHint(QPainter::Antialiasing);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // right part end

    outer_layout->addWidget(plot_area);
}

void InfoPlot::setupConnects() {
    // button -> plotMap
    connect(apply_button, &QPushButton::clicked, this, &InfoPlot::plotMap);
}

void InfoPlot::plotMap() {
    // get settings
    QString plot_type;
    for (int i = 0; i < info_filter->topLevelItemCount(); ++i)
        if (info_filter->topLevelItem(i)->checkState(0) == Qt::Checked)
            plot_type = info_filter->topLevelItem(i)->text(0);
    if (plot_type.isEmpty()) return;
    info = plot_type.indexOf("time") != -1 ? TIME : FEE;
    interval = plot_type.indexOf("day") != -1 ? DAY : ALL;
    progress_bar->setValue(33);

    // using another thread
    plotSeries();
}

void InfoPlot::plotSeries() {
    auto series = new QSplineSeries();
    vector<pair<int, double>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(90);

    for (const auto &wi : data_series)
        series->append(wi.first, wi.second);
    auto chart = new QChart();
    chart->legend()->hide();
    chart->createDefaultAxes();
    chart->addSeries(series);
    progress_bar->setValue(100);
    plot_area->setChart(chart);
}

void InfoPlot::calcSeries(vector<pair<int, double>> &series) {
    const int step_min = 20;
    if (interval == DAY) {
        series.resize(24 * 60 / step_min);
        for (int i = 0; i < 24 * 60; i += step_min)
            series[i / step_min].first = i;
    }
    auto num2str = [](const int &x) -> QString { return (x < 10 ? "0" : "") + QString::number(x); };
    for (auto day = 1; day <= DAY_NUM; ++day) {
        for (auto i = 0; i + step_min < 24 * 60; i += step_min) {
            QString time_day = num2str(i / 60) + ":" + num2str(i % 60) + ":00";
            QString time_str = "2016-11-" + num2str(day) + " " + time_day;
            QDateTime time_now = QLocale(QLocale::Chinese, QLocale::China).toDateTime(time_str, "yyyy-MM-dd hh:mm:ss");
            time_now.setTimeSpec(Qt::UTC);
            auto start_time = time_now.toTime_t();
            auto end_time = start_time + step_min * 60;
            double num = 0;
            for (auto i = 0; i < GRID_NUM; ++i)
                num += info == FEE ? db->feeCount(i, start_time, end_time) : db->timeCount(i, start_time, end_time);
            if (interval == DAY)
                series[i / step_min].second += num;
            else
                series.push_back(make_pair(i + 24 * 60 * day, num));
        }
    }
    if (interval == DAY)
        for (auto &wi : series)
            wi.second /= DAY_NUM;
}
