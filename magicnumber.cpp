#include "magicnumber.h"

MagicNumber::MagicNumber()
{}

std::vector< std::pair< QString, std::vector<char> > > MagicNumber::magic_numbers = {
   {"elf",     {'\x7F', '\x45', '\x4C', '\x46'}},
   {"png",     {'\x89', '\x50', '\x4E', '\x47', '\x0D', '\x0A', '\x1A', '\x0A'}},
   {"class",   {'\xCA', '\xFE', '\xBA', '\xBE'}}
};

bool MagicNumber::check_type(char *bf, int len) {
    //std::
    return true;
}
