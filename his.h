#ifndef HIS_H
#define HIS_H

#include <QWidget>
#include<QApplication>
#include<QFile>
#include<QTextStream>
#include<QListWidgetItem>
#include<QDebug>

namespace Ui {
class his;
}

class his : public QWidget
{
    Q_OBJECT

public:
    explicit his(QWidget *parent = 0);
    ~his();
    void addbook(QString& name,QString& file);
    void save();

private slots:
    void on_pushButton_2_clicked();

    void on_booklist_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked(bool checked);

signals:
    void clicked(const QString&);
    void toreading();
    void openfile();

private:
    Ui::his *ui;
    void read();
    bool isexists(QString& text);

};

#endif // HIS_H
