#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QtWebKitWidgets>
#include<QtWidgets>
#include<QFile>
#include<QDockWidget>
#include<mywebview.h>
#include<settiing.h>
#include<QApplication>
#include<his.h>
#include<QVBoxLayout>
#include<bookmark.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void readfile(const QString&);

private slots:
    void on_actionOpen_triggered();
    void contentclick(QListWidgetItem* item);
    void toshang();
    void toxia();
    void readset(QString set);

    void on_act_qiangzhi_triggered(bool checked);

    void on_action_4_triggered();
    void content_sh();
    void setok(QString&);
    void refresh();
    void fontchange(QString&);
    void sizechange(int);
    void gotcss();
    void initset();
    void mode(int);
    void setting_sh();
    void full(bool);

    void on_action_books_triggered();

    void on_action_mulu_triggered();
    void getbookmark(QString& bm);
    void makebookmark();

    void on_action_triggered();

    void on_action_shuqian_triggered();

private:
    Ui::MainWindow *ui;
    mywebview *webview;
    QString bookname,author;
    //QString filename;
    QString path;//program path
    QDir dir;// to get the program path
    QString rootpath;//
    QString inpath;
    QString readxml(QString filename);
    QString readopf(QString filename);
    int readncx(QString filename);
    int work();
    void makecontent(QStringList title,QStringList site);

    QListWidget *content;
    QDockWidget *dock,*dock2;
//    QAction *sh_content;

    //QDir dir;
    settiing *set;
    QString csspath;
    void changecss();
    bool qiangzhi;
    bool mycss;

    QString opfpath;
    QString opffilepath;
    QString apppath;

    int now;
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void keyPressEvent(QKeyEvent *event);
    QSize sizeHint() const;

    bool isfull;
    his* books;

    bookmark* bookmarks;
    bool reading;

};

#endif // MAINWINDOW_H
