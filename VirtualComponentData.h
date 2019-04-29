  #ifndef VIRTUALCOMPONENTDATA_H
#define VIRTUALCOMPONENTDATA_H

#include <QObject>
#include "JUDEBUG.h"

#define SWITCH_CHAIN_LENGTH     36	// 拨动开关扫描链长度（位），必须与硬件设计一致。
#define KEY_CHAIN_LENGTH     5	// 按键扫描链长度（位），必须与硬件设计一致。
#define REDLED_CHAIN_LENGTH     18	// 红色LED指示灯扫描链长度（位），必须与硬件设计一致。
#define GREENLED_CHAIN_LENGTH	18  // 绿色LED指示灯扫描链长度（位），必须与硬件设计一致。
#define LED_CHAIN_LENGTH        (REDLED_CHAIN_LENGTH + GREENLED_CHAIN_LENGTH) 	//36
#define SSTube_CHAIN_LENGTH      64	// 数码管扫描链长度（位），必须与硬件设计一致。

typedef enum  //虚拟按键的取值
{   //注意：如果实际电路板设计与此不同，不要改变此值，应通过RealKeyReleaseLevel设置。
    KEYRELEASE = 0,
    KEYPRESS = 1
}ButtonState;

typedef enum
{
    LOWLEVEL = 0,
    HIGHLEVEL = 1
}BinaryLevel;

class VirtualComponentData : public QObject
{
    Q_OBJECT
protected:
    int NumberOfSwitch;
    int NumberOfKey;
    int NumberOfRedLED;
    int NumberOfGreenLED;
    int NumberOfSSTube;
    bool * SwitchValue;
    bool * KeyValue;     //1-按下，0-松开
    bool * RedLEDValue;     //1-点亮，0-熄灭
    bool * GreenLEDValue;   //同上
    char * SSTubeValue;  //8位分别对应数码管的8个段，1-点亮，0-熄灭
    bool RealKeyReleaseLevel;	// 实际电路板按键松开状态的电平值
    char RealSSTubeDarkCode;    // 实际电路板七段数码管熄灭的电平值。注意该字节的8位应相同，即要么为0xFF，要么为0x00。

    //仿真
    bool isSimulate;
    void (VirtualComponentData::* SimulateLab)(void);
    char hex2SevenSegment(int hex);
    void SimulateLab1(void);
    void SimulateLab2(void);
    void SimulateExp1(void);
    void SimulateExp2(void);

public:
    explicit VirtualComponentData(QObject *parent = nullptr);
    //VirtualComponentData(int numSwitch, int numButton, int numRedLED, int numGreenLED,int numSSTube);
    ~VirtualComponentData();
    void SetRealKeyReleaseLevel(bool value) {RealKeyReleaseLevel=value;}
    void SetRealSevenSegmentTubeLightLevel(bool value);
    int  SetupSwitch(int number);
    int  SetupKey(int number);
    int  SetupRedLED(int number);
    int  SetupGreenLED(int number);
    int  SetupSevenSegmentTube(int number);
    int ChangeToFPGA(void);
    int  AcquireFromFPGA(void);
    bool GetRedLEDValue(int index)const {return RedLEDValue[index];}
    bool GetGreenLEDValue(int index)const {return GreenLEDValue[index];}
    char GetSevenSegmentTubeValue(int index)const {return SSTubeValue[index];}
    bool GetRealKeyReleaseLevel(void)const {return RealKeyReleaseLevel;}
    void setSimulation(bool checked) {isSimulate = checked;}
    void SetLabID(QString name);

signals:
    void FPGAStateChanged(void);

public slots:
    void SwitchChange(int index, bool value);
    int  KeyPressed(int);
    int  KeyReleased(int);

//TRIAL_EDITION
private:
    int trialClickCount;
    const int TrialClickLimit = 10; //试用版本限制操作的步数
public:
    void PanelChange(void) {trialClickCount = 0;}
};
#endif // VIRTUALELEMENT_H
