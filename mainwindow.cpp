#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    reading=false;
    ui->menuBar->setVisible(false);
    isfull=false;
    books = new his;
    books->setBaseSize(QSize(600,420));
    connect(books,SIGNAL(clicked(const QString&)),this,SLOT(readfile(const QString&)));
    connect(books,SIGNAL(openfile()),this,SLOT(on_actionOpen_triggered()));
    connect(books,SIGNAL(toreading()),this,SLOT(on_action_books_triggered()));
    webview= new mywebview;
    webview->load(QUrl("qrc:/setup.html"));
    webview->settings()->setUserStyleSheetUrl(QUrl("qrc:/return.css"));
    webview->setVisible(false);
    ui->verticalLayout->addWidget(books);
    ui->verticalLayout->addWidget(webview);
    setCentralWidget(ui->verticalLayoutWidget);


    ui->menuBar->addAction(ui->action_books);//首页ACTION

    dock=new QDockWidget("目录",this);
    dock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetClosable);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    content=new QListWidget(dock);
    dock->setWidget(content);
    content->setMinimumWidth(210);
    content->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Preferred);
    dock->setBackgroundRole(QPalette::Dark);
    dock->setAutoFillBackground(true);
    this->addDockWidget(Qt::LeftDockWidgetArea,dock);
    this->dock->hide();
    now=-1;//lock the button;
    connect(webview,SIGNAL(top()),this,SLOT(toshang()));
    connect(webview,SIGNAL(buttom()),this,SLOT(toxia()));
    connect(webview , SIGNAL(wheelup(bool)) , ui->menuBar , SLOT(setVisible(bool)));
    connect(content,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(contentclick(QListWidgetItem*)));

    ui->act_qiangzhi->setEnabled(false);
    set=new settiing(this);
    connect(set,SIGNAL(ok(QString&)),this,SLOT(setok(QString&)));
    connect(set,SIGNAL(refreshok()),this,SLOT(refresh()));
    connect(set,SIGNAL(fontchange(QString&)),this,SLOT(fontchange(QString&)));
    connect(set,SIGNAL(sizechange(int)),this,SLOT(sizechange(int)));
    connect(set,SIGNAL(gotcss()),this,SLOT(gotcss()));
    connect(set,SIGNAL(cancel()),this,SLOT(initset()));
    connect(set,SIGNAL(mode(int)),this,SLOT(mode(int)));

    dock2=new QDockWidget("显示设置",this);
    dock2->setFeatures(QDockWidget::DockWidgetMovable);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    dock2->setWidget(set);
    dock2->setBackgroundRole(QPalette::Dark);
    dock2->setAutoFillBackground(true);
    dock2->setFixedWidth(210);
    this->addDockWidget(Qt::RightDockWidgetArea,dock2);
    this->dock2->hide();
    connect(ui->actionSet,SIGNAL(triggered()),this,SLOT(setting_sh()));
    apppath=QApplication::applicationDirPath();

    bookmarks=new bookmark(this);
    bookmarks->setVisible(false);
    connect(bookmarks,SIGNAL(tobookmark(QString&)),this,SLOT(getbookmark(QString&)));

    initset();//page set at this.
}


QSize MainWindow::sizeHint() const
{
    return QSize(600,420);
}



void MainWindow::full(bool fu)
{
    if(fu)
    {
        showFullScreen();
        if(webview->isVisible()) ui->menuBar->hide();
    }
    else
    {
        showNormal();
        if(webview->isVisible()) ui->menuBar->show();
    }

}

void MainWindow::content_sh()
{
    if(webview->isVisible())
    {
        dock->setVisible(!dock->isVisible());
    }
}

void MainWindow::setting_sh()
{
    if(webview->isVisible())
        dock2->setVisible(!dock2->isVisible());
}

MainWindow::~MainWindow()
{
    //system("rm -rf /tmp/epub_tmp");
    if(reading)
    {
        makebookmark();
        bookmarks->save();
    }
    delete ui; 

}

void MainWindow::setok(QString& set)
{
    readset(set);
    dock2->setVisible(false);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{

    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    qDebug()<<fileName;

    QStringList list=fileName.split(".");
    //    qDebug()<<list.at(list.size()-1);
    QString name=list.at(list.size()-1);
    if(name.toLower()=="epub") readfile(fileName);

}//||name.toLower()=="txt"




void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("epub(*.epub);;所有文件(*)"));
    //qDebug()<<fileName;//file path
   readfile(fileName);

}


