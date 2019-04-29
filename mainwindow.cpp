#include "mainwindow.h"
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QString>
#include <QPixmap>
#include <QFileDialog>
#include <QCoreApplication>
#include <QTranslator>
#include <QLocale.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QProcess>
#include <Windows.h>
#include <QHeaderView>
#include "dialogConnnect.h"
#include "about.h"
#include "pannelsetting.h"
#include "jupodAdpter.h"
#include "errorMessage.h"
#include "VirtualPanelData.h"
#include "VirtualComponentData.h"
#include "DialogRealBoard.h"



#include <QDebug>

int wNumber;



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("WeLab");
    setWindowIcon(QIcon(":/images/AppIcon"));

    isEnglish = false;
    Designing=false;
   // isDelete=false;
    isNew=0;
    experimentName="";
    experimentDescribe="";
    imgBaseName="";

    realPanelName=0;
    keyReleasesign=false;
    ssTubeLightsign=false;

    createDock();
    createMenuAndBar();   
    setupModel();
    setupShowModel();
    createCtlTable();
    createShowTable();
    component = new VirtualComponentData;    
//    QString str;
//    QFile file(qApp->applicationDirPath() + "\\Board.json");
//    if(!file.exists()) //如果应用程序文件夹内没有"Board.json"，再看看用户数据文件夹内有没有
//        file.setFileName(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "\\Board.json");
//    if(file.open(QIODevice::ReadOnly))
//    {
//        QByteArray ba = file.readAll();
//        QJsonParseError e;
//        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
//        if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
//        {
//            QJsonObject obj = jsonDoc.object();
//            component->SetRealKeyReleaseLevel(!obj["button press level"].toInt(0));
//            component->SetRealSevenSegmentTubeLightLevel(obj["segment light level"].toInt(0));
//        }
//        file.close();
//    }

    /******************虚拟面板控件图片**************************/
    switch0Icon.load(":/images/switch_0");   //switchoffImage.load(":/images/switch_0");   //
    switch1Icon.load(":/images/switch_1");   //switchonImage.load(":/images/switch_1");    //
   // switchPixmap.load(":/images/switch_1");
    keyupIcon.load(":/images/KEY1");          //keyupIcon.addFile(":/images/key_up");
    keydownIcon.load(":/images/KEY0");      //keydownIcon.addFile(":/images/key_down");     //
    /*本想变形为宽高不一样的，没有成功。
    keyupIcon.scaled(QSize(wKey,hKey),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    keydownIcon.scaled(QSize(wKey,hKey),Qt::IgnoreAspectRatio,Qt::SmoothTransformation);*/
    ledOffImage.load(":/images/led_off");
    greenLedImage.load(":/images/led_green");
    redLedImage.load(":/images/led_red");
    nullPixmap.load(":/images/BG");
    sstubePixmap[0].load(":/images/A1");
    sstubePixmap[1].load(":/images/B1");
    sstubePixmap[2].load(":/images/C1");
    sstubePixmap[3].load(":/images/D1");
    sstubePixmap[4].load(":/images/E1");
    sstubePixmap[5].load(":/images/F1");
    sstubePixmap[6].load(":/images/G1");
    sstubePixmap[7].load(":/images/H1");






    keyFace=NULL;
    switchFace=NULL;
    redLedFace=NULL;
    greenLedFace=NULL;
    switchFaceGroup=NULL;
    keyFaceGroup=NULL;
    sstubeGroup=NULL;
    redLedFaceGroup=NULL;
    greenLedFaceGroup=NULL;

    sstubeViewFace=NULL;
    sstubeViewSence=NULL;
    sstubeButton=NULL;

    errorKeyDown=1;

    showMaximized();
    mainView=new QGraphicsView(this);
   // mainView->setStyleSheet("background-color:gray");
    graphicsView = new QGraphicsView(mainView);
    graphicsView->setStyleSheet("border:0px");
    mainView->setStyleSheet("border:0px");
    //mainView->setAcceptDrops(true);
    setCentralWidget(mainView);
    mainView->show();
    pannel = new VirtualPanelData;

    // 原JULAB程序初始化设置
    JupodAdpter jupod;
    jupod.AutoConnecting();
    SetParentWnd ((HWND)this->winId());

    connect(component,&VirtualComponentData::FPGAStateChanged,this,&MainWindow::refreshVirtualPanel);

    filePath = "../";
}

MainWindow::~MainWindow()
{
    deleteAllFace();
    delete removectl;
    delete pannel;
    delete component;
    delete graphicsView;
    delete toolBar1;
    delete mainView;
    delete ctlSizeLabel;
    delete ctlSizeSlider;
    delete backgroundSizeLabel;
    delete backgroundSizeSlider;
    for(int i=0;i<4;i++)
    {
        delete addctl[i];
    }
    delete setPannelInfo;
    //delete setPannelInfo;
    delete  showModel;
    delete   showctlIfoTable;
    delete switchComboBox;
    delete keyComboBox;
    delete greenLedComboBox;
    delete redLedComboBox;
    delete sstubeComboBox;
    delete ctldocklayout;
    delete dockWidget;
    delete controlDock;
    delete ctlModel;
    delete ctlIfoTable;
    delete ctlTableDock;

}

void MainWindow::deleteAllFace()
{
    if(keyFace)
    {
        for(int i=0;i<NumberOfKey;i++){
            delete keyFace[i];
        }
        delete[] keyFace;
//        keyFace=NULL;
        delete keyFaceGroup;
//        keyFaceGroup=NULL;
    }
    if(switchFace)
    {
        for(int i=0;i<NumberOfSwitch;i++){
            delete switchFace[i];
        }
        delete[] switchFace;
//        switchFace=NULL;
        delete switchFaceGroup;
//        switchFaceGroup=NULL;
    }
    if(redLedFace)
    {
        for(int i=0;i<NumberOfRedLed;i++){
            delete redLedFace[i];
        }
        delete[] redLedFace;
//        redLedFace=NULL;
        delete redLedFaceGroup;
//        redLedFaceGroup=NULL;
    }
    if(greenLedFace)
    {
        for(int i=0;i<NumberOfGreenLed;i++){
            delete greenLedFace[i];
        }
        delete[] greenLedFace;
//        greenLedFace=NULL;
        delete greenLedFaceGroup;
//        greenLedFaceGroup=NULL;
    }
    if(sstubeViewSence)
    {
        for(int i=0;i<NumberOfSSTube;i++){
            delete sstubeViewSence[i];
        }
        delete[] sstubeViewSence;
//        sstubeViewSence=NULL;
    }
    if(sstubeViewFace)
    {
        for(int i=0;i<NumberOfSSTube;i++){
            delete sstubeViewFace[i];
        }
        delete[] sstubeViewFace;
//        sstubeViewFace=NULL;
    }

    if(sstubeButton)
    {
        for(int i=0;i<NumberOfSSTube;i++){
            delete sstubeButton[i];
        }
       delete[] sstubeButton;
//        sstubeButton=NULL;
        delete sstubeGroup;
//        sstubeGroup=NULL;
   }

    keyFace=NULL;
    switchFace=NULL;
    redLedFace=NULL;
    greenLedFace=NULL;
    switchFaceGroup=NULL;
    keyFaceGroup=NULL;
    sstubeGroup=NULL;
    redLedFaceGroup=NULL;
    greenLedFaceGroup=NULL;

    sstubeViewFace=NULL;
    sstubeViewSence=NULL;
    sstubeButton=NULL;
}


//槽函数
void MainWindow::onOpenPanel()
{
    fileName=QFileDialog::getOpenFileName(this,tr("打开虚拟面板文件"),filePath,tr("虚拟面板文件 (*.vp2);;早期虚拟面板文件 (*.vpl)"));//
    if(!(fileName.isEmpty()))
   {
    deleteAllFace();//清除现有控件

    //QString filePath,baseName,imagePathName,suffixName;
    //QFileInfo fi;
    fi=QFileInfo(fileName);
    filePath=fi.path();
    baseName=fi.baseName();
    suffixName=fi.suffix();
    //imagePathName=filePath+"/"+baseName+".bmp";

    if(suffixName=="vp2")
    {
         readVP2(fileName,filePath);

    }
    else if(suffixName=="vpl")
    {
        experimentName="";
        experimentDescribe="";
        readVPL(fileName,filePath,baseName);
    }
    else if(suffixName=="json")
    {
        readVPjson(fileName);
    }
    updateToTable();
    component->SetLabID(experimentDescribe);  //仿真
  }
}

