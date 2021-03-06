#include "mywebview.h"

mywebview::mywebview(QWidget *parent) :
    QWebView(parent)
{
    oldwheel = 0;
    QVBoxLayout *vlayout  =  new QVBoxLayout(this);
    this->setLayout(vlayout);
    shang = new QPushButton("上一章",this);
    shang->setStyleSheet("QPushButton{background-color:#aa000000;color:white;}");
    shang->setCursor(QCursor(Qt::PointingHandCursor));
    connect(shang , SIGNAL(clicked(bool)) , this ,SLOT(toshang()));
    xia = new QPushButton("下一章" , this);
    xia->setStyleSheet("QPushButton{background-color:#aa000000;color:white;}");
    xia->setCursor(QCursor(Qt::PointingHandCursor));
    connect(xia  ,SIGNAL(clicked(bool)) ,this ,SLOT(toxia()));
    vlayout->addWidget(shang);
    vlayout->addWidget(xia);
    shang->hide();
    xia->hide();
}


void mywebview::toshang()
{
    int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
     this->page()->mainFrame()->scroll(0,-height()+20);//different from i
    if(i== this->page()->mainFrame()->scrollBarValue(Qt::Vertical))
    {
        emit top();
        shang->hide();
        xia->hide();
    }
}
void mywebview::toxia()
{
    int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
     this->page()->mainFrame()->scroll(0, height()-20);//different from i
    if(i== this->page()->mainFrame()->scrollBarValue(Qt::Vertical))
    {
        emit buttom();
        shang->hide();
        xia->hide();
    }
}

void mywebview :: wheelEvent(QWheelEvent *e){
    shang->hide();
    xia->hide();

    if(e->delta() > 0){
        int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
        if(i== this->page()->mainFrame()->scrollBarMinimum(Qt::Vertical))
       {
            shang->show();
        }
     emit   wheelup( true );
        //上
    }else{
        //下
        int i= this->page()->mainFrame()->scrollBarValue(Qt::Vertical);
        if(i== this->page()->mainFrame()->scrollBarMaximum(Qt::Vertical))
        {
            xia->show();
        }
        emit wheelup(false);
    }
    if(e->delta()*oldwheel >=0){
        oldwheel = e->delta();
        return    QWebView::wheelEvent(e);
    }
    oldwheel = e->delta();
}
