#include "thermalmap.h"

ThermalMap::ThermalMap(const int &w, const int &h) {
    //
    this->resize(w, h);
}

void ThermalMap::paintEvent(QPaintEvent *event) {
    QPainter pen;
    pen.begin(this);
    pen.drawPixmap(width() - height() + EDGE_SIZE, EDGE_SIZE, height() - EDGE_SIZE * 2, height() - EDGE_SIZE * 2,
                   std::move(QPixmap("../background.png")));
    pen.end();
}
