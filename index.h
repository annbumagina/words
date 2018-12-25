#ifndef INDEX_H
#define INDEX_H

#include "trigram.h"
#include <QObject>
#include <set>
#include <QFuture>
#include <atomic>

class Index
{
public:
    Index(QString const&);
    QString file;
    std::vector<unsigned> trigrams;
    bool canceled;

//signals:

//public slots:
    void index_file();
    long long search(std::string, std::vector<unsigned> const&);

//private:
};

#endif // INDEX_H
