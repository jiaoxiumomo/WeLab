#include "pannelsetting.h"
//#include "DialogRealBoard.h"

pannelSetting::pannelSetting(QWidget *parent)
      :QDialog(parent)
{
    //mainwindow=new MainWindow;
    tabWidget=new QTabWidget(this);
    keyGroupBox=new QGroupBox;
    keyGroupBox->setTitle(tr("按键按下时的逻辑电平"));
    sstubeGroupBox=new QGroupBox;
    sstubeGroupBox->setTitle(tr("七段数码管的段点亮电平"));
    panelName=new QLabel;
    panelName->setText(tr("实验板名称:"));
    remind=new QLabel;
    remind->setText(tr("注意：改变此项需要重新打开虚拟面板"));
    comobox=new QComboBox;
    comobox->addItem(tr("自定义"));
    comobox->addItem("JuLabPocket");
    comobox->addItem("DE2-115");
    comobox->addItem("Nexy4DDR");
    keyDown0=new QRadioButton;
    keyDown0->setText(tr("低电平"));
    keyDown1=new QRadioButton;
    keyDown1->setText(tr("高电平"));
    light0=new QRadioButton;
    light0->setText(tr("低电平(共阳极)"));
    light1=new QRadioButton;
    light1->setText(tr("高电平(共阴极)"));
    //realBoardSetting=new QPushButton;
    //realBoardSetting->setText(tr("确定实际面板设置"));
    layout2=new QGridLayout;
    layout2->addWidget(keyDown0,0,0,2,1);
    layout2->addWidget(keyDown1,0,3,2,1);
    layout2->addWidget(remind,4,0,2,5);
    keyGroupBox->setLayout(layout2);
    layout3=new QGridLayout;
    layout3->addWidget(light0,0,0,2,1);
    layout3->addWidget(light1,0,3,2,1);
    sstubeGroupBox->setLayout(layout3);
    realPanelLayout=new QGridLayout;
    realPanelLayout->addWidget(panelName,0,0,1,2);
    realPanelLayout->addWidget(comobox,1,0,1,5);
    realPanelLayout->addWidget(keyGroupBox,2,0,5,5);
    realPanelLayout->addWidget(sstubeGroupBox,7,0,3,5);
   for(int i=0;i<5;i++)
   {
       label[i]=new QLabel;
       spinBox[i]=new QSpinBox;
       spinBox[i]->setRange(0,50);
       spinBox[i]->setEnabled(false);
   }
   label[0]->setText(tr("开关数量："));
   label[1]->setText(tr("按键数量："));
   label[2]->setText(tr("红灯数量："));
   label[3]->setText(tr("绿灯数量："));
   label[4]->setText(tr("数码管数量："));
   spinBox[0]->setValue(36);
   spinBox[0]->setRange(0,36);
   spinBox[1]->setValue(5);
   spinBox[1]->setRange(0,5);
   spinBox[2]->setValue(18);
   spinBox[2]->setRange(0,18);
   spinBox[3]->setValue(18);
   spinBox[3]->setRange(0,18);
   spinBox[4]->setValue(8);
   spinBox[4]->setRange(0,8);
   for(int i=0;i<2;i++)
   {
       button[i]=new QPushButton;
   }
   button[0]->setText(tr("确定"));
   button[1]->setText(tr("取消"));
   nameLabel=new QLabel;
   nameLabel->setText(tr("名称："));
   describeLabel=new QLabel;
   describeLabel->setText(tr("标识："));
   nameLineEdit=new QLineEdit;
   nameLineEdit->setPlaceholderText(tr("请输入实验板名称"));
   describeLineEdit=new QLineEdit;
   describeLineEdit->setPlaceholderText(tr("仿真标识，普通用户请勿修改"));  
   layout=new QGridLayout;
   layout->addWidget(label[0],0,0,1,1);
   layout->addWidget(label[1],1,0,1,1);
   layout->addWidget(label[2],2,0,1,1);
   layout->addWidget(label[3],3,0,1,1);
   layout->addWidget(label[4],4,0,1,1);
   layout->addWidget(spinBox[0],0,1,1,1);
   layout->addWidget(spinBox[1],1,1,1,1);
   layout->addWidget(spinBox[2],2,1,1,1);
   layout->addWidget(spinBox[3],3,1,1,1);
   layout->addWidget(spinBox[4],4,1,1,1);
   groupBox=new QGroupBox;
   groupBox->setTitle(tr("控件信息设置"));
   groupBox->setLayout(layout);
   mainLayout=new QGridLayout;
   mainLayout->addWidget(nameLabel,0,0,1,2);
   mainLayout->addWidget(nameLineEdit,1,0,1,5);
   mainLayout->addWidget(describeLabel,2,0,1,2);
   mainLayout->addWidget(describeLineEdit,3,0,1,5);
   mainLayout->addWidget(groupBox,4,0,5,5);
   //mainLayout->addWidget( button[0],9,1,1,1);
   //mainLayout->addWidget( button[1],9,3,1,1);
   widget1=new QWidget;
   widget2=new QWidget;
   widget1->setLayout(mainLayout);
   widget2->setLayout(realPanelLayout);
   tabWidget->addTab(widget1,tr("虚拟面板设置"));
   tabWidget->addTab(widget2,tr("实际实验板设置"));
   centerLayout=new QGridLayout;
   centerLayout->addWidget(tabWidget,0,0,8,5);
   centerLayout->addWidget(button[0],8,1,1,1);
   centerLayout->addWidget(button[1],8,3,1,1);
   this->setLayout(centerLayout);
   this->setWindowTitle(tr("面板属性设置"));
   connect(button[0],&QPushButton::clicked,this,&pannelSetting::setNumberofCtl);
   connect(button[1],&QPushButton::clicked,this,&pannelSetting::close);
   connect(comobox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&pannelSetting::currentIndexChanged);


}
void pannelSetting::currentIndexChanged(int index)
{
    bool enable = false;
    switch(index)
    {
    case 1:
    case 2:
        keyDown0->setChecked(true);
        light0->setChecked(true);
        keyDown1->setChecked(false);
        light1->setChecked(false);
        break;
    case 3:
        keyDown0->setChecked(false);
        light0->setChecked(true);
        keyDown1->setChecked(true);
        light1->setChecked(false);
        break;
    default:
        enable = true;
        break;
    }
    keyGroupBox->setEnabled(enable);
    sstubeGroupBox->setEnabled(enable);
}

void pannelSetting::setNumberofCtl()
{
    a=spinBox[0]->value();
    b=spinBox[1]->value();
    c=spinBox[2]->value();
    d=spinBox[3]->value();
    e=spinBox[4]->value();
    testname=nameLineEdit->text();
    testDescribe=describeLineEdit->text();
    currentIndex=comobox->currentIndex();
    keylevel=keyDown1->isChecked();
    sstubeLevel=light1->isChecked();

    QDialog::accept();
}

void pannelSetting::close()
{
    QDialog::reject();

}

pannelSetting::~pannelSetting()
{

}
