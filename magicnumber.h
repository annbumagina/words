#ifndef MAGICNUMBER_H
#define MAGICNUMBER_H

#include <vector>
#include <QString>

class MagicNumber
{
public:
    MagicNumber();
    static std::vector< std::pair< QString, std::vector<char> > > magic_numbers;
    static bool check_type(char *, int);
};

#endif // MAGICNUMBER_H
