#include "settiing.h"
#include "ui_settiing.h"

settiing::settiing(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settiing)
{
    ui->setupUi(this);
    this->setLayout(ui->horizontalLayout);
    ui->group->setLayout(ui->verticalLayout_8);
    family="";
    size=16;
    type=0;
    ui->needcss->hide();
    ui->loadcss->hide();
    ui->group->hide();
    mycsspath=QApplication::applicationDirPath()+"/mycss.css";
   // qDebug()<<mycsspath;
    morepath=QApplication::applicationDirPath()+"/more.css";
    //qDebug()<<morepath;
    init();
    qDebug("builded ok:");
    qDebug()<<QApplication::applicationDirPath();
}

settiing::~settiing()
{
    delete ui;
}


void settiing::on_fontselect_clicked()
{
    bool ok;
    QFont font=QFontDialog::getFont(&ok,this->font(),this,"font");
    if(ok)
    {
        family=font.family();

        ui->fontselect->setText(family);
        ui->fontselect->setToolTip(family);
        qDebug()<<font.style()<<"|"<<font.weight();
        emit fontchange(family);
    }
}
void settiing::makecss()
{
    QFile file(morepath);
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out<<"img{width:100%;}\n";
    out<<"p{text-align: justify;\
         text-indent: 2em;\
         line-height:130%;\
         margin-bottom:auto;}\
         h,h1,h2,h3,h4,h5,h6{text-align: center;\
                             font-weight:bold;}";
    out<<"p,h,h1,h2,h3,h4,h5,h6{";
    if(ui->checkBox_bold->isChecked())out<<"font-weight:bold;";
    if(ui->checkBox_xieti->isChecked())out<<"font-style:italic;";
    if(!ui->fontcolor->text().isEmpty())out<<"color:"<<ui->fontcolor->text();
    out<<";}\n";
    if(!ui->bgcolor->text().isEmpty())out<<"body{background-color:"<<ui->bgcolor->text()<<";}\n";
    out<<"p{line-height:" <<ui->spinBox_2->value()<<"%;}\n";
    out.flush();
    file.close();
    QTimer *timer = new QTimer(this);
    QEventLoop loop;
    QObject::connect(timer,SIGNAL(timeout()),&loop, SLOT(quit()));
    timer->start(300);
    loop.exec();
}

void settiing::refresh()
{
      ui->group->setEnabled(false);
      makecss();
      ui->group->setEnabled(true);
      emit refreshok();
      qDebug("refresh ok");
}

void settiing::on_spinBox_valueChanged(int arg1)
{
        size=arg1;
        emit sizechange(size);
}

void settiing::on_more_clicked(bool checked)
{
    ui->back->setVisible(true);
    if(checked)
    {
        ui->needcss->hide();
        ui->loadcss->hide();
        ui->mycss->setChecked(false);

        ui->group->show();
        ui->fontselect->setEnabled(true);
        ui->spinBox->setEnabled(true);
        type=1;
        if(QFile::exists(morepath))
        {
            emit mode(2);
        }
    }
    else if(!ui->mycss->isChecked()) type=0;
    if(!checked)
    {
        ui->group->hide();
        emit mode(1);
    }
}

void settiing::on_mycss_clicked(bool checked)
{
    if(checked)
    {
        bool on=true;
        ui->group->hide();
        ui->back->hide();
        ui->fontselect->setEnabled(false);
        ui->spinBox->setEnabled(false);
        ui->needcss->setWordWrap(on);
        ui->needcss->show();
        ui->loadcss->show();

        ui->more->setChecked(false);
        type=2;

        if(QFile::exists(mycsspath))
        {
            emit mode(3);
        }
        qDebug()<<"load css ok";
    }


    else if(!ui->more->isChecked()) type=0;
    if(!checked)
    {
        ui->needcss->hide();
        ui->loadcss->hide();
        ui->fontselect->setEnabled(true);
        ui->spinBox->setEnabled(true);
        ui->back->setVisible(true);
        emit mode(1);
    }
}