//读取json文件 20180913
void MainWindow::readVPjson(QString fileName){
    QFile vpjson(fileName);
    if(!vpjson.open(QIODevice::ReadOnly))
        {
            qDebug() << "could't open projects json";
            return;
        }
    QByteArray jsonData = vpjson.readAll();
    vpjson.close();
    QJsonParseError json_error;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonData, &json_error));

    if(json_error.error != QJsonParseError::NoError)
       {
            qDebug() << "json error!";
            return;
       }
    QJsonObject rootObj = jsonDoc.object();
    QStringList keys = rootObj.keys();
    for(int i = 0; i < keys.size(); i++)
       {
           qDebug() << "key" << i << " is:" << keys.at(i);
       }
    if(rootObj.contains("name"))
        {
           QJsonObject subObj = rootObj.value("name").toObject();
           experimentName=subObj["name"].toString();
            qDebug() << experimentName;
        }
    if(rootObj.contains("description"))
        {
           QJsonObject subObj = rootObj.value("description").toObject();
           experimentName=subObj["description"].toString();
            qDebug() << experimentName;
        }
}
void MainWindow::readVPL(QString fileName, QString filePath, QString baseName)
{
    realPanelName=0;
    keyReleasesign=false;
    ssTubeLightsign=false;
    component->SetRealKeyReleaseLevel(true);
    component->SetRealSevenSegmentTubeLightLevel(false);
    //baseName=fi.baseName();
    imagePathName=filePath+"/"+baseName+".bmp";
    imgBaseName=baseName+".bmp";
    imgsuffix="bmp";
    pannel->ImportFromFile(fileName);
    if(fileName.isEmpty())
        return;
    setWindowTitle("WeLab");
    scene.clear();
    if(pannel->getHavingBmp())
    {
        myImage.load(imagePathName);
       // scene.addPixmap(myImage.scaled(pannel->getPanelWidth(),pannel->getPanelHeight()));
        if(!(myImage.isNull()))
        {
          scene.addPixmap(myImage.scaled(pannel->getPanelWidth(),pannel->getPanelHeight()));
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("找不到背景图，请将背景图"+baseName+".bmp"+"存放在："+filePath+"路径下后重新打开");
            msgBox.setStandardButtons(QMessageBox::Yes );
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
    graphicsView->setScene(&scene);
    graphicsView->setSceneRect(0,0,pannel->getPanelWidth(),pannel->getPanelHeight());
    graphicsView->resize(pannel->getPanelWidth()+10,pannel->getPanelHeight()+10);
    graphicsView->show();

    settingCtlNumber(SWITCH_CHAIN_LENGTH,KEY_CHAIN_LENGTH,REDLED_CHAIN_LENGTH,GREENLED_CHAIN_LENGTH,SSTube_CHAIN_LENGTH/8);
    createCtl();//创建控件
    settingCtlNumber(18,4,18,9,8);
    for(int i=0;i<NumberOfKey;i++)
    {
        keyFace[i]->setVisible(pannel->getKeyVisible(i));
        keyFace[i]->move(pannel->getKeyX(i),pannel->getKeyY(i));    //setGeometry ,32,32);
    }
    for(int i=0;i<NumberOfSwitch;i++)
    {
        switchFace[i]->setVisible(pannel->getSwitchVisible(i));
        switchFace[i]->move(pannel->getSwitchX(i),pannel->getSwitchY(i));   //setGeometry,20,32);
    }
    for(int i=0;i<NumberOfGreenLed;i++)
    {
        greenLedFace[i]->setVisible(pannel->getGreenLedVisible(i));
        greenLedFace[i]->move(pannel->getGreenLedX(i),pannel->getGreenLedY(i));//setGeometry,16,16);
    }
    for(int i=0;i<NumberOfRedLed;i++)
    {
        redLedFace[i]->setVisible(pannel->getRedLedVisible(i));
        redLedFace[i]->setGeometry(pannel->getRedLedX(i),pannel->getRedLedY(i),wLED,hLED);
    }
    for(int i=0;i<NumberOfSSTube;i++)
    {
        sstubeButton[i]->setVisible(pannel->getSSTubeVisible(i));
        sstubeButton[i]->setGeometry(pannel->getSSTubeX(i),pannel->getSSTubeY(i),wSSTube+2,hSSTube+2);
    }
    settingCtlNumber(SWITCH_CHAIN_LENGTH,KEY_CHAIN_LENGTH,REDLED_CHAIN_LENGTH,GREENLED_CHAIN_LENGTH,SSTube_CHAIN_LENGTH/8);
}
void MainWindow::readVP2(QString fileName, QString filePath)
{
    if(!fileName.isEmpty())
    {
       QFile ctldata(fileName);
       if(ctldata.open(QFile::ReadOnly|QFile::Text))
         {
            QTextStream in(&ctldata);
            QString line;
            ctlModel->removeRows(0,ctlModel->rowCount(QModelIndex()),QModelIndex());
            int row=0;
            do
             {
               line=in.readLine();
               if(!line.isEmpty())
               {
                   ctlModel->insertRows(row,1,QModelIndex());
                   QStringList pieces=line.split(",",QString::KeepEmptyParts); //SkipEmptyParts
                   ctlModel->setData(ctlModel->index(row,0,QModelIndex()),pieces.value(0));
                   ctlModel->setData(ctlModel->index(row,1,QModelIndex()),pieces.value(1));
                   ctlModel->setData(ctlModel->index(row,2,QModelIndex()),pieces.value(2));
                   ctlModel->setData(ctlModel->index(row,3,QModelIndex()),pieces.value(3));
                   ctlModel->setData(ctlModel->index(row,4,QModelIndex()),pieces.value(4));
                   ctlModel->setData(ctlModel->index(row,5,QModelIndex()),pieces.value(5));
                   ctlModel->setData(ctlModel->index(row,6,QModelIndex()),pieces.value(6));
                   ctlModel->setData(ctlModel->index(row,7,QModelIndex()),pieces.value(7));
                   row++;
               }
             }while(!line.isEmpty());
            ctldata.close();
         }
   }
    scene.clear();
    //实验名和描述显示
    QModelIndex name=ctlModel->index(0,0,QModelIndex());
    QModelIndex describe=ctlModel->index(0,4,QModelIndex());
    QModelIndex realPanelindex=ctlModel->index(0,5,QModelIndex());
    QModelIndex keyReleaselevel=ctlModel->index(0,6,QModelIndex());
    QModelIndex ssTubeLightlevel=ctlModel->index(0,7,QModelIndex());
    experimentName=name.data().toString();
    experimentName=experimentName.simplified();
    experimentDescribe=QString("%1").arg(describe.data().toString());
    experimentDescribe=experimentDescribe.simplified();
    realPanelName=realPanelindex.data().toInt();
    keyReleasesign=keyReleaselevel.data().toBool();
    ssTubeLightsign=ssTubeLightlevel.data().toBool();
    component->SetRealKeyReleaseLevel(!keyReleasesign);
    component->SetRealSevenSegmentTubeLightLevel(ssTubeLightsign);
    if (component->GetRealKeyReleaseLevel())
    {
        keyupIcon.load(":/images/KEY1");
        keydownIcon.load(":/images/KEY0");
    }
    else
    {
        keyupIcon.load(":/images/KEY1");
        keydownIcon.load(":/images/KEY0");
    }
   // mainView->setToolTip(name.data().toString()+"\n"+describe.data().toString());
    //graphicsView->setToolTip(name.data().toString()+"\n"+describe.data().toString());
    if(experimentName.isEmpty())
        setWindowTitle("WeLab");
    else
        setWindowTitle("WeLab - "+experimentName);
    // pannelInfo->setText("    "+name.data().toString()+"    ");
    //控件数量设置
    QModelIndex a=ctlModel->index(1,0,QModelIndex());
    QModelIndex b=ctlModel->index(1,1,QModelIndex());
    QModelIndex c=ctlModel->index(1,2,QModelIndex());
    QModelIndex d=ctlModel->index(1,3,QModelIndex());
    QModelIndex e=ctlModel->index(1,4,QModelIndex());
    settingCtlNumber(a.data().toInt(),b.data().toInt(),c.data().toInt(),d.data().toInt(),e.data().toInt());

    //背景设置
    QModelIndex backgroundName=ctlModel->index(0,1,QModelIndex());
    QModelIndex backgroundWidth=ctlModel->index(0,2,QModelIndex());
    QModelIndex backgroundHeight=ctlModel->index(0,3,QModelIndex());
   // baseName=backgroundName.data().toString();
    if(!(backgroundName.data().toString().isEmpty()))//.data().toInt()
    {
        imgBaseName=backgroundName.data().toString();
        imagePathName=filePath+"/"+imgBaseName;
        myImage.load(imagePathName);
        if(!(myImage.isNull()))
        {
          scene.addPixmap(myImage.scaled(backgroundWidth.data().toInt(),backgroundHeight.data().toInt()));
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setText("找不到背景图，请将背景图"+backgroundName.data().toString()+"存放在："+filePath+"路径下后重新打开");
            msgBox.setStandardButtons(QMessageBox::Yes );
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }

    }



    graphicsView->setSceneRect(0,0,backgroundWidth.data().toInt(),backgroundHeight.data().toInt());
    graphicsView->setScene(&scene);
    graphicsView->resize(backgroundWidth.data().toInt()+10,backgroundHeight.data().toInt()+10);
    graphicsView->show();

    //各类控件尺寸
    QModelIndex switchW=ctlModel->index(2,0,QModelIndex());
    QModelIndex switchH=ctlModel->index(2,1,QModelIndex());
    QModelIndex keyW=ctlModel->index(2,2,QModelIndex());
    QModelIndex keyH=ctlModel->index(2,3,QModelIndex());
    QModelIndex ledW=ctlModel->index(2,4,QModelIndex());
    QModelIndex ledH=ctlModel->index(2,5,QModelIndex());
    QModelIndex sstubeW=ctlModel->index(2,6,QModelIndex());
    QModelIndex sstubeH=ctlModel->index(2,7,QModelIndex());
    wSwitch=switchW.data().toInt();hSwitch=switchH.data().toInt();
    wKey=keyW.data().toInt();hKey=keyH.data().toInt();
    wLED=ledW.data().toInt();hLED=ledH.data().toInt();
    wSSTube=sstubeW.data().toInt();hSSTube=sstubeH.data().toInt();


    //各种类控件在表格中的位置
    int switchWhere,keyWhere,redLedWhere,greenLedWhere,sstubeWhere;


    switchWhere=3;
    keyWhere=switchWhere+NumberOfSwitch;
    redLedWhere=keyWhere+NumberOfKey;
    greenLedWhere=redLedWhere+NumberOfRedLed;
    sstubeWhere=greenLedWhere+NumberOfGreenLed;
    createCtl();//创建控件
    for(int i=0;i<NumberOfKey;i++)
    {
        QModelIndex keyVisible=ctlModel->index(i+keyWhere,1,QModelIndex());
        QModelIndex keyX=ctlModel->index(i+keyWhere,2,QModelIndex());
       QModelIndex keyY=ctlModel->index(i+keyWhere,3,QModelIndex());
       keyFace[i]->setVisible(keyVisible.data().toInt());
       keyFace[i]->move(keyX.data().toInt(),keyY.data().toInt());    //setGeometry ,32,32);
    }
    for(int i=0;i<NumberOfSwitch;i++)
    {
       QModelIndex switchVisible=ctlModel->index(i+switchWhere,1,QModelIndex());
       QModelIndex switchX=ctlModel->index(i+switchWhere,2,QModelIndex());
       QModelIndex switchY=ctlModel->index(i+switchWhere,3,QModelIndex());
       switchFace[i]->setVisible(switchVisible.data().toInt());
       switchFace[i]->move(switchX.data().toInt(),switchY.data().toInt());   //setGeometry,20,32);
    }
    for(int i=0;i<NumberOfGreenLed;i++)
    {
        QModelIndex greenLedVisible=ctlModel->index(i+greenLedWhere,1,QModelIndex());
        QModelIndex greenLedX=ctlModel->index(i+greenLedWhere,2,QModelIndex());
        QModelIndex greenLedY=ctlModel->index(i+greenLedWhere,3,QModelIndex());
        greenLedFace[i]->setVisible(greenLedVisible.data().toInt());
        greenLedFace[i]->move(greenLedX.data().toInt(),greenLedY.data().toInt());//setGeometry,16,16);
    }
    for(int i=0;i<NumberOfRedLed;i++)
    {
        QModelIndex redLedVisible=ctlModel->index(i+redLedWhere,1,QModelIndex());
        QModelIndex redLedX=ctlModel->index(i+redLedWhere,2,QModelIndex());
        QModelIndex redLedY=ctlModel->index(i+redLedWhere,3,QModelIndex());
        redLedFace[i]->setVisible(redLedVisible.data().toInt());
        redLedFace[i]->setGeometry(redLedX.data().toInt(),redLedY.data().toInt(),wLED,hLED);
    }
    for(int i=0;i<NumberOfSSTube;i++)
    {
        QModelIndex sstubeVisible=ctlModel->index(i+sstubeWhere,1,QModelIndex());
        QModelIndex sstubeX=ctlModel->index(i+sstubeWhere,2,QModelIndex());
        QModelIndex sstubeY=ctlModel->index(i+sstubeWhere,3,QModelIndex());
        sstubeButton[i]->setVisible(sstubeVisible.data().toInt());
        sstubeButton[i]->setGeometry(sstubeX.data().toInt(),sstubeY.data().toInt(),wSSTube+2,hSSTube+2);
    }

}
void MainWindow::createCtl()
{
    component->PanelChange();

    if (component->GetRealKeyReleaseLevel())
    {
        keyupIcon.load(":/images/KEY1");
        keydownIcon.load(":/images/KEY0");
    }
    else
    {
        keyupIcon.load(":/images/KEY1");
        keydownIcon.load(":/images/KEY0");
    }

    component->SetupKey(NumberOfKey);
    keyFace=new  QPushButton*[NumberOfKey];
    for(int i=0;i<NumberOfKey;i++)
       {//创建按钮
        keyFace[i]=new QPushButton(mainView);
        keyFace[i]->setIcon(keyupIcon);
        keyFace[i]->setVisible(false);
        //keyFace[i]->setToolTip("key"+QString::number(i));
        keyFace[i]->setIconSize(QSize(wKey,hKey));
        keyFace[i]->setFixedSize(wKey,hKey);
       }
    keyFaceGroup=new QButtonGroup;
    for(int i=0;i<NumberOfKey;i++){
        keyFaceGroup->addButton(keyFace[i],i);
    }
    keyFaceGroup->setExclusive(false);
    component->SetupSwitch(NumberOfSwitch);

    switchFace=new  QPushButton*[NumberOfSwitch];
    for(int i=0;i<NumberOfSwitch;i++)//创建开关
    {
        switchFace[i]=new QPushButton(mainView);
        //switchFace[i]->setIcon(switch0Icon);//switchoffImage
        //switchFace[i]->setToolTip("SW"+QString::number(i));
        switchFace[i]->setVisible(false);
        switchFace[i]->setIconSize(QSize(wSwitch,hSwitch));
        switchFace[i]->setFixedSize(wSwitch,hSwitch);
    }
    switchFaceGroup=new QButtonGroup;
    for(int i=0;i<NumberOfSwitch;i++){
        switchFaceGroup->addButton(switchFace[i],i);
    }
    switchFaceGroup->setExclusive(false);//设置是否具有排他性

    component->SetupGreenLED(NumberOfGreenLed);
    greenLedFace=new  QPushButton*[NumberOfGreenLed];
    for(int i=0;i<NumberOfGreenLed;i++)//创建绿灯
    {
        greenLedFace[i]=new QPushButton(mainView);
        greenLedFace[i]->setIcon(ledOffImage);
        //greenLedFace[i]->setToolTip("greenLed"+QString::number(i));
        greenLedFace[i]->setVisible(false);
        greenLedFace[i]->setIconSize(QSize(wLED,hLED));
        greenLedFace[i]->setFixedSize(wLED,hLED);
    }
    greenLedFaceGroup=new QButtonGroup;
    for(int i=0;i<NumberOfGreenLed;i++){
        greenLedFaceGroup->addButton(greenLedFace[i],i);
    }
    greenLedFaceGroup->setExclusive(false);//设置是否具有排他性

    component->SetupRedLED(NumberOfRedLed);
    redLedFace=new  QPushButton*[NumberOfRedLed];
    for(int i=0;i<NumberOfRedLed;i++)//创建红灯
    {
        redLedFace[i]=new QPushButton(mainView);
        redLedFace[i]->setIcon(ledOffImage);
        //redLedFace[i]->setToolTip("redLed"+QString::number(i));
         redLedFace[i]->setVisible(false);
        redLedFace[i]->setIconSize(QSize(wLED,hLED));
        redLedFace[i]->setFixedSize(wLED,hLED);
    }
    redLedFaceGroup=new QButtonGroup;
    for(int i=0;i<NumberOfRedLed;i++){
        redLedFaceGroup->addButton(redLedFace[i],i);
    }
    redLedFaceGroup->setExclusive(false);//设置是否具有排他性

    component->SetupSevenSegmentTube(NumberOfSSTube);
    sstubeButton=new QPushButton*[NumberOfSSTube];
    sstubeViewFace=new QGraphicsView*[NumberOfSSTube];
    sstubeViewSence=new QGraphicsScene*[NumberOfSSTube];

    for(int i=0;i<NumberOfSSTube;i++)//创建七段显示器
    {
        sstubeButton[i]=new QPushButton(mainView);
        sstubeButton[i]->setVisible(false);
        //sstubeButton[i]->setToolTip("sstube"+QString::number(i));
        sstubeButton[i]->resize(wSSTube,hSSTube);
        sstubeViewFace[i]=new QGraphicsView(sstubeButton[i]);
        sstubeViewSence[i]=new QGraphicsScene();
        sstubeViewSence[i]->addPixmap(nullPixmap.scaled(wSSTube,hSSTube));
        sstubeViewFace[i]->setScene( sstubeViewSence[i]);

    }
    sstubeGroup=new QButtonGroup;
    for(int i=0;i<NumberOfSSTube;i++){
        sstubeGroup->addButton(sstubeButton[i],i);
    }
    sstubeGroup->setExclusive(false);//设置是否具有排他性

}


void MainWindow::onSwitchToggle(int index, bool state)
{
    if(state)
        switchFace[index]->setIcon(switch1Icon);// switchonImage
    else
        switchFace[index]->setIcon(switch0Icon);//switchoffImage
}

void MainWindow::onKeyDown(int index)
{
    errorKeyDown = component->KeyPressed(index);
    keyFace[index]->setIcon(keydownIcon);
}

void MainWindow::onKeyUp(int index)
{
    if (errorKeyDown==0)
    {
        component->KeyReleased(index);
        errorKeyDown=1;
    }
    keyFace[index]->setIcon(keyupIcon);
}

void MainWindow::showRedLED(int index, bool state)
{
    if (state)
        redLedFace[index]->setIcon(redLedImage);
    else
        redLedFace[index]->setIcon(ledOffImage);
}

void MainWindow::showGreenLED(int index, bool state)
{
    if (state)
        greenLedFace[index]->setIcon(greenLedImage);
    else
        greenLedFace[index]->setIcon(ledOffImage);
}

void MainWindow::showSSTube(int index, char seg)
{
    //判断是否叠加图元
    sstubeViewSence[index]->clear();
    sstubeViewSence[index]->addPixmap(nullPixmap.scaled(wSSTube,hSSTube, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));//, Qt::IgnoreAspectRatio, Qt::SmoothTransformation
    for(int i=0;i<8;i++)
    {
       if(seg & 1<<i)
       {
           sstubeViewSence[index]->addPixmap(sstubePixmap[i].scaled(wSSTube,hSSTube, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));//, Qt::IgnoreAspectRatio, Qt::SmoothTransformation
       }
    }
}

void  MainWindow::refreshVirtualPanel(void)
{
    int i;
    for (i=0; i<NumberOfRedLed; i++)
    {
        showRedLED(i,component->GetRedLEDValue(i));
    }
    for (i=0; i<NumberOfGreenLed; i++)
    {
        showGreenLED(i,component->GetGreenLEDValue(i));
    }
    for (i=0; i<NumberOfSSTube; i++)
        showSSTube(i, component->GetSevenSegmentTubeValue(i));
}

void MainWindow::createMenuAndBar(void)
{
    actionOpenPanel=new QAction(QIcon(":/images/open"),tr("打开虚拟面板(&O)"),this);//打开虚拟面板
    actionOpenPanel->setShortcut(QKeySequence::Open);
    connect(actionOpenPanel,&QAction::triggered,this,&MainWindow::judgeModel);

    actionSavePanel  = new QAction(tr("保存虚拟面板(&S)"),this);
    actionSavePanel->setShortcut(QKeySequence::Save);
    actionSavePanel->setEnabled(false);    //暂未实现
    connect(actionSavePanel,&QAction::triggered,this,&MainWindow::savePanelInfo);

    actionSavePanelAs  = new QAction(tr("另存虚拟面板为(&A)..."),this);
    actionSavePanelAs->setEnabled(false);    //暂未实现
    connect(actionSavePanelAs,&QAction::triggered,this,&MainWindow::saveAs);

    actionLinkAdapter=new QAction(QIcon(":/images/link"),tr("连接实验板(&N)"),this);//连接
    connect(actionLinkAdapter,&QAction::triggered,this,&MainWindow::showlinkDlg);

    actionFpgaProgram=new  QAction(QIcon(":/images/burn"),tr("下载FPGA(&G)"),this);//FPGA编程器
    connect(actionFpgaProgram,&QAction::triggered,this,&MainWindow::fpgaProgram);

    //actionRealBoardSettings = new QAction(tr("实际实验板设置(&R)"),this);
   // connect(actionRealBoardSettings,&QAction::triggered,this,&MainWindow::onRealBoardSettings);

    actionExit = new QAction(tr("退出(&X)"), this);
    actionExit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(actionExit, &QAction::triggered, this, &MainWindow::close);

    aboutAction=new QAction(tr("版本版权信息(&A)"),this);//关于实验系统
    connect(aboutAction,&QAction::triggered,this,&MainWindow::showAbout);

    debugadapternotes=new QAction(tr("调试适配器信息(&D)"),this);//调试适配器信息
    connect(debugadapternotes,&QAction::triggered,this,&MainWindow::showAdapterInfo);

    testcircuitnotes=new QAction(tr("片上调试器信息(&O)"),this);//实验电路信息
    connect(testcircuitnotes,&QAction::triggered,this,&MainWindow::showFPGAInfo);

    actionChangeLanguage = new QAction(QIcon(":/images/flags"), tr("Change &Language to English"), this);
    //actionChangeLanguage->setCheckable(true);
    connect(actionChangeLanguage, &QAction::triggered,this,&MainWindow::changeLanguage);

    removectl=new QAction(tr("右击删除控件"),this);



    //实验板菜单
    virtualboard=menuBar()->addMenu(tr("实验板(&B)"));
    virtualboard->addAction(actionOpenPanel);
    virtualboard->addAction(actionSavePanel);
    //virtualboard->addAction(actionSavePanelAs);
    virtualboard->addSeparator();
    virtualboard->addAction(actionLinkAdapter);
    virtualboard->addAction(actionFpgaProgram);
    virtualboard->addSeparator();
    //virtualboard->addAction(actionRealBoardSettings);
    virtualboard->addSeparator();
    virtualboard->addAction(actionExit);



    //模式菜单
    menuMode = menuBar()->addMenu(tr("模式(&M)"));
    actionSimulate = new QAction(tr("无板仿真(&S)"),this);
        actionSimulate->setCheckable(true);
        connect(actionSimulate, &QAction::triggered, this, &MainWindow::turnSimulation);
    menuMode->addAction(actionSimulate);
    actionDesign = new QAction(tr("虚拟面板设计(&D)"),this);
        actionDesign->setCheckable(true);
        //actionDesign->setEnabled(false);    //暂未实现
        connect(actionDesign,&QAction::triggered,this,&MainWindow::turnDesign);
    menuMode->addAction(actionDesign);
#if defined(DISABLE_SIMULATION)
    actionSimulate->setEnabled(false);  //关闭仿真菜单
#endif

    //帮助菜单
    help=menuBar()->addMenu(tr("帮助(&H)"));
    help->addAction(actionChangeLanguage);
    help->addAction(debugadapternotes);
    help->addAction(testcircuitnotes);
    help->addAction(aboutAction);

    //右键删除菜单
    ongoingTest=new QMenu(this);
    ongoingTest->setVisible(false);

    toolBar1= new QToolBar(this);
    toolBar1->addAction(actionOpenPanel);
    toolBar1->addAction(actionLinkAdapter);
    toolBar1->addAction(actionFpgaProgram);
    //toolBar1->addWidget(toolBar1);
    toolBar1->setMovable(true);
    this->addToolBar(Qt::LeftToolBarArea,toolBar1);
}

void MainWindow::setLanguage(bool isEnglish)
{
    this->isEnglish = isEnglish;
}


void MainWindow::setRealPanelInfo(int a, bool b, bool c)
{
    realPanelName=a;
    keyReleasesign=b;
    ssTubeLightsign=c;
}
void MainWindow::fpgaProgram()
{

    QString fileName=QFileDialog::getOpenFileName(this,tr("指定FPGA配置文件"),"",tr("FPGA配置文件 (*.rbf)"));
    if (fileName.isEmpty())
        return;
    QByteArray ba = fileName.toLocal8Bit();

    int error = ConfigureFPGA(ba.data());
    if (error)
    {
        if(0 == ErrorMessage::show(error))
            error = ConfigureFPGA(ba.data());
    }
    if (error == 0)
        QMessageBox::information(this, tr("信息"),tr("配置FPGA成功！"));
}


//对话框显示
void MainWindow::showlinkDlg()
{
    DialogConnnect dialog(this);
    dialog.exec();
}

void MainWindow::showAdapterInfo()
{
    ErrorMessage::show(ShowJupodInfo(NULL,true));
//    tsspqxx dialog(this);
//    dialog.exec();
}

void MainWindow::showAbout()
{
    about dialog(this);
    dialog.exec();
}
void MainWindow::showPannelSetting()
{
  pannelSetting dialog(this);
  dialog.comobox->setCurrentIndex(realPanelName);
  dialog.keyDown1->setChecked(keyReleasesign);
  dialog.keyDown0->setChecked(!keyReleasesign);
  dialog.light1->setChecked(ssTubeLightsign);
  dialog.light0->setChecked(!ssTubeLightsign);
  if(dialog.exec()== QDialog::Accepted)
  {
   settingCtlNumber(dialog.a,dialog.b,dialog.c,dialog.d,dialog.e);
   if(!(dialog.testname.isEmpty()))
       {
       experimentName=dialog.testname;
       }
   else
   {
       experimentName="";
   }
   if(!(dialog.testDescribe.isEmpty()))
   {
       experimentDescribe=dialog.testDescribe;
   }
   else
   {
       experimentDescribe="";
   }
   experimentDescribe=experimentDescribe.simplified();
   experimentName=experimentName.simplified();
   realPanelName=0;
   keyReleasesign=false;
   ssTubeLightsign=false;
   setRealPanelInfo(dialog.currentIndex,dialog.keylevel,dialog.sstubeLevel);
   component->SetRealKeyReleaseLevel(dialog.keyReleaseLevel());
   component->SetRealSevenSegmentTubeLightLevel(dialog.sevenSegmentTubeLightLevel());
   deleteAllFace();
   scene.clear();
   removeCombo();
   if(dialog.nameLineEdit->text().isEmpty())
       setWindowTitle("WeLab");
   else
       setWindowTitle("WeLab - "+experimentName);
   ctlModel->removeRows(0,ctlModel->rowCount(QModelIndex()),QModelIndex());
   showModel->removeRows(0,showModel->rowCount(QModelIndex()),QModelIndex());
   createCtl();
   designUIChange();
   designConnect();
   addCombo();
  // mainView->setToolTip(experimentName+"\n"+experimentDescribe);
   //graphicsView->setToolTip(experimentName+"\n"+experimentDescribe);
  }
}

void MainWindow::showFPGAInfo()
{
    ErrorMessage::show(ShowChipInfo());
}

void MainWindow::changeLanguage(void)
{
    QMessageBox msgBox;
    msgBox.setText("这将会关闭程序然后重新打开，现在继续吗？");
    msgBox.setInformativeText("That will close the program and reopen it. Are you sure you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
        return;

    QJsonObject objLanguage;
    QString str;
    if (!isEnglish)
    {
        str = "english";
    }
    else
    {
        str = "chinese";
    }

    objLanguage.insert("language", str);

    QJsonDocument jsonDoc(objLanguage);
    QByteArray ba = jsonDoc.toJson(QJsonDocument::Indented);
    QFile file(qApp->applicationDirPath() + "\\Language.json");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
    {   //应用程序文件夹下以写入方式打开配置文件失败，则写入到应用数据文件夹。
        QString fpath = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        QDir dir;
        if(!dir.exists(fpath))//判断文件夹是否存在
        {
            dir.mkdir(fpath); //创建文件夹。Qt文档上说会自动创建，调试发现不会自动创建。
        }
        file.setFileName(fpath + "\\Language.json");
        if(!file.open(QIODevice::WriteOnly|QIODevice::Append))
        {
            QMessageBox::warning(this,tr("警告"),tr("写入文件失败！"));
            return;
        }
    }
    file.write(ba);
    file.flush();
    file.close();
    this->close();
    QProcess process(this);
    process.startDetached(qApp->applicationName());
    qApp->quit();
}

void MainWindow::turnSimulation(bool checked)
{
    component->setSimulation(checked);
}

// /////////////////// ////设计模式////////////////////////
void MainWindow::turnDesign(bool isDesign)
{
    Designing=isDesign;
    if(isDesign)
    {
       controlDock->setVisible(true);
       if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
       designUIChange();       

       ctlToolConnect();
       actionSavePanel->setEnabled(true);
       actionSavePanelAs->setEnabled(true);
    }
    else
    {
      controlDock->setVisible(false);
      if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
      //{
         // refreshVirtualPanel();
      designUIrecovery();
    //  refreshVirtualPanel();
     // }
      actionSavePanel->setEnabled(false);
      actionSavePanelAs->setEnabled(false);
    }
}


/**********************************************************************************************/
/**********************************************************************************************/
void MainWindow::createDock()
{
    switchComboBox=new QComboBox;
    switchComboBox->addItem(tr("开关"));
    keyComboBox=new QComboBox;
    keyComboBox->addItem(tr("按键"));
    greenLedComboBox=new QComboBox;
    greenLedComboBox->addItem(tr("绿灯"));
    redLedComboBox=new QComboBox;
    redLedComboBox->addItem(tr("红灯"));
    sstubeComboBox=new QComboBox;
    sstubeComboBox->addItem(tr("数码管"));

    setPannelInfo=new QPushButton;
    setPannelInfo->setText(tr("设置虚拟面板"));
    setPannelInfo->resize(60,60);
    dockWidget=new QWidget;
    controlDock=new QDockWidget(this);
    controlDock->setVisible(false);
    controlDock->setFixedWidth(130);
    ctldocklayout=new QGridLayout;
    controlDock->setWindowTitle(tr("设计工具箱"));
    ctlSizeLabel=new QLabel;
    ctlSizeLabel->setText(tr("数码管尺寸大小："));
    ctlSizeSlider=new QSlider(Qt::Horizontal);
    ctlSizeSlider->setMinimum(0);
    ctlSizeSlider->setMaximum(10);
    ctlSizeSlider->setValue(5);
    backgroundSizeLabel=new QLabel;
   // backgroundSizeLabel->setVisible(false);
    backgroundSizeLabel->setText(tr("背景图片大小："));
    backgroundSizeSlider=new QSlider(Qt::Horizontal);
   // backgroundSizeSlider->setVisible(false);
    backgroundSizeSlider->setMinimum(0);
    backgroundSizeSlider->setMaximum(20);
    backgroundSizeSlider->setValue(10);
    for(int i=0;i<4;i++)
    {
    addctl[i]=new QPushButton;
    addctl[i]->resize(60,60);
    }
    addctl[0]->setText(tr("添加/更换背景图"));
    addctl[1]->setText(tr("保存虚拟面板"));
    addctl[2]->setText(tr("新建虚拟面板"));
    addctl[3]->setText(tr("控件信息列表"));
    ctldocklayout->addWidget( addctl[2],0,0,1,1);
    ctldocklayout->addWidget( setPannelInfo,1,0,1,1);
    ctldocklayout->addWidget( addctl[1],2,0,1,1);
    ctldocklayout->addWidget( addctl[3],3,0,1,1);
    ctldocklayout->addWidget( addctl[0],4,0,1,1);
    ctldocklayout->addWidget( switchComboBox,5,0,1,1);
    ctldocklayout->addWidget( keyComboBox,6,0,1,1);
    ctldocklayout->addWidget( redLedComboBox,7,0,1,1);
    ctldocklayout->addWidget(greenLedComboBox,8,0,1,1);
    ctldocklayout->addWidget( sstubeComboBox,9,0,1,1);
    ctldocklayout->addWidget( ctlSizeLabel,10,0,1,1);
    ctldocklayout->addWidget( ctlSizeSlider,11,0,1,1);
    ctldocklayout->addWidget( backgroundSizeLabel,12,0,1,1);
    ctldocklayout->addWidget( backgroundSizeSlider,13,0,1,1);
    ctldocklayout->setAlignment(Qt::AlignTop);
    dockWidget->setLayout(ctldocklayout);
    controlDock->setWidget(dockWidget);
    controlDock->setFeatures(QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    controlDock->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);   
    this->addDockWidget(Qt::LeftDockWidgetArea,controlDock);
}

/**********************************************************************************************/
/**********************************************************************************************/

bool MainWindow::eventFilter(QObject *obj, QEvent *evt)
{
    //判断进入过滤器的是哪个控件
     if(obj==keyFace[wNumber])
     {
         objctl=keyFace[wNumber];
     }
     else if(obj==switchFace[wNumber])
     {
         objctl=switchFace[wNumber];
     }
     else if(obj==sstubeButton[wNumber])
     {
         objctl=sstubeButton[wNumber];
     }
     else if(obj==greenLedFace[wNumber])
     {
         objctl=greenLedFace[wNumber];
     }
     else if(obj==redLedFace[wNumber])
     {
         objctl=redLedFace[wNumber];
     }

    static QPoint lastPnt;
    static bool isHover = false;
    QPalette pal    = objctl->palette();
    QBrush brush = pal.background();
    QColor col      = brush.color();

    if(evt->type() == QEvent::MouseButtonDblClick)
    {

      if(col=="gray")
      {
          objctl->setStyleSheet("QPushButton { background-color:transparent; }");
      }
      else
      {
          objctl->setStyleSheet("QPushButton { background-color:gray; }");
      }
    }
    if(evt->type() == QEvent::MouseButtonPress)
    {

        QMouseEvent* e = static_cast<QMouseEvent*>(evt);
        if(e->buttons()&Qt::LeftButton)
        {
         if(objctl->rect().contains(e->pos()) && //is the mouse is clicking the key
             (e->button() == Qt::LeftButton)) //if the mouse click the right key
          {
             lastPnt = e->pos();
             isHover = true;
          }
        }
        else if(e->buttons()&Qt::RightButton)
        {
           //右击出菜单，显示在鼠标光标位置
           // rightPress->exec(QCursor::pos());

           if(col=="gray")
           {
            removingCtl(objctl);
           }

        }
    }
    else if(evt->type() == QEvent::MouseMove && isHover)
    {
        QMouseEvent* e = static_cast<QMouseEvent*>(evt);
        int dx = e->pos().x() - lastPnt.x();
        int dy=e->pos().y()-lastPnt.y();
        objctl->move(objctl->x()+dx,objctl->y()+dy);

    }else if(evt->type() == QEvent::MouseButtonRelease && isHover)
    {

        isHover = false;
        updateToTable();

    }
    else if(evt->type() == QEvent::Leave)
    {
        //wgq 20180908 优化了删除逻辑，鼠标移开后自动变成未选中状态；
       objctl->setStyleSheet("QPushButton { background-color:transparent; }");
    }

    return false;
}
void MainWindow::returnKey(int index)
{
    wNumber=index;
}
void MainWindow::judgeModel()//先开设计模式，在再打开面板的情况
{
    onOpenPanel();
    if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
    {
    if(Designing)
    {     
     designUIChange();
    }
    else
    {

     designUIrecovery();
    // experimentConnect();

    }
    removeCombo();
    addCombo();
    }
}
void MainWindow::designUIChange()
{
    for(int i=0;i<NumberOfKey;i++)
    {
        keyFace[i]->setToolTip("KEY"+QString::number(i)+",双击选中后右击删除");
        keyFace[i]->installEventFilter(this);
        //keyFace[i]->setStyleSheet("QPushButton { background-color:gray; }");
    }
    for(int i=0;i<NumberOfSwitch;i++)
    {
        switchFace[i]->setToolTip("SW"+QString::number(i)+",双击选中后右击删除");
        switchFace[i]->installEventFilter(this);
        switchFace[i]->setCheckable(false);
         switchFace[i]->setIcon(switch1Icon);
        switchFace[i]->setFlat(false);
       // switchFace[i]->setStyleSheet("QPushButton { background-color:gray; }");
    }
    for(int i=0;i<NumberOfGreenLed;i++)
    {
        greenLedFace[i]->setToolTip("LEDG"+QString::number(i)+",双击选中后右击删除");
        greenLedFace[i]->installEventFilter(this);
        greenLedFace[i]->setFlat(false);
        greenLedFace[i]->setIcon(greenLedImage);
        //greenLedFace[i]->setStyleSheet("background: black;");
    }
    for(int i=0;i<NumberOfRedLed;i++)
    {
        redLedFace[i]->setToolTip("LEDR"+QString::number(i)+",双击选中后右击删除");
        redLedFace[i]->installEventFilter(this);
        redLedFace[i]->setFlat(false);
        redLedFace[i]->setIcon(redLedImage);
        //redLedFace[i]->setStyleSheet("background: black;");
    }
    for(int i=0;i<NumberOfSSTube;i++)
    {
        sstubeButton[i]->setToolTip("LEDSD"+QString::number(i)+",双击选中后右击删除");
        sstubeButton[i]->installEventFilter(this);
        //sstubeButton[i]->setStyleSheet("QPushButton { background-color:gray; }");
        sstubeViewFace[i]->setVisible(false);
        sstubeButton[i]->setIcon(nullPixmap);
        sstubeButton[i]->setIconSize(QSize(wSSTube,hSSTube));
    }
     designConnect();
}
void MainWindow::designUIrecovery()
{

    for(int i=0;i<NumberOfKey;i++)
    {
       keyFace[i]->setToolTip("KEY"+QString::number(i));
       keyFace[i]->removeEventFilter(this);
       keyFace[i]->setStyleSheet("QPushButton { background-color:transparent; }");
    }
    for(int i=0;i<NumberOfSwitch;i++)
    {
        switchFace[i]->setToolTip("SW"+QString::number(i));
        switchFace[i]->removeEventFilter(this);
        switchFace[i]->setCheckable(true);
        switchFace[i]->setFlat(true);
        switchFace[i]->setIcon(switch0Icon);
        switchFace[i]->setStyleSheet("QPushButton { background-color:transparent; }");
    }
    for(int i=0;i<NumberOfGreenLed;i++)
    {
        greenLedFace[i]->setToolTip("LEDG"+QString::number(i));
        greenLedFace[i]->removeEventFilter(this);
        greenLedFace[i]->setFlat(true);
        greenLedFace[i]->setIcon(ledOffImage);
        greenLedFace[i]->setStyleSheet("QPushButton { background-color:transparent; }");
    }
    for(int i=0;i<NumberOfRedLed;i++)
    {
        redLedFace[i]->setToolTip("LEDR"+QString::number(i));
        redLedFace[i]->removeEventFilter(this);
        redLedFace[i]->setFlat(true);
        redLedFace[i]->setIcon(ledOffImage);
        redLedFace[i]->setStyleSheet("QPushButton { background-color:transparent; }");
    }
    for(int i=0;i<NumberOfSSTube;i++)
    {
        sstubeButton[i]->setToolTip("LEDSD"+QString::number(i));
        sstubeButton[i]->removeEventFilter(this);
        // sstubeButton[i]->setIconSize(QSize(0,0));
       // sstubeButton[i]->setStyleSheet("QPushButton { background-color:transparent; }");
        //sstubeButton[i]->setIcon("");
        sstubeViewFace[i]->setVisible(true);
        sstubeViewFace[i]->setSceneRect(0,0,wSSTube,hSSTube);
        sstubeViewFace[i]->resize(wSSTube,hSSTube);
        sstubeViewSence[i]->addPixmap(nullPixmap.scaled(wSSTube,hSSTube, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        //sstubeViewFace[i]->setSceneRect(0,0,wSSTube,hSSTube);
    }
    experimentConnect();
}
void MainWindow::designConnect()
{
    connect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    connect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    connect(sstubeGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    connect(greenLedFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    connect(redLedFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);    
    disconnect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::onKeyDown);
    disconnect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),this,&MainWindow::onKeyUp);
    disconnect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled),this,&MainWindow::onSwitchToggle);
    disconnect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled),component,&VirtualComponentData::SwitchChange);
}
void MainWindow::experimentConnect()
{
    disconnect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    disconnect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    disconnect(sstubeGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    disconnect(greenLedFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    disconnect(redLedFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::returnKey);
    connect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonPressed),this,&MainWindow::onKeyDown);
    connect(keyFaceGroup,static_cast<void (QButtonGroup::*)(int)>(&QButtonGroup::buttonReleased),this,&MainWindow::onKeyUp);
    connect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled),this,&MainWindow::onSwitchToggle);
    connect(switchFaceGroup,static_cast<void (QButtonGroup::*)(int,bool)>(&QButtonGroup::buttonToggled),component,&VirtualComponentData::SwitchChange);
    disconnect(switchComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneSwitch);
    disconnect(keyComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneKey);
    disconnect(redLedComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneRedLed);
    disconnect(greenLedComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneGreenled);
    disconnect(sstubeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneSStube);
    disconnect(addctl[0],&QPushButton::clicked,this,&MainWindow::addBackground);
    disconnect(addctl[1],&QPushButton::clicked,this,&MainWindow::savePanelInfo);
    disconnect(addctl[2],&QPushButton::clicked,this,&MainWindow::createNewPanel);
    disconnect(addctl[3],&QPushButton::clicked,this,&MainWindow::showTable);
    disconnect(setPannelInfo,&QPushButton::clicked,this,&MainWindow::showPanelInfo);
    disconnect(backgroundSizeSlider,static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),this,&MainWindow::changeBackgroundSize);
    disconnect(ctlSizeSlider,static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),this,&MainWindow::changeCTLSize);
}
void MainWindow::ctlToolConnect()
{
    connect(switchComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneSwitch);
    connect(keyComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneKey);
    connect(redLedComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneRedLed);
    connect(greenLedComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneGreenled);
    connect(sstubeComboBox,static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),this,&MainWindow::addoneSStube);
    connect(addctl[0],&QPushButton::clicked,this,&MainWindow::addBackground);
    connect(addctl[1],&QPushButton::clicked,this,&MainWindow::savePanelInfo);
    connect(addctl[2],&QPushButton::clicked,this,&MainWindow::createNewPanel);
    connect(addctl[3],&QPushButton::clicked,this,&MainWindow::showTable);
    connect(setPannelInfo,&QPushButton::clicked,this,&MainWindow::showPanelInfo);
    connect(backgroundSizeSlider,static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),this,&MainWindow::changeBackgroundSize);
    connect(ctlSizeSlider,static_cast<void (QSlider::*)(int)>(&QSlider::valueChanged),this,&MainWindow::changeCTLSize);
}

