#ifndef PANNELSETTING_H
#define PANNELSETTING_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QSpinBox>
#include <QLayout>
#include <QLineEdit>
#include <QTabWidget>
#include <QRadioButton>
#include <QComboBox>
#include "mainwindow.h"
//#include "VirtualComponentData.h"

class pannelSetting : public QDialog
{
    Q_OBJECT
public:
    explicit pannelSetting(QWidget *parent=0);
    ~pannelSetting();
    //VirtualComponentData *component;
    //const VirtualComponentData * getComponent(void)const {return component;}
    bool keyReleaseLevel(void)const {return keyDown0->isChecked();}
    bool sevenSegmentTubeLightLevel(void)const {return light1->isChecked();}

    QLabel *label[5],*nameLabel,*describeLabel,*panelName,*remind  ;
    QPushButton *button[2];
    QSpinBox *spinBox[5];
    QGroupBox *groupBox,*keyGroupBox,*sstubeGroupBox;
    QGridLayout *layout,*mainLayout,*layout2,*layout3,*realPanelLayout,*centerLayout;

    QLineEdit *nameLineEdit,*describeLineEdit;
    QPushButton *realBoardSetting;
    QTabWidget *tabWidget;
    QRadioButton *keyDown0,*keyDown1,*light0,*light1;
    QComboBox *comobox;
    QWidget *widget1,*widget2;
    //MainWindow *mainwindow;



    int a,b,c,d,e;
    int currentIndex;
    bool keylevel,sstubeLevel;
    QString testname,testDescribe;

    void setNumberofCtl();
    void close();
    void currentIndexChanged(int index);
    void realPanelSetting();
    void RealBoardSettings();
signals:
     //void sendData(int a,int b,int c,int d,int e);
     // void sendJudge(int judge);
     //void sendName(QString testName);
     //void sendDescribe(QString testDescribe);
   // void sendRealBoardSettings(int a,bool b,bool c);


};

#endif // PANNELSETTING_H
