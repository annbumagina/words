#ifndef TRIGRAM_H
#define TRIGRAM_H

#include <set>
#include <QString>
#include <vector>
#include <unordered_set>

class Trigram
{
public:
    void text_to_tris();
    char *bf;
    int len;
    std::unordered_set<unsigned> tris;
    //int id;
    QString file;
    Trigram();
};

#endif // TRIGRAM_H
