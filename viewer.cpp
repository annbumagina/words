#include "viewer.h"

#include <QTextStream>
#include <QScrollBar>
#include <QDebug>

Viewer::Viewer(QWidget *parent = 0) : QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void Viewer::display(int id) {
    moveCursor(QTextCursor::Start);
    QTextCharFormat fmt;
    fmt.setBackground(Qt::white);
    QTextCursor cursor(document());
    cursor.setCharFormat(fmt);
    setTextCursor(cursor);

    this->id = id;
    QFile f(m[id].first);
    f.open(QIODevice::ReadOnly);
    QTextStream st(&f);
    setPlainText(st.readAll());
    f.close();

    fmt.setBackground(Qt::yellow);
    long long sz = word.size();
    for (auto pos: m[id].second) {
        cursor.setPosition(pos, QTextCursor::MoveAnchor);
        cursor.setPosition(pos + sz, QTextCursor::KeepAnchor);
        cursor.setCharFormat(fmt);
    }
    cursor.setPosition(m[id].second[0], QTextCursor::MoveAnchor);
    cursor.setPosition(m[id].second[0] + sz, QTextCursor::KeepAnchor);
    setTextCursor(cursor);
    pos = 0;
    qDebug() << m[id].second[0];
}

void Viewer::next() {
    pos = (pos + 1) % (m[id].second.size());
    QTextCursor cursor(document());
    cursor.setPosition(m[id].second[pos], QTextCursor::MoveAnchor);
    cursor.setPosition(m[id].second[pos] + word.size(), QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}

void Viewer::prev() {
    pos = (pos - 1 + m[id].second.size()) % (m[id].second.size());
    QTextCursor cursor(document());
    cursor.setPosition(m[id].second[pos], QTextCursor::MoveAnchor);
    cursor.setPosition(m[id].second[pos] + word.size(), QTextCursor::KeepAnchor);
    setTextCursor(cursor);
}
