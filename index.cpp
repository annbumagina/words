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
        t[i].cnt = 0;
    }
}

bool Index::index_file() {
    if (QFile(file).size() <= N) {
        return index_small_file();
    } else {
        return index_large_file();
    }
    //qDebug() << file << ' ' << QString::number(trigrams.size());
}

bool Index::index_large_file() {
    //qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    std::ifstream in(file.toStdString().c_str());
    char bf[TH][N + 2];
    in.read(bf[0], 2);
    char a = bf[0][0], b = bf[0][1];
    while (!in.eof()) {
        if (parent->canceled) { clear(); return 0; }
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
        for (int i = 0; i < TH; i++) {
            if (t[i].tris.size() > MAX_TRIS_COUNT) {
                clear();
                return 0;
            }
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
    return 1;
}

bool Index::index_small_file() {
    std::ifstream in(file.toStdString().c_str());
    char bf[N];
    unsigned mask = 0x00FFFFFF;
    std::unordered_set<unsigned> tr;

    in.read(bf, 2);
    unsigned x = (unsigned(uchar(bf[0])) << 8) | unsigned(uchar(bf[1]));
    while (!in.eof()) {
        if (parent->canceled) { return 0; }
        in.read(bf, N);
        int len = in.gcount();
        for (int i = 0; i < len; i++) {
            x = (((x << 8) & mask) | uchar(bf[i]));
            tr.insert(x);
            //qDebug() << QString::number(x);
        }
    }

    trigrams.resize(tr.size());
    std::copy(tr.begin(), tr.end(), trigrams.begin());
    std::sort(trigrams.begin(), trigrams.end());
    //qDebug() << QString ::number(trigrams.size());
    return 1;
}

long long Index::search(int WS, std::regex const& r, std::vector<unsigned> const& tr) {
    //qDebug() << QString(__func__) << " from work thread: " << QThread::currentThreadId();
    //qDebug() << QString::fromStdString(word);
    if (tr.size() > 0 && trigrams.size() > 0) {
        bool good = true;
        for (auto x: tr) {
            if (std::lower_bound(trigrams.begin(), trigrams.end(), x) == trigrams.end()) {
                good = false;
                break;
            }
        }
        if (!good)
            return {};
    }

    if (QFile(file).size() <= N) {
        return search_small(WS, r);
    } else {
        return search_large(WS, r);
    }
}

long long Index::search_small(int WS, std::regex const& r) {
    std::ifstream in(file.toStdString().c_str());
    char bf[N + WS - 1];
    long long cnt = 0;

    in.read(bf, WS - 1);
    while (!in.eof()) {
        if (parent->canceled) { return 0; }
        in.read(bf + WS - 1, N);
        int len = in.gcount();

        auto words_begin = std::regex_iterator<char *>(bf, bf + WS - 1 + len, r);
        auto words_end = std::regex_iterator<char *>();
        cnt += std::distance(words_begin, words_end);
        std::copy(bf + len, bf + WS - 1 + len, bf);
    }
    //qDebug() << QString::number(cnt);
    return cnt;
}

long long Index::search_large(int WS, std::regex const& r) {
    std::ifstream in(file.toStdString().c_str());
    char bf[TH][N + WS - 1];
    char wbf[WS - 1];
    int len;

    in.read(wbf, WS - 1);
    while (!in.eof()) {
        if (parent->canceled) { clear(); return 0; }
        for (int i = 0; i < TH; i++) {
            std::copy(wbf, wbf + WS - 1, bf[i]);
            in.read(bf[i] + WS - 1, N);
            len = in.gcount();
            std::copy(bf[i] + len, bf[i] + len + WS - 1, wbf);
            future[i] = QtConcurrent::run(&t[i], &Trigram::find_word, bf[i] + 0, len + WS - 1, r, WS);
        }
        for (int i = 0; i < TH; i++) {
            future[i].waitForFinished();
        }
    }
    long long cnt = 0;
    for (int i = 0; i < TH; i++) {
        cnt += t[i].cnt;
    }
    //qDebug() << QString::number(cnt);
    clear();
    return cnt;
}