void MainWindow::savePanelInfo()
{
    if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
    {
        QString fiName=QFileDialog::getSaveFileName(this,tr("保存虚拟面板文件"),"",tr("虚拟面板文件 (*.vp2)"));
        fi=QFileInfo(fiName);
        filePath=fi.path();
        QFile data(fiName);
        QMultiMap<QPushButton*,QString> switchMap[NumberOfSwitch];
        QMultiMap<QPushButton*,QString> keyMap[NumberOfKey];
        QMultiMap<QPushButton*,QString> redLedMap[NumberOfRedLed];
        QMultiMap<QPushButton*,QString> greenLedMap[NumberOfGreenLed];
        QMultiMap<QPushButton*,QString> sstubeMap[NumberOfSSTube];
        for(int i=0;i<NumberOfSwitch;i++)
        {
            createContainer("sw",switchMap[i],switchFace[i],i);
        }
        for(int i=0;i<NumberOfKey;i++)
        {
            createContainer("key",keyMap[i],keyFace[i],i);
        }
        for(int i=0;i<NumberOfRedLed;i++)
        {
            createContainer("redLed",redLedMap[i],redLedFace[i],i);
        }
        for(int i=0;i<NumberOfGreenLed;i++)
        {
            createContainer("greenLed",greenLedMap[i],greenLedFace[i],i);
        }
        for(int i=0;i<NumberOfSSTube;i++)
        {
            createContainer("sstube",sstubeMap[i],sstubeButton[i],i);
        }
        if(data.open(QFile::ReadWrite|QFile::Truncate|QIODevice::Text))
        {
           QTextStream out(&data);
           //if(baseName.isEmpty())
              //baseName="";
           out<<experimentName<<","<<imgBaseName<<","<<myImage.width()<<","<<myImage.height()<<","<<experimentDescribe<<",";
           out<<realPanelName<<","<<(int)keyReleasesign<<","<<(int)ssTubeLightsign<<endl;
           out<<QString::number(NumberOfSwitch)<<",";
           out<<QString::number(NumberOfKey)<<",";
           out<<QString::number(NumberOfRedLed)<<",";
           out<<QString::number(NumberOfGreenLed)<<",";
           out<<QString::number(NumberOfSSTube)<<endl;
           out<<wSwitch<<","<<hSwitch<<",";
           out<<wKey<<","<<hKey<<",";
           out<<wLED<<","<<hLED<<",";
           out<<wSSTube<<","<<hSSTube<<endl;
           for(int i=0;i<NumberOfSwitch;i++)
             saveing(out,switchFace[i],switchMap[i]);
           for(int i=0;i<NumberOfKey;i++)
             saveing(out,keyFace[i],keyMap[i]);
           for(int i=0;i<NumberOfRedLed;i++)
             saveing(out,redLedFace[i],redLedMap[i]);
           for(int i=0;i<NumberOfGreenLed;i++)
             saveing(out,greenLedFace[i],greenLedMap[i]);
           for(int i=0;i<NumberOfSSTube;i++)
             saveing(out,sstubeButton[i],sstubeMap[i]);
        }
     }
}

