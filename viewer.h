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
    QString text;
    void display(int);
};

#endif // VIEWER_H
