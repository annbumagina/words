#include "index.h"

#include <set>
#include <QThread>
#include <QDebug>
#include <fstream>
#include <QtConcurrent/QtConcurrentMap>
#include <QtConcurrent/QtConcurrentRun>
#include <QElapsedTimer>
#include <future>
#include <regex>
#include <iterator>

Index::Index(const QString &s) : file(s), canceled(false)
{}

void Index::index_file() {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    std::ifstream in(file.toStdString().c_str());
    const int N = 1024 * 32;
    char bf[N];
    unsigned mask = 0x00FFFFFF;
    std::unordered_set<unsigned> tr;

    in.read(bf, 2);
    unsigned x = (unsigned(bf[0]) << 8) | unsigned(bf[1]);
    while (!in.eof()) {
        if (canceled) return;
        in.read(bf, N);
        int len = in.gcount();
        for (int i = 0; i < len; i++) {
            x = (((x << 8) & mask) | bf[i]);
            tr.insert(x);
            //qDebug() << QString::number(x);
        }
    }
    
    trigrams.reserve(tr.size());
    for (auto i: tr) {
        trigrams.push_back(i);
    }
    std::sort(trigrams.begin(), trigrams.end());
}
long long Index::search(std::string word, std::vector<unsigned> const& tr) {
    qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    qDebug() << QString::fromStdString(word);
    bool good = true;
    for (auto x: tr) {
        if (std::lower_bound(trigrams.begin(), trigrams.end(), x) == trigrams.end()) {
            good = false;
            break;
        }
    }
    if (!good)
        return 0;

    std::regex r(word);
    std::ifstream in(file.toStdString().c_str());
    const int N = 1024 * 32;
    int WS = word.size();
    long long count = 0;
    char bf[N + WS - 1];

    in.read(bf, WS - 1);
    while (!in.eof()) {
        if (canceled) return 0;
        in.read(bf + WS - 1, N);
        int len = in.gcount();

        std::regex r(word);
        int match_count = std::distance(std::regex_iterator<char *>(bf, bf + WS - 1 + len, r), std::regex_iterator<char *>());
        count += match_count;
        std::copy(bf + len, bf + WS - 1 + len, bf);
        //count += m.size() - 1;
    }
    qDebug() << QString::number(count);
    return count;
}
