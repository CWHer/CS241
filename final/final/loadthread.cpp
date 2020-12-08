#include "loadthread.h"

void LoadThread::init(QString path, DataBase *_db) { db = _db, folder_path = path; }

void LoadThread::run() {
    QDir dir(folder_path);
    if (!dir.exists()) return;
    QStringList filter("*csv");
    QFileInfoList file_list = dir.entryInfoList(filter);
    // parse all file
    for (auto i = 0; i < file_list.size(); ++i) {
        // 201611xx
        QRegExp r("201611(\\d+)");
        QString file_path = file_list[i].absoluteFilePath();
        // 1-15 date or rect
        int date = r.indexIn(file_path) == -1 ? -1 : r.cap(1).toInt() - 1;
        QFile file(file_path);
        if (!file.open(QFile::ReadOnly)) {
            QErrorMessage *error = new QErrorMessage();
            error->showMessage("error line");
            return;
        }
        QTextStream fin(&file);
        QString line;
        //        a wrong version
        //        line = fin.readLine();
        fin >> line;
        while (!fin.atEnd()) {
            fin >> line;
            if (line.isEmpty()) continue;
            if (date != -1) {
                tour cur;
                read_daily(line, cur);
                db->tours[date].push_back(std::move(cur));
            } else {
                grid cur;
                read_grid(line, cur);
                db->grids.push_back(std::move(cur));
            }
        }
        emit setValue((i + 1) * 100.0 / file_list.size());
    }
    emit switchwindow();
}
