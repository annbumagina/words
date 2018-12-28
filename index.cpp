#include "task.h"

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
#include <unordered_set>
#include <QFile>

Index::Index(Task *p, const QString &s) : file(s), parent(p)
{}

void Index::clear() {
    for (int i = 0; i < TH; i++) {
        t[i].tris.clear();
        t[i].pos.clear();
    }
}

void Index::index_file() {
    if (QFile(file).size() <= N) {
        index_small_file();
    } else {
        index_large_file();
    }
}

void Index::index_large_file() {
    //qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    std::ifstream in(file.toStdString().c_str());
    char bf[TH][N + 2];
    in.read(bf[0], 2);
    char a = bf[0][0], b = bf[0][1];
    while (!in.eof()) {
        if (parent->canceled) { clear(); return; }
        for (int i = 0; i < TH && !in.eof(); i++) {
            bf[i][0] = a;
            bf[i][1] = b;
            in.read(bf[i] + 2, N);
            a = bf[i][N];
            b = bf[i][N + 1];
            future[i] = QtConcurrent::run(&t[i], &Trigram::text_to_tris, bf[i] + 0, in.gcount() + 2);
        }
        for (int i = 0; i < TH; i++) {
            future[i].waitForFinished();
        }
    }
    long long sum = 0;
    long long pos[TH];
    for (int i = 0; i < TH; i++) {
        pos[i] = sum;
        sum += t[i].tris.size();
    }
    trigrams.resize(sum);
    for (int i = 0; i < TH; i++) {
        std::copy(t[i].tris.begin(), t[i].tris.end(), trigrams.begin() + pos[i]);
    }
    clear();
    std::sort(trigrams.begin(), trigrams.end());
    trigrams.resize(std::unique(trigrams.begin(), trigrams.end()) - trigrams.begin());
    //qDebug() << QString::number(trigrams.size());
}

void Index::index_small_file() {
    std::ifstream in(file.toStdString().c_str());
    char bf[N];
    unsigned mask = 0x00FFFFFF;
    std::unordered_set<unsigned> tr;

    in.read(bf, 2);
    unsigned x = (unsigned(bf[0]) << 8) | unsigned(bf[1]);
    while (!in.eof()) {
        if (parent->canceled) { return; }
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
    //qDebug() << QString ::number(trigrams.size());
}

std::vector<long long> Index::search(int WS, std::regex const& r, std::vector<unsigned> const& tr) {
    //qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    //qDebug() << QString::fromStdString(word);
    bool good = true;
    for (auto x: tr) {
        if (std::lower_bound(trigrams.begin(), trigrams.end(), x) == trigrams.end()) {
            good = false;
            break;
        }
    }
    if (!good)
        return {};

    std::vector<long long> pos;
    if (QFile(file).size() <= N) {
        search_small(WS, r, pos);
    } else {
        search_large(WS, r, pos);
    }
    //qDebug() << QString::number(cnt);
    return pos;
}

void Index::search_small(int WS, std::regex const& r, std::vector<long long> &pos) {
    std::ifstream in(file.toStdString().c_str());
    char bf[N + WS - 1];
    long long SHIFT = 0;

    in.read(bf, WS - 1);
    while (!in.eof()) {
        if (parent->canceled) { return; }
        in.read(bf + WS - 1, N);
        int len = in.gcount();

        auto words_begin = std::regex_iterator<char *>(bf, bf + WS - 1 + len, r);
        auto words_end = std::regex_iterator<char *>();
        for (auto i = words_begin; i != words_end; ++i) {
           pos.push_back((*i).position(0) + SHIFT);
        }
        SHIFT += N;
        std::copy(bf + len, bf + WS - 1 + len, bf);
    }
}

void Index::search_large(int WS, std::regex const& r, std::vector<long long> &pos) {
    std::ifstream in(file.toStdString().c_str());
    char bf[TH][N + WS - 1];
    char wbf[WS - 1];
    long long SHIFT = 0;
    int len;

    in.read(wbf, WS - 1);
    while (!in.eof()) {
        if (parent->canceled) { clear(); return; }
        for (int i = 0; i < TH; i++) {
            std::copy(wbf, wbf + WS - 1, bf[i]);
            in.read(bf[i] + WS - 1, N);
            len = in.gcount();
            std::copy(bf[i] + len, bf[i] + len + WS - 1, wbf);
            future[i] = QtConcurrent::run(&t[i], &Trigram::find_word, bf[i] + 0, len + WS - 1, r, WS, SHIFT);
            SHIFT += N;
        }
        for (int i = 0; i < TH; i++) {
            future[i].waitForFinished();
        }
    }
    long long sum = 0;
    long long pos_[TH];
    for (int i = 0; i < TH; i++) {
        pos_[i] = sum;
        sum += t[i].pos.size();
    }
    pos.resize(sum);
    for (int i = 0; i < TH; i++) {
        std::copy(t[i].pos.begin(), t[i].pos.end(), pos.begin() + pos_[i]);
    }
    clear();
    //qDebug() << QString::number(count);
}


