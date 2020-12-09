#include "timeplot.h"

TimePlot::TimePlot(const int w, const int h, QWidget *parent)
    : QWidget(parent)
    , WIDTH(w)
    , HEIGHT(h) {}

void TimePlot::setupLayouts() {
    QFont font = QFont("consolas", 10);
    main_widget = new QWidget();
    auto outer_layout = new QVBoxLayout(main_widget);

    // selection part begin
    auto selection_part = new QVBoxLayout();

    // top
    auto top_layout = new QHBoxLayout();
    auto button1 = new QPushButton();
    top_layout->addWidget(button1);
    auto button2 = new QPushButton();
    top_layout->addWidget(button2);

    selection_part->addLayout(top_layout);
    // mid
    auto mid_layout = new QHBoxLayout();
    auto button3 = new QPushButton();
    mid_layout->addWidget(button3);
    auto button4 = new QPushButton();
    mid_layout->addWidget(button4);

    selection_part->addLayout(mid_layout);
    outer_layout->addLayout(selection_part);
    // selection part end

    // plot area begin
    plot_area = new QChartView();
    plot_area->setMinimumSize(WIDTH, HEIGHT / 4 * 3);
    plot_area->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    outer_layout->addWidget(plot_area);
    // plot area end
}
