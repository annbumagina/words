#ifndef INDEX_H
#define INDEX_H

#include "task.h"
#include "trigram.h"
#include <QObject>
#include <set>
#include <QFuture>
#include <atomic>

const int TH = 6;
const int N = 1024 * 32;
const int MAX_TRIS_COUNT = 1e5;

class Index
{
public:
    Index(Task *, QString const&);
    QString file;

    bool index_file();
    std::vector<long long> search(int, std::regex const&, std::vector<unsigned> const&);

private:
    bool index_small_file();
    bool index_large_file();
    void search_large(int, std::regex const&, std::vector<long long> &);
    void search_small(int, std::regex const&, std::vector<long long> &);
    void clear();

    Task *parent;
    std::vector<unsigned> trigrams;
    Trigram t[TH];
    QFuture<void> future[TH];
};

#endif // INDEX_H