void MainWindow::createContainer(QString name,QMultiMap<QPushButton *, QString> &mapI, QPushButton *buttonI, int index)
{
    //mapI.insert(buttonI,QString::number(buttonI->height()));
    //mapI.insert(buttonI,QString::number(buttonI->width()));
    mapI.insert(buttonI,QString::number(index));
    mapI.insert(buttonI,QString::number(buttonI->y()));
    mapI.insert(buttonI,QString::number(buttonI->x()));
    mapI.insert(buttonI,QString::number(buttonI->isVisible()));
    mapI.insert(buttonI,name+QString::number(index));   
}
void MainWindow::saveing(QTextStream &out,QPushButton *btn,QMultiMap<QPushButton *, QString> &mapI)
{
    QList<QString> rList;
    rList.clear();
    rList=mapI.values(btn);
      for(int i=0;i<rList.size();i++)
       {
         out<<left<<rList.at(i)<<",";
       }
      out<<endl;
}
void MainWindow::updateToTable()
{
    showModel->removeRows(0,showModel->rowCount(QModelIndex()),QModelIndex());
    int lineNumber;
    lineNumber=0;
    for(int i=0;i<NumberOfSwitch;i++)
    {
        if(switchFace[i]->isVisible())
        {
            showModel->insertRows(lineNumber,1,QModelIndex());
            showModel->setData(showModel->index(lineNumber,0,QModelIndex()),"SW"+QString::number(i));
            showModel->setData(showModel->index(lineNumber,1,QModelIndex()),QString::number(switchFace[i]->isVisible()));
            showModel->setData(showModel->index(lineNumber,2,QModelIndex()),QString::number(switchFace[i]->x()));
            showModel->setData(showModel->index(lineNumber,3,QModelIndex()),QString::number(switchFace[i]->y()));
            showModel->setData(showModel->index(lineNumber,4,QModelIndex()),QString::number(i));
            lineNumber++;
        }
    }
    for(int i=0;i<NumberOfKey;i++)
    {
        if(keyFace[i]->isVisible())
        {
            showModel->insertRows(lineNumber,1,QModelIndex());
            showModel->setData(showModel->index(lineNumber,0,QModelIndex()),"KEY"+QString::number(i));
            showModel->setData(showModel->index(lineNumber,1,QModelIndex()),QString::number(keyFace[i]->isVisible()));
            showModel->setData(showModel->index(lineNumber,2,QModelIndex()),QString::number(keyFace[i]->x()));
            showModel->setData(showModel->index(lineNumber,3,QModelIndex()),QString::number(keyFace[i]->y()));
            showModel->setData(showModel->index(lineNumber,4,QModelIndex()),QString::number(i));
            lineNumber++;
        }
    }
    for(int i=0;i<NumberOfRedLed;i++)
    {
        if(redLedFace[i]->isVisible())
        {
            showModel->insertRows(lineNumber,1,QModelIndex());
            showModel->setData(showModel->index(lineNumber,0,QModelIndex()),"LEDR"+QString::number(i));
            showModel->setData(showModel->index(lineNumber,1,QModelIndex()),QString::number(redLedFace[i]->isVisible()));
            showModel->setData(showModel->index(lineNumber,2,QModelIndex()),QString::number(redLedFace[i]->x()));
            showModel->setData(showModel->index(lineNumber,3,QModelIndex()),QString::number(redLedFace[i]->y()));
            showModel->setData(showModel->index(lineNumber,4,QModelIndex()),QString::number(i));
            lineNumber++;
        }
    }
    for(int i=0;i<NumberOfGreenLed;i++)
    {
        if(greenLedFace[i]->isVisible())
        {
            showModel->insertRows(lineNumber,1,QModelIndex());
            showModel->setData(showModel->index(lineNumber,0,QModelIndex()),"LEDG"+QString::number(i));
            showModel->setData(showModel->index(lineNumber,1,QModelIndex()),QString::number(greenLedFace[i]->isVisible()));
            showModel->setData(showModel->index(lineNumber,2,QModelIndex()),QString::number(greenLedFace[i]->x()));
            showModel->setData(showModel->index(lineNumber,3,QModelIndex()),QString::number(greenLedFace[i]->y()));
            showModel->setData(showModel->index(lineNumber,4,QModelIndex()),QString::number(i));
            lineNumber++;
        }
    }
    for(int i=0;i<NumberOfSSTube;i++)
    {
        if(sstubeButton[i]->isVisible())
        {
            showModel->insertRows(lineNumber,1,QModelIndex());
            showModel->setData(showModel->index(lineNumber,0,QModelIndex()),"LEDSD"+QString::number(i));
            showModel->setData(showModel->index(lineNumber,1,QModelIndex()),QString::number(sstubeButton[i]->isVisible()));
            showModel->setData(showModel->index(lineNumber,2,QModelIndex()),QString::number(sstubeButton[i]->x()));
            showModel->setData(showModel->index(lineNumber,3,QModelIndex()),QString::number(sstubeButton[i]->y()));
            showModel->setData(showModel->index(lineNumber,4,QModelIndex()),QString::number(i));
            lineNumber++;
        }
    }
}

