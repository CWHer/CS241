#include "database.h"

// folder_path -> file_list
void DataBase::parseFolder() {
    QDir dir(folder_path);
    if (!dir.exists()) return;
    QStringList filter("*.csv");
    file_list = dir.entryInfoList(filter);
    for (const auto &file : file_list) {
        QRegExp expr("201611(\\d+)");
        QString abs_path = file.absoluteFilePath();
        // tell 1-15 day or rectangle
        int date = expr.indexIn(abs_path) == -1 ? -1 : expr.cap(1).toInt() - 1;
        if (date == -1) {
            // load coordinate data
            QFile rect_data(abs_path);
            if (!rect_data.open(QFile::ReadOnly)) {
                QErrorMessage *error = new QErrorMessage();
                error->showMessage("error file" + abs_path);
                return;
            }
            QTextStream fin(&rect_data);
            QString line;
            fin >> line;
            while (!fin.atEnd()) {
                fin >> line;
                if (line.isEmpty()) continue;
                grid cur_grid;
                read_grid(line, cur_grid);
                grids.push_back(std::move(cur_grid));
            }
        } else
            file_info[date].push_back(abs_path), ++total;
    }
}

// load one day data
void DataBase::loadData() {
    for (auto i = 0; i < DAY_NUM; ++i) {
        for (const auto &abs_path : file_info[i]) {
            QFile file(abs_path);
            if (!file.open(QFile::ReadOnly)) {
                QErrorMessage *error = new QErrorMessage();
                error->showMessage("error file" + abs_path);
                return;
            }
            QTextStream fin(&file);
            QString line;
            fin >> line;
            while (!fin.atEnd()) {
                fin >> line;
                if (line.isEmpty()) continue;
                tour cur_tour;
                read_daily(line, cur_tour);
                tours[i].push_back(std::move(cur_tour));
            }
            emit setValue(++cnt * 100.0 / total);
        }
    }
    emit loadDone();
}
