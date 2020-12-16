#include "gridselector.h"

GridSelector::GridSelector(bool empty) {
    this->setFixedSize(WIDTH, HEIGHT);
    setWindowTitle("Select grid");
    init_empty = empty;
    setupLayouts();
    setupConnects();
}

void GridSelector::paintEvent(QPaintEvent *event) {
    QPainter pen;
    pen.begin(this);
    pen.drawPixmap(EDGE_SIZE, EDGE_SIZE, WIDTH - 2 * EDGE_SIZE, WIDTH - 2 * EDGE_SIZE,
                   std::move(QPixmap("../background.png")));
    pen.end();
}

void GridSelector::setupLayouts() {
    QFont font("consolas", 10);
    auto outer_layer = new QVBoxLayout();

    button_group.resize(GRID_NUM);
    for (auto &button : button_group) {
        button = new QCheckBox();
        button->setCheckState(init_empty ? Qt::Unchecked : Qt::Checked);
        button->setMinimumSize(80, 80);
    }
    if (!init_empty)
        for (int i = 0; i < GRID_NUM; ++i)
            grid_id.push_back(i);

    // grid selection begin
    auto select_part = new QVBoxLayout();
    for (int i = 0; i < GRID_PER; ++i) // row
    {
        auto button_line = new QHBoxLayout();
        for (int j = 0; j < GRID_PER; ++j) // column
            button_line->addWidget(button_group[i * GRID_PER + j]);
        select_part->addLayout(button_line);
    }
    outer_layer->addLayout(select_part);
    // grid selection end

    // apply button
    auto button_part = new QHBoxLayout();
    auto spacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Preferred);
    button_part->addItem(spacer);
    apply_button = new QPushButton();
    apply_button->setText("Apply");
    apply_button->setFont(font);
    button_part->addWidget(apply_button);
    outer_layer->addLayout(button_part);

    setLayout(outer_layer);
}
void GridSelector::setupConnects() {
    // apply -> accepted
    connect(apply_button, &QPushButton::clicked, this, &GridSelector::accept);
}

void GridSelector::selectGrid() {
    if (this->exec() == GridSelector::Accepted) {
        grid_id.clear();
        for (int i = 0; i < GRID_NUM; ++i)
            if (button_group[i]->checkState() == Qt::Checked) grid_id.push_back(i);
    }
}
