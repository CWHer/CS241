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
                Grid cur_grid;
                read_grid(line, cur_grid);
                grids.push_back(std::move(cur_grid));
                for (auto pos : {cur_grid.x1, cur_grid.x2, cur_grid.y1, cur_grid.y2}) {
                    lng.push_back(pos.first);
                    lat.push_back(pos.second);
                }
            }
        } else
            file_info[date].push_back(abs_path), ++total;
    }
    sort(lng.begin(), lng.end());
    lng.resize(std::unique(lng.begin(), lng.end()) - lng.begin());
    sort(lat.begin(), lat.end());
    lat.resize(std::unique(lat.begin(), lat.end()) - lat.begin());
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
                Tour cur_tour;
                read_daily(line, cur_tour);
                tours[i].push_back(std::move(cur_tour));
            }
            emit setValue(++cnt * 100.0 / total);
        }
    }
    initData();
    emit loadDone();
}

inline int DataBase::searchGridID(const Pos &pos) {
    int r = std::upper_bound(lng.begin(), lng.end(), pos.first) - lng.begin();
    int c = std::upper_bound(lat.begin(), lat.end(), pos.second) - lat.begin();
    return (c - 1) * GRID_PER + r - 1;
}

void DataBase::initData() {
    for (const auto &day : tours)
        for (auto &ti : day) {
            auto id = searchGridID(ti.src);
            // src may be not in grid areas!!!
            //  wtf...
            if (id < 0 || id >= GRID_NUM) continue;
            start_tour[id].push_back(ti);
            id = searchGridID(ti.dst);
            // dest may be not in grid areas!!!
            if (id < 0 || id >= GRID_NUM) continue;
            end_tour[id].push_back(ti);
        }
    for (auto &tours : start_tour)
        sort(tours.begin(), tours.end(), [](const Tour &lhs, const Tour &rhs) { return lhs.start < rhs.start; });
    for (auto &tours : end_tour)
        sort(tours.begin(), tours.end(), [](const Tour &lhs, const Tour &rhs) { return lhs.end < rhs.end; });
}

// [ql, qr)
int DataBase::startCount(int id, int ql, int qr) {
    Tour start_key, end_key;
    start_key.start = ql, end_key.start = qr;
    auto func = [](const Tour &lhs, const Tour &rhs) -> bool { return lhs.start < rhs.start; };
    int ls = lower_bound<Tour>(start_tour[id], func, start_key);
    int rs = lower_bound<Tour>(start_tour[id], func, end_key);
    return rs - ls;
}

int DataBase::endCount(int id, int ql, int qr) {
    Tour start_key, end_key;
    start_key.end = ql, end_key.end = qr;
    auto func = [](const Tour &lhs, const Tour &rhs) -> bool { return lhs.end < rhs.end; };
    int ls = lower_bound<Tour>(end_tour[id], func, start_key);
    int rs = lower_bound<Tour>(end_tour[id], func, end_key);
    return rs - ls;
}

// advanced version of std::lower_bound
//  vector + (*func) + key
//  func is something like '<'
//  return first w[x]>=key
template <class T> int DataBase::lower_bound(const vector<T> &w, bool (*func)(const T &, const T &), T key) {
    int l = 0, r = w.size() - 1;
    while (l < r) {
        auto mid = (l + r) >> 1;
        if (func(w[mid], key))
            l = mid + 1;
        else
            r = mid;
    }
    return r;
}