void MainWindow::readfile(const QString& fileName)
{
    if(!fileName.isEmpty())
    {
        QString book=fileName;
        QStringList list=fileName.split("/");
        QString name=list.at(list.size()-1);//file name
        books->addbook(name,book);
        setWindowTitle(name);
        path=apppath;//the path of program
        dir.mkdir("/tmp/epub_tmp");
        rootpath="/tmp/epub_tmp/"+name;
        QDir ().mkpath(rootpath);

        QString tmp1;
        QString unzip = "unar -D -f  ";//使用unar命令，需系统额外安装, 参数-D：不创建文件夹，-o: 输出位置
        tmp1=unzip+ ' \"'+fileName+'\"'+" -o  "+'\"'+rootpath+'/\"';
        qDebug()<<tmp1;
        if(!system(tmp1.toStdString().c_str()))
        {
            qDebug()<<"unar epub  file ok";
            if(reading)
            {
                makebookmark();
                bookmarks->save();
            }
            work();
            reading=true;


        books->setVisible(false);
        webview->setVisible(true);
        ui->menuBar->setVisible(true);
        bookmarks->read(name);//在生成目录里加上了阻塞，保证load完才执行read读取最后阅读记录
        setMinimumSize(200,200);
     }
    }

}
int MainWindow::work()
{
    QString opf;
    opf=rootpath+"/"+readxml(rootpath+"/META-INF/container.xml");
    opffilepath=opf;
    qDebug()<<"opf:"<<opf;
    QString ncx=inpath+readopf(opf);
    qDebug()<<"ncx:"<<ncx;
    int result=readncx(ncx);
    now=0;//reset;
    if(!result) return 0;
    return 1;
}

QString MainWindow::readxml(QString filename)
{
    QString opf;
    //qDebug()<<"readxml:"<<filename;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString text=in.readAll();
   // qDebug()<<text;
    QRegularExpression re("path=\\\"\\S*\\.[o,O][p,P][f,F]");
    QRegularExpressionMatch match = re.match(text);
    opf=match.captured(0);
    opf.remove("path=\"");
    inpath=opf;
    inpath.remove(QRegularExpression("\\w*\\.[o,O][p,P][f,F]"));
    inpath=rootpath+"/"+inpath;
    file.close();
    qDebug()<<"return"<<opf;
    return opf;
}

QString MainWindow::readopf(QString filename)
{
    QString ncx;
   // qDebug()<<"readopf:"<<filename;
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString text=in.readAll();
   // qDebug()<<text;
    QRegularExpression re("(?<=href=\").+?\\.[n,N][c,C][x,X]");
    QRegularExpressionMatch match = re.match(text);
    ncx=match.captured(0);
    //ncx.remove("href=\"");
    file.close();
    return ncx;

}
int MainWindow::readncx(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::ReadOnly );
    QTextStream input(&file);
    QString text= input.readAll();
   // qDebug()<<text;
    QRegularExpression re("(?<=\\<text\\>).+?(?=\\</text)");
    QStringList sitelist;
    QStringList titlelist;
   // titlelist.clear();
   // sitelist.clear();
    QRegularExpressionMatchIterator i = re.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        titlelist.append(match.captured(0));
    }

    re.setPattern("(?<=src=\").+?(?=\")");
    i=re.globalMatch(text);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
         sitelist.append(match.captured(0));
    }
    qDebug()<<sitelist;
    qDebug()<<titlelist;
    file.close();
    if(titlelist.count()-sitelist.count()>2) return 0;
    makecontent(titlelist,sitelist);
    return 1;
}

void MainWindow::makecontent(QStringList title,QStringList site)
{
    int i=0;
    content->clear();
    int dif=title.count()-site.count();
    if(dif>0)
    {
        bookname=title.at(0);
        if(dif==2) author=title.at(1);
    }
    for(;i<site.count();i++)
    {
        QListWidgetItem *item=new QListWidgetItem();
        item->setText(title.at(i+dif));
        item->setWhatsThis(site.at(i));
        item->setToolTip(title.at(i+dif));
        item->setSizeHint(QSize(0,22));
        content->addItem(item);
    }

    opfpath="";//reseting it when a file loaded.
    if(qiangzhi||mycss) changecss();
    webview->load(QUrl("file://"+inpath+site.at(0)));
    content->setCurrentRow(0);
    now=0;
  //  qDebug()<<"file://"+inpath+site.at(0);

}

