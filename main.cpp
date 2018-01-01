#include "mainwindow.h"
#include <QApplication>
#define VERSION "Epub阅读器 v0.3"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir dir(a.applicationDirPath());
    dir.mkpath(a.applicationDirPath()+"/bookmarks");
    MainWindow w;
    w.setWindowTitle( VERSION );
    w.setWindowIcon(QIcon(":/book.png"));
    w.setBackgroundRole(QPalette::Light);
    w.setAutoFillBackground(true);

    if(argv[1])
    {
       w.readfile(QString(argv[1]));
    }

    w.show();
    return a.exec();
}
