#include "dialogConnnect.h"
#include "ui_dialogConnnect.h"

DialogConnnect::DialogConnnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConnnect)
{
    ui->setupUi(this);
    ui->textWarning->setVisible(false);

    jupod = new JupodAdpter;
    connect(ui->button1,&QPushButton::clicked,this,&DialogConnnect::Refresh);
    connect(ui->button2,&QPushButton::clicked,this,&DialogConnnect::Connecting);
    connect(ui->button3,&QPushButton::clicked,this,&DialogConnnect::reject);
    Refresh(true);
}

DialogConnnect::~DialogConnnect()
{
    delete jupod;
    delete ui;
}

void DialogConnnect::setWarningText(QString msg)
{
    ui->textWarning->setText(msg);
    ui->textWarning->setStyleSheet("color:red;");
    ui->textWarning->setVisible(true);
}

void DialogConnnect::Connecting(void)
{
    int error = jupod->Select(ui->deviceList->currentRow());
    if (error==0)
        accept();
}

void DialogConnnect::Refresh(bool silence)
{
    ui->deviceList->clear();
    int error;
    error = jupod->Scan(silence);
    if(error == 0)
    {
        for(int i=0;i<jupod->GetDeviceCount();i++)
        {
            ui->deviceList->addItem(jupod->GetDeviceName(i));
        }
        ui->deviceList->setCurrentRow(0);
    }
}

