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
    auto ch1 = new QTreeWidgetItem(info_filter);
    ch1->setText(0, "xxx");
    ch1->setFont(0, font);
    ch1->setCheckState(0, Qt::Unchecked);
    auto ch2 = new QTreeWidgetItem(info_filter);
    ch2->setText(0, "yyy");
    ch2->setFont(0, font);
    ch2->setCheckState(0, Qt::Unchecked);
    info_filter->setMinimumWidth(WIDTH / 4);
    info_filter->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    left_part->addWidget(info_filter);

    // progress bar
    auto progress_bar = new QProgressBar();
    progress_bar->setTextVisible(false);
    progress_bar->setMinimumWidth(WIDTH / 5);
    progress_bar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    left_part->addWidget(progress_bar);

    outer_layout->addLayout(left_part);
    // left part end

    // right part begin
    auto plot_area = new QChartView();
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    outer_layout->addWidget(plot_area);
}

void InfoPlot::plotMap() {}