void settiing::on_fcolor_button_clicked()
{
    color= new QColor;
    *color = QColorDialog::getColor();
    if(color->isValid())
    {
        ui->fontcolor->setText(color->name());
        refresh();
    }
}

void settiing::on_bgcolor_button_clicked()
{
    color= new QColor;
    *color = QColorDialog::getColor();
    if(color->isValid())
    {
        ui->bgcolor->setText(color->name());
        refresh();
    }
}


void settiing::on_loadcss_clicked()
{
    QString filename=QFileDialog::getOpenFileName(this,"选择CSS文件","","CSS文件(*.css)");
   // qDebug()<<filename;
    if(!filename.isNull())
    {
        QFile::remove(mycsspath);
        QFile::copy(filename,mycsspath);
        emit gotcss();
    }
}

void settiing::toaccepted()
{
    QString out="";
    if(!ui->more->isChecked()&&!ui->mycss->isChecked()) out+="0|";
    else if(ui->more->isChecked()) out+="1|";
    else out+="2|";
    if(ui->fontselect->text()!="只能选择字体！！") out+=ui->fontselect->text()+"|";
    else out+="|";
    out+=ui->spinBox->text()+"|";
    out+=ui->fontcolor->text()+"|";
    out+=ui->bgcolor->text()+"|";
    out+=ui->spinBox_2->text()+"|";
    ui->checkBox_bold->isChecked()?out+="1|":out+="|";
    if(ui->checkBox_xieti->isChecked()) out+="1";

    QFile file(QApplication::applicationDirPath()+"/setting");
    file.open(QIODevice::WriteOnly);
    QTextStream outf(&file);
    outf<<out;
    outf.flush();
    file.close();


    emit ok(out);
}

void settiing::init()
{
    QFile file(QApplication::applicationDirPath()+"/setting");;
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        QStringList sets=in.readAll().split("|");
        qDebug()<<"setting-init:"<<sets;
        file.close();
        if(sets.count()==8)
        {
            if(!sets.at(1).isEmpty()) {
                ui->fontselect->setText(sets.at(1));
               }
            if(!sets.at(2).isEmpty())
            {
                ui->spinBox->setValue(sets.at(2).toInt());
            }
            if(!sets.at(3).isEmpty()) ui->fontcolor->setText(sets.at(3));
            if(!sets.at(4).isEmpty()) ui->bgcolor->setText(sets.at(4));
            if(!sets.at(5).isEmpty()) ui->spinBox_2->setValue(sets.at(5).toInt());
            sets.at(6).isEmpty()?ui->checkBox_bold->setChecked(false):ui->checkBox_bold->setChecked(true);
            sets.at(7).isEmpty()?ui->checkBox_xieti->setChecked(false):ui->checkBox_xieti->setChecked(true);
            if(sets.at(0)=="1"||sets.at(0)=="1+")
            {
                ui->more->setChecked(true);
                on_more_clicked(true);
                refresh();

            }
            else if(sets.at(0)=="2")
            {
                makecss();
                ui->mycss->setChecked(true);
                on_mycss_clicked(true);
            }
            else
            {
                makecss();
                ui->more->setChecked(false);
                ui->mycss->setChecked(false);
                on_more_clicked(false);
                on_mycss_clicked(false);
            }
        }

    }
    qDebug("setting:init ok");

}

void settiing::on_spinBox_2_valueChanged(int arg1)
{
    refresh();
}

void settiing::on_back_clicked()
{
    init();
    emit cancel();
}

void settiing::torejected()
{
    init();
    emit cancel();
}

void settiing::on_checkBox_bold_clicked()
{
    refresh();
}

void settiing::on_checkBox_xieti_clicked()
{
    refresh();
}

void settiing::on_pushButton_clicked()
{
    toaccepted();
}
