#include "timeplot.h"

TimePlot::TimePlot(const int w, const int h, DataBase *_db, QThread *_thread)
    : xxxPlot(_db, _thread)
    , WIDTH(w)
    , HEIGHT(h) {
    selector = new GridSelector();
}

void TimePlot::setupLayouts() {
    QFont font("consolas", 10);
    main_widget = new QWidget();
    auto outer_layout = new QVBoxLayout(main_widget);

    // selection part begin
    auto selection_part = new QVBoxLayout();

    // top begin
    auto top_layout = new QHBoxLayout();
    auto spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);

    //  place select
    place_button = new QPushButton();
    place_button->setText("Select grids");
    place_button->setFont(font);
    place_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(place_button);
    top_layout->addItem(spacer);

    //  date select
    auto date_label = new QLabel();
    date_label->setText("Date");
    date_label->setFont(font);
    date_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    //    date_label->setMinimumWidth(
    top_layout->addWidget(date_label);
    date_combo = new QComboBox();
    date_combo->setFont(font);
    for (auto i = 1; i <= DAY_NUM; ++i)
        date_combo->addItem("11-" + QString::number(i));
    top_layout->addWidget(date_combo);
    top_layout->addItem(spacer);

    //  start time select
    auto start_time_label = new QLabel();
    start_time_label->setText("Start Time");
    start_time_label->setFont(font);
    start_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(start_time_label);
    start_combo = new QComboBox();
    start_combo->setFont(font);
    for (auto i = 0; i < 23; ++i)
        start_combo->addItem(QString::number(i));
    top_layout->addWidget(start_combo);
    top_layout->addItem(spacer);

    //  end time select
    auto end_time_label = new QLabel();
    end_time_label->setText("End Time");
    end_time_label->setFont(font);
    end_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(end_time_label);
    end_combo = new QComboBox();
    end_combo->setFont(font);
    for (auto i = 1; i <= 24; ++i)
        end_combo->addItem(QString::number(i));
    top_layout->addWidget(end_combo);
    top_layout->addItem(spacer);

    selection_part->addLayout(top_layout);
    // top end

    //  mid begin
    auto mid_layout = new QHBoxLayout();

    // step select
    auto step_label = new QLabel();
    step_label->setText("Step(min)");
    step_label->setFont(font);
    step_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(step_label);
    step_combo = new QComboBox();
    step_combo->setFont(font);
    step_combo->addItem(QString::number(10));
    for (auto i = 30; i <= 180; i += 30)
        step_combo->addItem(QString::number(i));
    mid_layout->addWidget(step_combo);
    mid_layout->addItem(spacer);

    //  type select
    auto type_label = new QLabel();
    type_label->setText("Plot Type");
    type_label->setFont(font);
    type_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(type_label);
    type_combo = new QComboBox();
    type_combo->setFont(font);
    type_combo->addItem("Line Chart");
    type_combo->addItem("Pie Chart");
    mid_layout->addWidget(type_combo);
    mid_layout->addItem(spacer);

    //  progress bar
    progress_bar = new QProgressBar();
    progress_bar->setRange(0, 100);
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 4);
    mid_layout->addWidget(progress_bar);
    auto min_spacer = new QSpacerItem(40, 20);
    mid_layout->addSpacerItem(min_spacer);

    //  plot button
    plot_button = new QPushButton();
    plot_button->setText("Plot!");
    plot_button->setFont(font);
    plot_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    mid_layout->addWidget(plot_button);

    selection_part->addLayout(mid_layout);
    // mid end

    outer_layout->addLayout(selection_part);
    // selection part end

    // plot area begin
    //    line = new QLineSeries();
    //    chart = new QChart();
    //    chart->legend()->hide();
    //    chart->createDefaultAxes();
    plot_area = new QChartView();
    plot_area->setRenderHint(QPainter::Antialiasing);
    plot_area->setMinimumSize(WIDTH, HEIGHT / 4 * 3);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    outer_layout->addWidget(plot_area);
    // plot area end
}

void TimePlot::setupConnects() {
    // button -> plotMap
    connect(plot_button, &QPushButton::clicked, this, &TimePlot::plotMap);
    connect(place_button, &QPushButton::clicked, selector, &GridSelector::selectGrid);
}

