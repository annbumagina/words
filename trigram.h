#ifndef TRIGRAM_H
#define TRIGRAM_H

#include <set>
#include <QString>
#include <vector>
#include <unordered_set>
#include <regex>

class Trigram
{
public:
    void text_to_tris(char *, int);
    void find_word(char *, int, std::regex const&, int);
    std::unordered_set<unsigned> tris;
    long long cnt = 0;
    Trigram();
};

#endif // TRIGRAM_H
