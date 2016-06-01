#ifndef SETTIING_H
#define SETTIING_H

#include <QDialog>
#include<QtWidgets>
#include<QtWebKitWidgets>
#include<QApplication>
#include<QTimer>
namespace Ui {
class settiing;
}

class settiing : public QDialog
{
    Q_OBJECT

public:
    explicit settiing(QWidget *parent = 0);
    ~settiing();

signals:
    void ok(QString&);
    void refreshok();
    void fontchange(QString&);
    void sizechange(int);
    void gotcss();
    void cancel();
    void mode(int);

private slots:
    void on_fontselect_clicked();

    void refresh();

    void on_spinBox_valueChanged(int arg1);

    void on_more_clicked(bool checked);

    void on_mycss_clicked(bool checked);

    void on_fcolor_button_clicked();

    void on_bgcolor_button_clicked();



    void on_loadcss_clicked();

    void toaccepted();

    void on_spinBox_2_valueChanged(int arg1);


    void on_back_clicked();

    void torejected();

    void on_checkBox_bold_clicked();

    void on_checkBox_xieti_clicked();

    void on_pushButton_clicked();


private:
    Ui::settiing *ui;
    QString family;
    int size;
    int type;
    QColor *color;
    bool bgchongfu;
    bool bgjuzhong;
    QString mycsspath,morepath;
    void init();
    void makecss();
    //QStringList *backup;

};

#endif // SETTIING_H
