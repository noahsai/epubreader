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
    void setbuttonvisible(bool);
    bool buttonvisible();
signals:
    void top();
    void buttom();
public slots:
    void toshang();
    void toxia();

private slots:
    void setbutton();
private:
    void resizeEvent(QResizeEvent *);
    QPushButton *shang;
    QPushButton *xia;
};

#endif // MYWEBVIEW_H