void MainWindow::createNewPanel()
{
    QMessageBox msgBox;
    msgBox.setText("新建后会清除当前界面，继续吗");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    if (ret == QMessageBox::No)
        return;

    showPannelSetting();
    component->SetLabID(experimentDescribe);  //仿真
}
void MainWindow::removingCtl(QWidget *useLessCtl)
{
    useLessCtl->setVisible(false);
}
void MainWindow::addoneSwitch(int index)
{
   //添加开关
    if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
    {      
        --index;
        if(index>=0)
        {
        switchFace[index]->setVisible(true);
        switchFace[index]->move(0,0);
        updateToTable();
        }
    }
    designConnect();


}
void MainWindow::addoneKey(int index)//单击添加按键
{  
     if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
     {
          --index;
         if(index>=0)
         {
          keyFace[index]->setVisible(true);
          keyFace[index]->move(0,0);
          updateToTable();
         }
     }
     designConnect();
}
void MainWindow::addoneRedLed(int index)
{
     if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
     {
         --index;
         if(index>=0)
         {
          redLedFace[index]->setVisible(true);
          redLedFace[index]->move(0,0);
          updateToTable();
         }
     }
     designConnect();
}
void MainWindow::addoneSStube(int index)
{
     if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
     {
        --index;
         if(index>=0)
          {
          sstubeButton[index]->setVisible(true);
          sstubeButton[index]->move(0,0);
          updateToTable();
         }
     }
     designConnect();
}
void MainWindow::addoneGreenled(int index)
{
     if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
     {
         --index;
         if(index>=0)
         {
          greenLedFace[index]->setVisible(true);
          greenLedFace[index]->move(0,0);
          updateToTable();
         }
     }
     designConnect();
}
void MainWindow::addBackground()
{
    scene.clear();
    QString imageName=QFileDialog::getOpenFileName(this,tr("选择背景图片"),"",tr("背景图片 (*.bmp *.jpg *.png *.gif)"));
    QFileInfo newFi=QFileInfo(imageName);;
    //baseName=newFi.baseName();
    imgsuffix=newFi.suffix();
    imgBaseName=newFi.baseName()+"."+imgsuffix;
    myImage.load(imageName);
    scene.addPixmap(myImage);
    graphicsView->setScene(&scene);
    graphicsView->setSceneRect(0,0,myImage.width(),myImage.height());
    graphicsView->resize(myImage.width()+10,myImage.height()+10);
    //graphicsView->setFixedSize(myImage.width()+10,myImage.height()+10);
    graphicsView->show();

}
void MainWindow::changeBackgroundSize(int index)//改变背景尺寸
{
   float x,y,z;
   z=((float)index)/10;
   x=myImage.width();
   y=myImage.height();
   scene.clear();
   scene.addPixmap(myImage.scaled(x*z,x*z, Qt::KeepAspectRatio, Qt::SmoothTransformation));
   graphicsView->setSceneRect(0,0,x*z,y*z);
   graphicsView->resize(x*z+10,y*z+10);
   graphicsView->setScene(&scene);
   graphicsView->show();

}
void MainWindow::changeCTLSize(int index)//改变控件尺寸
{
    float x,y,z;
    z=((float)index)/10;
    x=43;
    y=67;
     if(sstubeButton)
     {
         for(int i=0;i<NumberOfSSTube;i++)
         {
         sstubeViewSence[i]->clear();
         sstubeButton[i]->resize(x*z,y*z);
         sstubeButton[i]->setIcon(nullPixmap);
         sstubeButton[i]->setIconSize(QSize(x*z,y*z));
         }

     }
     wSSTube=x*z;
     hSSTube=y*z;
}
void MainWindow::createCtlTable()
{  
   ctlIfoTable=new QTableView;
   ctlIfoTable->horizontalHeader()->setStretchLastSection(true);
   ctlIfoTable->setModel(ctlModel);

}
void MainWindow::createShowTable()
{
    ctlTableDock=new QDockWidget(this);
    ctlTableDock->setWindowTitle(tr("控件信息"));
    ctlTableDock->setVisible(false);
    showctlIfoTable=new QTableView;
    showctlIfoTable->horizontalHeader()->setStretchLastSection(true);
    showctlIfoTable->setModel(showModel);
    showctlIfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //showctlIfoTable->setSelectionMode ( QAbstractItemView::SingleSelection);
    showctlIfoTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ctlTableDock->setWidget(showctlIfoTable);
    ctlTableDock->setAllowedAreas(Qt::AllDockWidgetAreas);//TopDockWidgetArea|Qt::BottomDockWidgetArea
    this->addDockWidget(Qt::BottomDockWidgetArea,ctlTableDock);
}
void MainWindow::showTable()
{
      ctlTableDock->setVisible(true);

}
void MainWindow::setupModel()
{
    ctlModel=new QStandardItemModel(6,8,this);
    ctlModel->setHeaderData(0,Qt::Horizontal,tr("种类"));
    ctlModel->setHeaderData(1,Qt::Horizontal,tr("显示"));
    ctlModel->setHeaderData(2,Qt::Horizontal,tr("X坐标"));
    ctlModel->setHeaderData(3,Qt::Horizontal,tr("Y坐标"));
    ctlModel->setHeaderData(4,Qt::Horizontal,tr("排序"));
    ctlModel->setHeaderData(5,Qt::Horizontal,tr("长度"));
    ctlModel->setHeaderData(6,Qt::Horizontal,tr("宽度"));
    ctlModel->setHeaderData(7,Qt::Horizontal,tr("横竖"));
}

