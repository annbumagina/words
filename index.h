#ifndef INDEX_H
#define INDEX_H

#include "trigram.h"
#include <QObject>
#include <set>
#include <QFuture>

const int TH = 1;

class Index
{
public:
    Index(const QString &s);
    QString file;
    std::unordered_set<unsigned> trigrams;

//signals:

//public slots:
    void index_file();

//private:
};

#endif // INDEX_H
