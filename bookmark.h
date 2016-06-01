#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QWidget>
#include<QApplication>
#include<QFile>
#include<QTextStream>
#include<QListWidgetItem>
#include<QDebug>
#include<QKeyEvent>



namespace Ui {
class bookmark;
}

class bookmark : public QWidget
{
    Q_OBJECT

public:
    explicit bookmark(QWidget *parent = 0);
    ~bookmark();

public slots:
    void add(QString& bm);
    void read(QString& n);
    void save();

private:
    Ui::bookmark *ui;
    QString name;
    QString tobm;

signals:
    void tobookmark(QString&);

private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // BOOKMARK_H
