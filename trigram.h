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
    void find_word(char *, int, std::regex const&, int, int);
    std::unordered_set<unsigned> tris;
    std::vector<long long> pos;
    Trigram();
};

#endif // TRIGRAM_H
