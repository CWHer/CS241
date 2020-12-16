#ifndef GRIDSELECTOR_H
#define GRIDSELECTOR_H

#include "common.h"

class GridSelector : public QDialog {
    Q_OBJECT
  public:
    GridSelector(bool empty = false);
    // store grid chosen
    vector<int> grid_id;
    void setupLayouts();
    void setupConnects();
    void selectGrid();
    void paintEvent(QPaintEvent *) override;

  private:
    vector<QCheckBox *> button_group;
    QPushButton *apply_button;
    static const int WIDTH = 800;
    static const int HEIGHT = 845;
    static const int EDGE_SIZE = 10;
    bool init_empty;
};

#endif // GRIDSELECTOR_H
