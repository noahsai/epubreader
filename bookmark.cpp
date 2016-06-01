#include "bookmark.h"
#include "ui_bookmark.h"

bookmark::bookmark(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::bookmark)
{
    ui->setupUi(this);
    setLayout(ui->verticalLayout);
    setWindowTitle("书签");
    setWindowFlags(Qt::Dialog);
    name="";
}

bookmark::~bookmark()
{
    delete ui;
}

void bookmark::add(QString &bm)
{
    if(!name.isEmpty())
    {
        QListWidgetItem* item ;
        QStringList list=bm.split("|");
        int t=list.at(2).toInt();//千分之几
        double p=t/10;//变为百分之几
        list.removeLast();
        list.append(QString().setNum(p));
        item = new QListWidgetItem(list.join("|")+"%");
        ui->listWidget->addItem(item);
        item->setToolTip(list.join("|")+"%");
    }
}

void bookmark::read(QString &n)
{
    ui->listWidget->clear();
    name=n;
    QFile file(QApplication::applicationDirPath()+"/bookmarks/"+name+".bm");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line;
        QListWidgetItem* item ;
        while(!in.atEnd())
        {
            line=in.readLine();
            item = new QListWidgetItem(line);
            ui->listWidget->addItem(item);
            item->setToolTip(line);

        }
        if(!line.isEmpty())
        {
            item = ui->listWidget->takeItem(ui->listWidget->count()-1);
            tobm=item->text();
            emit tobookmark(tobm);
            file.close();
        }
    }
}

void bookmark::save()
{
    if(!name.isEmpty())
    {
        QString out="";
        for(int i=0;i<ui->listWidget->count();i++)
        {
            QListWidgetItem* item =ui->listWidget->item(i);
            out+=item->text()+"\n";
        }
        QFile file(QApplication::applicationDirPath()+"/bookmarks/"+name+".bm");
        file.open(QIODevice::WriteOnly);
        QTextStream outf(&file);
        outf<<out;
        outf.flush();
        file.close();
    }
}

void bookmark::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    tobm=item->text();
    emit tobookmark(tobm);
}

void bookmark::on_pushButton_clicked()
{
    ui->listWidget->clear();
}

void bookmark::on_pushButton_2_clicked()
{
    QList<QListWidgetItem* > list= ui->listWidget->selectedItems();
    for(int i=0;i<list.count();i++)
    {
        int n=ui->listWidget->row(list.at(i));
        ui->listWidget->takeItem(n);
    }
}
