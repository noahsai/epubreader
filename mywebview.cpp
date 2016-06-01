#include "mywebview.h"

mywebview::mywebview(QWidget *parent) :
    QWebView(parent)
{

    shang=new QPushButton(this);
    xia=new QPushButton(this);
    shang->setFlat(true);
    shang->setFocusPolicy(Qt::NoFocus);
    shang->setCursor(Qt::PointingHandCursor);
    shang->setStyleSheet("QPushButton{border:0pxgroovegray;}");
    shang->setToolTip("上一页");
    shang->setShortcut(QKeySequence(Qt::Key_Left));
    xia->setFlat(true);
    xia->setFocusPolicy(Qt::NoFocus);
    xia->setCursor(Qt::PointingHandCursor);
    xia->setStyleSheet("QPushButton{border:0pxgroovegray;}");
    xia->setToolTip("下一页");
    xia->setShortcut(QKeySequence(Qt::Key_Right));

    shang->setVisible(false);
    xia->setVisible(false);

    connect(shang,SIGNAL(clicked()),this,SLOT(toshang()));
    connect(xia,SIGNAL(clicked()),this,SLOT(toxia()));

}

bool mywebview::buttonvisible()
{
    return shang->isVisible();
}


void mywebview::setbuttonvisible(bool)
{
    setbutton();
    shang->setVisible(true);
    xia->setVisible(true);
}

void mywebview::resizeEvent(QResizeEvent *even)
{
    QWebView::resizeEvent(even);
    setbutton();
}


void mywebview::toshang()
{
    int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
     this->page()->mainFrame()->scroll(0,-height()+20);//different from i
    if(i== this->page()->mainFrame()->scrollBarValue(Qt::Vertical))
    {
        emit top();
    }
}
void mywebview::toxia()
{
    int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
     this->page()->mainFrame()->scroll(0, height()-20);//different from i
    if(i== this->page()->mainFrame()->scrollBarValue(Qt::Vertical))
    {
        emit buttom();
    }
}


void mywebview::setbutton()//it can avoid the button disappear afre webview load();
{
    shang->setGeometry(0,0, width()/4, height()-13);
    xia->setGeometry(3* width()/4-13,0, width()/4, height()-13);
}

