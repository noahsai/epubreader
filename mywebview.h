#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QWebView>
#include<QtWidgets>
#include<QtWebKitWidgets>
#include <QWebFrame>

class mywebview : public QWebView
{
    Q_OBJECT
public:
    explicit mywebview(QWidget *parent = 0);
signals:
    void top();
    void buttom();
    void wheelup(bool);
public slots:
    void toshang();
    void toxia();

private slots:
private:
    QPushButton *shang;
    QPushButton *xia;
    void wheelEvent(QWheelEvent *);
    int oldwheel;

};

#endif // MYWEBVIEW_H
