#include "trigram.h"
#include <QDebug>
#include <QThread>
#include <QString>
#include <unordered_set>
#include <regex>

Trigram::Trigram()
{}

void Trigram::text_to_tris(char *bf, int len) {
    //qDebug() << QString(__func__) <<" from work thread: " << QThread::currentThreadId();
    unsigned mask = 0x00FFFFFF;
    unsigned x = ((unsigned(uchar(bf[0])) << 8) | unsigned(uchar(bf[1])));
    for (int i = 2; i < len; i++) {
        x = (((x << 8) & mask) | bf[i]);
        tris.insert(x);
    }
}

void Trigram::find_word(char *bf, int len, std::regex const& r, int WS, int SHIFT) {
    auto words_begin = std::regex_iterator<char *>(bf, bf + WS - 1 + len, r);
    auto words_end = std::regex_iterator<char *>();
    for (auto i = words_begin; i != words_end; ++i) {
       pos.push_back((*i).position(0) + SHIFT);
    }
}
