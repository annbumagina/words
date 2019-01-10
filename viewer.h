#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <QString>
#include <QPlainTextEdit>

class Viewer : public QPlainTextEdit
{
public:
    explicit Viewer(QWidget *parent);
    std::vector< std::pair<QString, std::vector<long long> > > m;
    std::string word;
    long long pos;
    int id;
    void display(int);
    void next();
    void prev();
};

#endif // VIEWER_H
