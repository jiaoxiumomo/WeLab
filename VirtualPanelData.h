#ifndef VIRTUALPANELDATA_H
#define VIRTUALPANELDATA_H

#include <QObject>
#include <QStandardItemModel>


//虚拟实验板控件位置等信息结构体
struct VirtualCtrl
{
    bool isVisible;
    bool isName;
    bool isComment;
    int  x;
    int  y;
};

//虚拟实验板面板文件结构体
struct VirtualLabFile
{
    VirtualCtrl GreenLeds[9];
    VirtualCtrl RedLeds[18];
    VirtualCtrl Switchs[18];
    VirtualCtrl HexTubes[8];
    VirtualCtrl Keys[4];
    int iBmpWidth;
    int iBmpHeight;
    bool bHavingBmp;
};


class VirtualPanelData : public QObject
{
    Q_OBJECT
protected:

    VirtualLabFile m_VLF;
//    int NumberOfSwitch;
//    int NumberOfKey;
//    int NumberOfRedLED;
//    int NumberOfGreenLED;
//    int NumberOfSSTube;

public:
    explicit VirtualPanelData(QObject *parent = nullptr);
    void ImportFromFile(QString filename);



    //获得开关信息
    //inline int  getNumberOfSwitch() {return NumberOfSwitch;}
    //inline void writeNumberOfSwitch(int index) { NumberOfSwitch=index;}
    inline bool getSwitchVisible(int index)const {return m_VLF.Switchs[index].isVisible;}
    inline int  getSwitchX(int index)const {return m_VLF.Switchs[index].x;}
    inline int  getSwitchY(int index)const {return m_VLF.Switchs[index].y;}
    //获得绿灯信息
    //inline int  getNumberOfGreenLed(){return NumberOfGreenLED;}
    //inline void writeNumberOfGreenLed(int index){ NumberOfGreenLED=index;}
    inline bool getGreenLedVisible(int index)const {return m_VLF.GreenLeds[index].isVisible;}
    inline int  getGreenLedX(int index)const {return m_VLF.GreenLeds[index].x;}
    inline int  getGreenLedY(int index)const {return m_VLF.GreenLeds[index].y;}
    //获得红灯信息
    //inline int  getNumberOfRedLed() {return NumberOfRedLED;}
    //inline void writeNumberOfRedLed(int index){ NumberOfRedLED=index;}
    inline bool getRedLedVisible(int index)const {return  m_VLF.RedLeds[index].isVisible;}
    inline int  getRedLedX(int index)const {return m_VLF.RedLeds[index].x;}
    inline int  getRedLedY(int index)const {return m_VLF.RedLeds[index].y;}
    //获得数码管信息
    //inline int  getNumberOfSSTube() {return NumberOfSSTube;}
    // inline void writeNumberOfSSTube(int index){ NumberOfSSTube=index;}
    inline bool getSSTubeVisible(int index)const {return m_VLF.HexTubes[index].isVisible;}
    inline int  getSSTubeX(int index)const  {return m_VLF.HexTubes[index].x;}
    inline int  getSSTubeY(int index)const {return m_VLF.HexTubes[index].y;}
    //获得按键信息
    //inline int  getNumberOfKey() {return NumberOfKey;}
   // inline void writeNumberOfKey(int index){ NumberOfKey=index;}
    inline bool getKeyVisible(int index)const {return m_VLF.Keys[index].isVisible;}
    inline int  getKeyX(int index)const {return m_VLF.Keys[index].x;}
    inline int  getKeyY(int index)const {return m_VLF.Keys[index].y;}
    //获取背景信息
    inline int  getPanelWidth() const {return m_VLF.bHavingBmp ? m_VLF.iBmpWidth : 800;}
    inline int  getPanelHeight() const {return m_VLF.bHavingBmp ? m_VLF.iBmpHeight : 500;}
    inline bool getHavingBmp() const {return m_VLF.bHavingBmp;}





signals:

public slots:
};

#endif // VIRTUALPANNELDATA_H