void MainWindow::setupShowModel()
{
    showModel=new QStandardItemModel(1,5,this);
    showModel->setHeaderData(0,Qt::Horizontal,tr("种类"));
    showModel->setHeaderData(1,Qt::Horizontal,tr("显示"));
    showModel->setHeaderData(2,Qt::Horizontal,tr("X坐标"));
    showModel->setHeaderData(3,Qt::Horizontal,tr("Y坐标"));
    showModel->setHeaderData(4,Qt::Horizontal,tr("排序"));
}
void MainWindow::saveAs() //文件另存为，暂时作为新版文件的打开函数
{
  savePanelInfo();
}
void MainWindow::settingCtlNumber(int a, int b, int c, int d, int e)
{
    NumberOfSwitch = a;
    NumberOfKey=b;
    NumberOfRedLed=c;
    NumberOfGreenLed=d;
    NumberOfSSTube=e;
}
void MainWindow::addCombo()
{
  for(int i=0;i<NumberOfSwitch;i++)
  {
      switchComboBox->addItem(tr("SW")+QString::number(i));
  }
  for(int i=0;i<NumberOfKey;i++)
  {
      keyComboBox->addItem(tr("KEY")+QString::number(i));
  }
  for(int i=0;i<NumberOfRedLed;i++)
  {
      redLedComboBox->addItem(tr("LEDR")+QString::number(i));
  }
  for(int i=0;i<NumberOfGreenLed;i++)
  {
      greenLedComboBox->addItem(tr("LEDG")+QString::number(i));
  }
  for(int i=0;i<NumberOfSSTube;i++)
  {
      sstubeComboBox->addItem(tr("LEDSD")+QString::number(i));
  }

}
void MainWindow::removeCombo()
{    
      switchComboBox->clear();
      keyComboBox->clear();
      redLedComboBox->clear();
      greenLedComboBox->clear();
      sstubeComboBox->clear();
      switchComboBox->addItem(tr("开关"));
      keyComboBox->addItem(tr("按键"));
      redLedComboBox->addItem(tr("红灯"));
      greenLedComboBox->addItem(tr("绿灯"));
      sstubeComboBox->addItem(tr("数码管"));
}
void MainWindow::showPanelInfo()
{
    if(keyFace||switchFace||greenLedFace||redLedFace||sstubeButton)
    {
        pannelSetting dialog(this);
        dialog.comobox->setCurrentIndex(realPanelName);
        dialog.keyDown1->setChecked(keyReleasesign);
        dialog.keyDown0->setChecked(!keyReleasesign);
        dialog.light1->setChecked(ssTubeLightsign);
        dialog.light0->setChecked(!ssTubeLightsign);
        dialog.spinBox[0]->setValue(NumberOfSwitch);
        dialog.spinBox[1]->setValue(NumberOfKey);
        dialog.spinBox[2]->setValue(NumberOfRedLed);
        dialog.spinBox[3]->setValue(NumberOfGreenLed);
        dialog.spinBox[4]->setValue(NumberOfSSTube);
        for(int i=0;i<5;i++)
            {
               dialog.spinBox[i]->setEnabled(false);
            }
        dialog.nameLineEdit->setText(experimentName);
        dialog.describeLineEdit->setText(experimentDescribe);
        if(dialog.exec()== QDialog::Accepted)
            {
            if(!(dialog.testname.isEmpty()))
                {
                experimentName=dialog.testname;
                }
            else
            {
                experimentName="";
            }
            if(!(dialog.testDescribe.isEmpty()))
            {
                experimentDescribe=dialog.testDescribe;
            }
            else
            {
                experimentDescribe="";
            }
            experimentDescribe=experimentDescribe.simplified();
            experimentName=experimentName.simplified();
             setRealPanelInfo(dialog.currentIndex,dialog.keylevel,dialog.sstubeLevel);
             component->SetRealKeyReleaseLevel(dialog.keyReleaseLevel());
             component->SetRealSevenSegmentTubeLightLevel(dialog.sevenSegmentTubeLightLevel());
            }
        if(dialog.nameLineEdit->text().isEmpty())
            setWindowTitle("WeLab");
        else
            setWindowTitle("WeLab - "+experimentName);
        // mainView->setToolTip(experimentName+"\n"+experimentDescribe);
        component->SetLabID(experimentDescribe);  //仿真
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("你还没有打开或创建虚拟面板");
        msgBox.setStandardButtons(QMessageBox::Yes );
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
}

///////待用 拖拽效果
/*
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        {
            drag = new QDrag(this);
            QMimeData *data = new QMimeData;
            drag->setMimeData(data);
            drag->setPixmap(switch1Icon);
            drag->exec(Qt::MoveAction);
        }
}
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == mainView)
        {
            event->acceptProposedAction();
        }
}
void MainWindow::dropEvent(QDropEvent *event)
{
    event->accept();
   // keyFace[0]->setVisible(true);
   // keyFace[0]->move(QCursor::pos());
}
*/
//void MainWindow::createPanelInfoDock()
//{
//    QWidget *pannel=new QWidget(this);
//  QDockWidget  *panelInfoDock=new QDockWidget;
//  QLabel *name=new QLabel(tr("名称"));
//  QLabel *identify=new QLabel(tr("标识"));
//  QLabel *ctlInfo=new QLabel;
//  ctlInfo->setText("开关："+QString::number(NumberOfSwitch)+"按键："+QString::number(NumberOfKey)+"红灯："+QString::number(NumberOfRedLed)+"绿灯："+QString::number(NumberOfGreenLed)+"数码管："+QString::number(NumberOfSSTube));
//  QPushButton *modifyName=new QPushButton(tr("修改"));
//  QPushButton *modifyidentify=new QPushButton(tr("修改"));
//  QLineEdit *nameLineEdit=new QLineEdit;
//  QLineEdit *describeLineEdit=new QLineEdit;
//  QGridLayout *layout=new QGridLayout;
//  layout->addWidget(name,0,0,1,1);
//  layout->addWidget(nameLineEdit,0,1,1,3);
//  layout->addWidget(identify,1,0,1,1);
//  layout->addWidget(describeLineEdit,1,0,1,3);
//  layout->addWidget(modifyName,0,4,1,1);
//  layout->addWidget(modifyidentify,1,4,1,1);
//  layout->addWidget(ctlInfo,2,0,1,5);
//  pannel->setLayout(layout);
//  panelInfoDock->setWidget(pannel);
//  panelInfoDock->setAllowedAreas(Qt::AllDockWidgetAreas);//TopDockWidgetArea|Qt::BottomDockWidgetArea
//  this->addDockWidget(Qt::BottomDockWidgetArea,panelInfoDock);

//}
//MainWindow::~createPanelInfoDock()
//{

//}
