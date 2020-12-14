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
    QDateTime default_date(QDate(2016, 11, 1), QTime(0, 0));

    // selection part begin
    auto selection_part = new QVBoxLayout();

    // top begin
    auto top_layout = new QHBoxLayout();

    //  place select
    place_button = new QPushButton();
    place_button->setText("Select grids");
    place_button->setFont(font);
    place_button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(place_button);
    top_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

    //  start time select
    auto start_time_label = new QLabel();
    start_time_label->setText("Start Time");
    start_time_label->setFont(font);
    start_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(start_time_label);
    start_edit = new QDateTimeEdit();
    start_edit->setFont(font);
    start_edit->setDateTime(default_date);
    top_layout->addWidget(start_edit);
    top_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

    //  end time select
    auto end_time_label = new QLabel();
    end_time_label->setText("End Time");
    end_time_label->setFont(font);
    end_time_label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    top_layout->addWidget(end_time_label);
    end_edit = new QDateTimeEdit();
    end_edit->setFont(font);
    end_edit->setDateTime(default_date);
    top_layout->addWidget(end_edit);
    top_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

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
    mid_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

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
    mid_layout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred));

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
    plot_area = new QChartView();
    plot_area->setRubberBand(QChartView::HorizontalRubberBand);
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
    progress_bar->setValue(0);
    // get settings
    grid_id = selector->grid_id;
    start_datetime = start_edit->dateTime();
    end_datetime = end_edit->dateTime();
    step_min = step_combo->currentText().toInt();
    progress_bar->setValue(33);

    // tend to use another thread to plot
    //  however fast enough to only use main
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

void TimePlot::calcSeries(vector<pair<long long, int>> &data_series) {
    for (auto i = start_datetime.toSecsSinceEpoch(); i + step_min <= end_datetime.toSecsSinceEpoch();
         i += step_min * 60) {
        int num = 0;
        for (auto &id : grid_id)
            num += db->startCount(id, i, i + step_min * 60);
        data_series.push_back(make_pair(i * 1000, num));
    }
}

void TimePlot::plotSeriesMap() {
    QFont font("consolas", 10);
    //    auto series = new QLineSeries();
    auto series = new QSplineSeries();
    int max_value = 0;
    vector<pair<long long, int>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(67);
    for (const auto &wi : data_series) {
        series->append(wi.first, wi.second);
        max_value = std::max(max_value, wi.second);
    }

    auto chart = new QChart();
    chart->setAnimationDuration(QChart::SeriesAnimations);
    chart->legend()->hide();
    //    chart->setTitle("Number of Orders");
    chart->setFont(font);

    // x_axis
    auto x_axis = new QDateTimeAxis();
    x_axis->setTickCount(10);
    x_axis->setFormat("dd h:mm ");
    x_axis->setTitleText("time");
    x_axis->setGridLineVisible(true);
    x_axis->setLabelsAngle(-45);
    x_axis->setLabelsFont(font);
    x_axis->setLineVisible(true);
    x_axis->setRange(start_datetime, end_datetime);
    chart->addAxis(x_axis, Qt::AlignBottom);
    series->attachAxis(x_axis);

    // y_axis
    auto y_axis = new QValueAxis();
    y_axis->setLabelFormat("%i");
    y_axis->setTitleText("order number");
    y_axis->setRange(0, max_value);
    y_axis->setLabelsFont(font);
    chart->addAxis(y_axis, Qt::AlignLeft);

    chart->addSeries(series);
    plot_area->setChart(chart);

    //    this->moveToThread(main_thread);
    //    db->moveToThread(main_thread);
    //    chart->moveToThread(main_thread);
    progress_bar->setValue(100);
    //    emit resetPlot(this);
}

void TimePlot::plotPieMap() {
    QFont font("consolas", 10);
    auto series = new QPieSeries();
    vector<pair<long long, int>> data_series;
    calcSeries(data_series);
    progress_bar->setValue(67);
    //    const double R = 0x66, G = 0xcc, B = 0xff;
    const double R = 0x00, G = 0x40, B = 0x80;
    const double threshold = 255;
    const double dR = threshold - R, dG = threshold - G, dB = threshold - B;
    int mx = 0;
    for (const auto &wi : data_series)
        mx = std::max(mx, wi.second);
    for (const auto &wi : data_series) {
        QDateTime date;
        date.setMSecsSinceEpoch(wi.first);
        QString time_str = date.toString("dd hh:mm");
        series->append(time_str, wi.second);
        double pi = 1 - wi.second * 1.0 / mx;
        QColor col(R + dR * pi, G + dG * pi, B + dB * pi);
        series->slices().back()->setColor(col);
    }
    // do not display tags when too many data
    const int visible_size = 24;
    if (data_series.size() < visible_size) series->setLabelsVisible();

    auto chart = new QChart();
    chart->setAnimationDuration(QChart::SeriesAnimations);
    chart->legend()->hide();
    //    chart->setTitle("Number of Orders");
    chart->setFont(font);
    chart->addSeries(series);
    plot_area->setChart(chart);

    progress_bar->setValue(100);
    //    emit resetPlot(this);
}
