#include "viewer.h"

#include <QTextStream>

Viewer::Viewer(QWidget *parent = 0) : QPlainTextEdit(parent)
{
    setReadOnly(true);
}

void Viewer::display(int id) {
    QFile f(m[id].first);
    f.open(QIODevice::ReadOnly);
    QTextStream st(&f);
    setPlainText(st.readAll());
    f.close();


}
