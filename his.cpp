#include "his.h"
#include "ui_his.h"

his::his(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::his)
{
    ui->setupUi(this);
    ui->booklist->setVisible(false);
    ui->pushButton_2->setVisible(false);
    setLayout(ui->verticalLayout);
    read();

}

his::~his()
{
    delete ui;
    save();

}

void his::on_pushButton_2_clicked()
{
    ui->booklist->clear();
    save();
}

void his::addbook(QString& name,QString& file)
{
    if(!isexists(file))
    {
        QListWidgetItem* item = new QListWidgetItem(name);
        item->setToolTip(file);
        item->setTextAlignment(Qt::AlignHCenter);
        ui->booklist->addItem(item);
    }
}

bool his::isexists(QString& text)
{
    for(int i=0;i<ui->booklist->count();i++)
    {
        QListWidgetItem* item =ui->booklist->item(i);
        if(item->toolTip()==text) return true;
    }
    return false;
}

void his::save()
{
    QString out="";
    for(int i=0;i<ui->booklist->count();i++)
    {
        QListWidgetItem* item =ui->booklist->item(i);
        out+=item->text()+"|"+item->toolTip()+"\n";
    }
    QFile file(QApplication::applicationDirPath()+"/books");
    file.open(QIODevice::WriteOnly);
    QTextStream outf(&file);
    outf<<out;
    outf.flush();
    file.close();

}

void his::read()
{
    QFile file(QApplication::applicationDirPath()+"/books");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QString line;
        QListWidgetItem* item ;
        QStringList one;
        while(!in.atEnd())
        {
            line=in.readLine();
            one=line.split("|");
            item = new QListWidgetItem(one.at(0));
            item->setToolTip(one.at(1));
            item->setTextAlignment(Qt::AlignHCenter);
            ui->booklist->addItem(item);
        }
        file.close();
    }
}

void his::on_booklist_itemDoubleClicked(QListWidgetItem *item)
{
    QString filepath=item->toolTip();
    emit clicked(filepath);
}

void his::on_pushButton_3_clicked()
{
    emit openfile();
}

void his::on_pushButton_clicked()
{
    emit toreading();
}

void his::on_pushButton_4_clicked(bool checked)
{

    ui->booklist->setVisible(checked);
    ui->pushButton_2->setVisible(checked);
}
