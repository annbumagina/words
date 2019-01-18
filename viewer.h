#ifndef VIEWER_H
#define VIEWER_H

#include <vector>
#include <QString>
#include <QPlainTextEdit>
#include <QSyntaxHighlighter>
#include <QRegularExpressionMatchIterator>
#include <QDebug>

class Viewer : public QPlainTextEdit
{
public:
    explicit Viewer(QWidget *parent);
    std::vector<long long> m;
    QString file;
    std::string word;
    long long pos;
    void display();
    void next();
    void prev();
    class Highlighter : public QSyntaxHighlighter {
    public:
        Highlighter(QTextDocument *, std::vector<long long> *);
        void highlightBlock(QString const&);
        QString word;
        std::vector<long long> *m;
    } highliter;
};

#endif // VIEWER_H
