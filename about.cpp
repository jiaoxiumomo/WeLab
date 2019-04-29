#include "about.h"
#include <QLayout>
#include <QPixmap>
#include <QIcon>
#include "JUTILITY.h"

about::about(QWidget *parent)
     :QDialog(parent)
{
    label1=new QLabel(this);
    label1->setText("WeLab 0.2");

    label2=new QLabel(this);
    QString text = QString::fromLocal8Bit(GetCopyRightText());
    label2->setText(text);


    label3=new QLabel(this);   //label加载图片
    QPixmap pixmap(":/images/Application");
    label3->setPixmap(pixmap);
    label3->setFixedSize(50,50);
    label3->setScaledContents(true);//图片适应控件大小
    label3->show();


    QVBoxLayout   *layout1=new QVBoxLayout; //垂直布局
    layout1->addWidget(label1);
    layout1->addWidget(label2);

    QHBoxLayout *layout2=new QHBoxLayout;  //横向布局
    layout2->addWidget(label3);
    layout2->addLayout(layout1);
    this->setLayout(layout2);
    this->setWindowTitle(tr("About WeLab"));
    this->setFixedSize(300,100);



}
about::~about()
{}