void MainWindow::contentclick(QListWidgetItem* item)
{
    webview->load(QUrl("file://"+inpath+item->whatsThis()));
    now=content->currentRow();
    //qDebug()<<now;
}

void MainWindow::toshang()
{
    if(now>=0)
    {
        now--;
        if(now < 0) now=0;
        else
        {
            webview->load(QUrl("file://"+inpath+content->item(now)->whatsThis()));
            content->item(now)->setSelected(true);
        }
    }

}
void MainWindow::toxia()
{
    if(now>=0)
    {
       // qDebug()<<"b:"<<now;
        now++;
        //qDebug()<<"a:"<<now;
        //qDebug()<<"count:"<<content->count();
        if(now >= content->count()) now=content->count();
        else
        {
            webview->load(QUrl("file://"+inpath+content->item(now)->whatsThis()));
            content->item(now)->setSelected(true);
        }
    }
}


void MainWindow::changecss()
{

    QFile file(opffilepath);
    if(QFile::exists(opffilepath))
    {
        if(opfpath=="")
        {
            file.open(QIODevice::ReadOnly);
            QTextStream in(&file);
            QString text=in.readAll();
           // qDebug()<<text;
            QRegularExpression re("href=\\\"\\S*\\.[c,C][s,S][s,S]");
            QRegularExpressionMatch match = re.match(text);
            QString css=match.captured(0);
            css.remove("href=\"");
            opfpath=opffilepath;
            opfpath.remove(QRegularExpression("/\\w*\\.opf"));
            csspath=opfpath+"/"+css;
            qDebug()<<csspath;
        }
        QFile::rename(csspath,csspath+"_");
    }
    file.close();

}


void MainWindow::on_act_qiangzhi_triggered(bool checked)
{
    QString morepath=apppath+"/more.css";
    if(checked)
    {
        qiangzhi=true;
        changecss();
        webview->settings()->setUserStyleSheetUrl(QUrl("file://"+morepath));


    }
    else
    {
        qiangzhi=false;
        QFile::rename(csspath+"_",csspath);
        webview->settings()->setUserStyleSheetUrl(QUrl("file://"+morepath));
    }
    webview->reload();

    QString text;
    QFile file(apppath+"/setting");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream io(&file);
        text=io.readAll();
        file.close();
        QStringList list=text.split("|");
        if(checked) list.replace(0,"1+");
        else  list.replace(0,"1");

        file.open(QIODevice::WriteOnly);
        io<<list.join("|");
        io.flush();
        file.close();
    }
}
void MainWindow::initset()
{
    qDebug()<<"init in";
    qiangzhi=false;
    mycss=false;
    QFile file(apppath+"/setting");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString text=in.readAll();
        readset(text);
        file.close();
        //qDebug()<<"init out";
    }
    if(file.isOpen()) file.close();

}

void MainWindow::readset(QString set)
{
    qDebug()<<"set in";

    QStringList sets=set.split("|");
    qDebug()<<sets;
    if(!sets.at(0).isEmpty()&&sets.at(0)!="2")
    {

        if(!sets.at(1).isEmpty()) {
            QString family;
            family=sets.at(1);
            webview->settings()->setFontFamily(QWebSettings::StandardFont,family);
            webview->settings()->setFontFamily(QWebSettings::FixedFont,family);
            webview->settings()->setFontFamily(QWebSettings::SerifFont,family);
            webview->settings()->setFontFamily(QWebSettings::SansSerifFont,family);
            webview->settings()->setFontFamily(QWebSettings::CursiveFont,family);
            webview->settings()->setFontFamily(QWebSettings::FantasyFont,family);
        }
        if(!sets.at(2).isEmpty())  webview->settings()->setFontSize(QWebSettings::DefaultFontSize,sets.at(2).toUInt());
        if(sets.at(0)=="1"||sets.at(0)=="1+")
        {
            QString morepath=apppath+"/more.css";
            if(QFile::exists(morepath)) webview->settings()->setUserStyleSheetUrl(QUrl("file://"+morepath));
            else qDebug()<<"more.css not exist.";
            ui->act_qiangzhi->setEnabled(true);
            if(sets.at(0)=="1+")
            {
                qiangzhi=true;
                ui->act_qiangzhi->setChecked(true);
            }
        }
        else
        {
            if(!csspath.isEmpty()) QFile::rename(csspath+"_",csspath);//when the file opened ,reset the css
            webview->settings()->setUserStyleSheetUrl(QUrl("qrc:/return.css"));
            ui->act_qiangzhi->setEnabled(false);
        }
    }

    else if(sets.at(0)=="2")
    {
        mycss=true;
        if(!opffilepath.isEmpty()) changecss();// 已打开文件才执行。
        QString mycsspath=apppath+"/mycss.css";
        if(QFile::exists(mycsspath)) webview->settings()->setUserStyleSheetUrl(QUrl("file://"+mycsspath));
        else qDebug()<<"mycss.css not exist.";
        ui->act_qiangzhi->setEnabled(false);
    }
    qDebug()<<"set out";
    //webview->reload();

}

