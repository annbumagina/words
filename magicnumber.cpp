#include "magicnumber.h"

#include <cstring>
#include <QDebug>

MagicNumber::MagicNumber()
{}

std::vector< std::pair< QString, std::vector<char> > > MagicNumber::magic_numbers = {
    { "elf",            { '\x7F', '\x45', '\x4C', '\x46' }},
    { "png",            { '\x89', '\x50', '\x4E', '\x47', '\x0D', '\x0A', '\x1A', '\x0A' }},
    { "gif_1",          { '\x47', '\x49', '\x46', '\x38', '\x37', '\x61' }},
    { "gif_2",          { '\x47', '\x49', '\x46', '\x38', '\x39', '\x61' }},
    { "jpg_3",          { '\xFF', '\xD8', '\xFF', '\xE0', '\x00', '\x10', '\x4A', '\x46', '\x49', '\x46', '\x00', '\x01' }},
    { "mkv",            { '\x1A', '\x45', '\xDF', '\xA3' }},
    { "mp3_idtv2",      { '\x49', '\x44', '\x33' }},
    { "zip, jar ...1",  { '\x50', '\x4B', '\x03', '\x04' }},
    { "zip, jar ...2",  { '\x50', '\x4B', '\x05', '\x06' }},
    { "zip, jar ...3",  { '\x50', '\x4B', '\x07', '\x08' }},
    { "lz",             { '\x4C', '\x5A', '\x49', '\x50' }},
    { "rar_1",          { '\x52', '\x61', '\x72', '\x21', '\x1A', '\x07', '\x00' }},
    { "rar_2",          { '\x52', '\x61', '\x72', '\x21', '\x1A', '\x07', '\x01', '\x00' }},
    { "pdf",            { '\x25', '\x50', '\x44', '\x46', '\x2d' }},
    { "psd",            { '\x38', '\x42', '\x50', '\x53' }},
    { "doc, xls ...",   { '\xD0', '\xCF', '\x11', '\xE0', '\xA1', '\xB1', '\x1A', '\xE1' }},
    { "7z",             { '\x37', '\x7A', '\xBC', '\xAF', '\x27', '\x1C' }},
    { "rpm",            { '\xED', '\xAB', '\xEE', '\xDB' }},
    { "class",          { '\xCA', '\xFE', '\xBA', '\xBE' }},
    { "flac",           { '\x66', '\x4C', '\x61', '\x43' }},
    { "tar_lzw",        { '\x1F', '\x9D' }},
    { "tar_lzh",        { '\x1F', '\xA0' }},
    { "tar_lzip",       { '\x4C', '\x5A', '\x49', '\x50' }},
    { "gzip",           { '\x1F', '\x8B' }},
    { "maco_32",        { '\xFE', '\xED', '\xFA', '\xCE' }},
    { "maco_64",        { '\xFE', '\xED', '\xFA', '\xCF' }},
    { "bmp",            { '\x42', '\x4D' }},
    { "bz2",            { '\x42', '\x5A', '\x68' }},
    { "exe",            { '\x4D', '\x5A' }},
    { "jpg_1",          { '\xFF', '\xD8', '\xFF', '\xDB' }},
    { "jpg_2",          { '\xFF', '\xD8', '\xFF', '\xEE' }}
};

bool MagicNumber::check_type(char *bf, int len) {
    for (auto &mn : magic_numbers) {
        if (std::memcmp(bf, mn.second.data(), std::min(len, int(mn.second.size()))) == 0) {
            //qDebug() << mn.first;
            return false;
        }
    }
    return true;
}
