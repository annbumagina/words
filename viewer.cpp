#include "viewer.h"

#include <QTextStream>
#include <QScrollBar>
#include <QDebug>
#include <regex>

Viewer::Viewer(QWidget *parent = 0) : QPlainTextEdit(parent), highliter(document(), &m)
{
    setReadOnly(true);
    zoomIn(3);
}

void Viewer::display() {
    std::sort(m.begin(), m.end());
    std::regex sp { R"([-[\]{}()*+?.,\^$|#\s\\])" };
    QString sanitized = QString::fromStdString(regex_replace(word, sp, R"(\$&)"));
    highliter.word = sanitized;

    m.clear();
    pos = 0;

    QFile f(file);
    f.open(QIODevice::ReadOnly);
    QTextStream st(&f);
    setPlainText(st.readAll());
    f.close();

    if (m.size() == 0) return;

    QTextCursor cursor(document());
    cursor.setPosition(m[0], QTextCursor::MoveAnchor);
    cursor.setPosition(m[0] + QString::fromStdString(word).size(), QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void Viewer::next() {
    if (m.size() == 0) return;

    pos = (pos + 1) % (m.size());
    QTextCursor cursor(document());
    cursor.setPosition(m[pos], QTextCursor::MoveAnchor);
    cursor.setPosition(m[pos] + QString::fromStdString(word).size(), QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void Viewer::prev() {
    if (m.size() == 0) return;

    pos = (pos - 1 + m.size()) % (m.size());
    QTextCursor cursor(document());
    cursor.setPosition(m[pos], QTextCursor::MoveAnchor);
    cursor.setPosition(m[pos] + QString::fromStdString(word).size(), QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

Viewer::Highlighter::Highlighter(QTextDocument *document, std::vector<long long> *m) : QSyntaxHighlighter(document), m(m) {}

void Viewer::Highlighter::highlightBlock(QString const& text) {
    //qDebug() << text;
    QTextCharFormat fmt;
    fmt.setBackground(Qt::green);
    QRegularExpression expression(word);
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    long long tb_pos = currentBlock().position();
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        setFormat(match.capturedStart(), match.capturedLength(), fmt);
        m->push_back(tb_pos + match.capturedStart());
    }
}