void MainWindow::on_action_4_triggered()
{
    this->close();
}

void MainWindow::refresh()
{
    webview->settings()->setUserStyleSheetUrl(QUrl("file://"+apppath+"/more.css"));
    webview->reload();
}

void MainWindow::fontchange(QString& family)
{
    webview->settings()->setFontFamily(QWebSettings::StandardFont,family);
    webview->settings()->setFontFamily(QWebSettings::FixedFont,family);
    webview->settings()->setFontFamily(QWebSettings::SerifFont,family);
    webview->settings()->setFontFamily(QWebSettings::SansSerifFont,family);
    webview->settings()->setFontFamily(QWebSettings::CursiveFont,family);
    webview->settings()->setFontFamily(QWebSettings::FantasyFont,family);
}

void MainWindow::sizechange(int arg1)
{
    webview->settings()->setFontSize(QWebSettings::DefaultFontSize,arg1);
}

void MainWindow::gotcss()
{
    webview->settings()->setUserStyleSheetUrl(QUrl("file://"+apppath+"/mycss.css"));
    webview->reload();
}

void MainWindow::mode(int mode)
{
    switch(mode)
    {
    case 1:
        webview->settings()->setUserStyleSheetUrl(QUrl("qrc:/return.css"));
        webview->reload();break;
    case 2:
        refresh();break;
    case 3:
        gotcss();break;
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        if(isfull)
        {
            full(false);
            isfull=false;
        }
        break;
    case Qt::Key_F11:
        full(!isfull);
        isfull=!isfull;
        break;
    case Qt::Key_Left:  webview->toshang();break;
    case Qt::Key_Right: webview->toxia();break;
    case Qt::Key_C:  content_sh();break;
    case Qt::Key_S: setting_sh();break;
    case Qt::Key_B: on_action_shuqian_triggered();break;
    default :QMainWindow::keyPressEvent(event);break;
    }
}

void MainWindow::on_action_books_triggered()
{
    dock->setVisible(false);
    dock2->setVisible(false);
    ui->menuBar->setVisible(!webview->isVisible());
    webview->setVisible(!webview->isVisible());
    books->setVisible(!books->isVisible());

}

void MainWindow::on_action_mulu_triggered()
{
    content_sh();
}

void MainWindow::getbookmark(QString& bm)
{
    QStringList list=bm.split("|");//item->whatsThis()
    int num=list.at(0).toInt();
    QString q=list.at(2);
    q=q.remove("%");
    double p=q.toDouble()/100.0;

    webview->load(QUrl("file://"+inpath+content->item(num)->whatsThis()));

    QEventLoop loop;
    QObject::connect(webview,SIGNAL(loadFinished(bool)),&loop, SLOT(quit()));
    loop.exec();
    now=num;
    content->item(num)->setSelected(true);
    int pagelen=webview->page()->mainFrame()->scrollBarMaximum(Qt::Vertical)+webview->height();
    int t = pagelen * p;
    //qDebug()<<t<<"t,page"<<webview->height();
    webview->page()->mainFrame()->scroll(0,t);
}

void MainWindow::makebookmark()
{
    if(reading)
    {
        int pagelen=webview->page()->mainFrame()->scrollBarMaximum(Qt::Vertical)+webview->height();
        int t = (webview->page()->mainFrame()->scrollBarValue(Qt::Vertical)*1000)/pagelen;
        QString bookmark=QString().setNum(now)+"|"+content->item(now)->text()+"|"+QString().setNum(t);
        bookmarks->add(bookmark);
    }
}

void MainWindow::on_action_triggered()
{
    makebookmark();
    if(reading)
        QMessageBox::information(this,"添加书签","添加完成！");

}

void MainWindow::on_action_shuqian_triggered()
{
    if(webview->isVisible())
    {
        bookmarks->setVisible(!bookmarks->isVisible());
    }
}