void TimePlot::plotMap() {
    // get settings
    grid_id = selector->grid_id;
    QRegExp expr("11-(\\d+)");
    expr.indexIn(date_combo->currentText());
    day = expr.cap(1).toInt();
    start_hour = start_combo->currentText().toInt();
    end_hour = end_combo->currentText().toInt();
    step_min = step_combo->currentText().toInt();
    progress_bar->setValue(33);

    // using another thread to plot
    if (type_combo->currentText() == "Line Chart") {
        //        auto painter = new myThread();
        //        connect(painter, &myThread::funcStart, this, &TimePlot::plotLineMap);
        //        this->moveToThread(painter);
        //        db->moveToThread(painter);
        //        painter->start();
        plotSeriesMap();
    } else {
        //        auto painter = new myThread();
        //        connect(painter, &myThread::funcStart, this, &TimePlot::plotPieMap);
        //        this->moveToThread(painter);
        //        db->moveToThread(painter);
        //        painter->start();
        plotPieMap();
    }
}

void TimePlot::calcSeries(vector<pair<int, int>> &data_series) {
    // int -> 2 byte fix length string
    auto num2str = [](const int &x) -> QString { return (x < 10 ? "0" : "") + QString::number(x); };
    for (auto i = start_hour * 60; i + step_min <= end_hour * 60; i += step_min) {
        // transform time to Unix time stamp
        //  hh:mm:ss
        QString time_day = num2str(i / 60) + ":" + num2str(i % 60) + ":00";
        QString time_str = "2016-11-" + num2str(day) + " " + time_day;
        QDateTime time_now = QLocale(QLocale::Chinese, QLocale::China).toDateTime(time_str, "yyyy-MM-dd hh:mm:ss");
        time_now.setTimeSpec(Qt::UTC);
        auto start_time = time_now.toTime_t();
        auto end_time = start_time + step_min * 60;
        int num = 0;
        for (auto &id : grid_id)
            num += db->startCount(id, start_time, end_time);
        data_series.push_back(make_pair(i, num));
    }
}

void TimePlot::plotSeriesMap() {
    //    auto series = new QLineSeries();
    auto series = new QSplineSeries();
    vector<pair<int, int>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(67);
    for (const auto &wi : data_series)
        series->append(wi.first, wi.second);
    auto chart = new QChart();
    chart->legend()->hide();
    chart->createDefaultAxes();
    //    chart->removeAllSeries();
    chart->addSeries(series);
    //    this->moveToThread(main_thread);
    //    db->moveToThread(main_thread);
    //    chart->moveToThread(main_thread);
    progress_bar->setValue(100);
    plot_area->setChart(chart);
    //    emit resetPlot(this);
}

void TimePlot::plotPieMap() {
    auto series = new QPieSeries();
    vector<pair<int, int>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(67);
    //    const double R = 0x66, G = 0xcc, B = 0xff;
    const double R = 0x00, G = 0x40, B = 0x80;
    const double threshold = 255;
    const double dR = threshold - R, dG = threshold - G, dB = threshold - B;
    int mx = 0;
    for (const auto &wi : data_series)
        mx = std::max(mx, wi.second);
    auto num2str = [](const int &x) -> QString { return (x < 10 ? "0" : "") + QString::number(x); };
    for (const auto &wi : data_series) {
        // hh:mm
        QString time_day = QString::number(wi.first / 60) + ":" + num2str(wi.first % 60);
        series->append(time_day, wi.second);
        double pi = 1 - wi.second * 1.0 / mx;
        QColor col(R + dR * pi, G + dG * pi, B + dB * pi);
        series->slices().back()->setColor(col);
    }
    // do not display tags when too many data
    const int visible_size = 24;
    if (data_series.size() < visible_size) series->setLabelsVisible();
    auto chart = new QChart();
    chart->legend()->hide();
    //    chart->createDefaultAxes();
    chart->addSeries(series);
    progress_bar->setValue(100);
    plot_area->setChart(chart);
    //    emit resetPlot(this);
}
