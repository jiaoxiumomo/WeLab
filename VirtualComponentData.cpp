#include "VirtualComponentData.h"
#include "errorMessage.h"
#include "dialogConnnect.h"

VirtualComponentData::VirtualComponentData(QObject *parent) : QObject(parent)
{
    NumberOfSwitch = 0;
    NumberOfKey = 0;
    NumberOfRedLED = 0;
    NumberOfGreenLED = 0;
    NumberOfSSTube = 0;

    RealKeyReleaseLevel = HIGHLEVEL; // 以DE2-115为默认值，按键松开时为1。
    RealSSTubeDarkCode = 0xFF;   // 以DE2-115为默认值，0点亮，1熄灭。

    SwitchValue = NULL;
    KeyValue = NULL;
    RedLEDValue = NULL;
    GreenLEDValue = NULL;
    SSTubeValue = NULL;

    trialClickCount = 0;

    isSimulate = false;
    SimulateLab = NULL;
}

/*VirtualComponentData::VirtualComponentData(int numSwitch, int numButton, int numRedLED, int numGreenLED,int numSSTube)
{
    RealKeyReleaseLevel = HIGHLEVEL; // 以DE2-115为默认值，按键松开时为1。
    RealSSTubeDarkCode = 0xFF;   // 以DE2-115为默认值，0点亮，1熄灭。

    SwitchValue = NULL;
    KeyValue = NULL;
    RedLEDValue = NULL;
    GreenLEDValue = NULL;
    SSTubeValue = NULL;

    SetupSwitch(numSwitch);
    SetupKey(numButton);
    SetupRedLED(numRedLED);
    SetupGreenLED(numGreenLED);
    SetupSevenSegmentTube(numSSTube);
#ifdef TRIAL_EDITION
    trialClickCount = 0;
#endif
}*/

VirtualComponentData::~VirtualComponentData()
{
    delete[] SwitchValue;
    delete[] KeyValue;
    delete[] RedLEDValue;
    delete[] GreenLEDValue;
    delete[] SSTubeValue;
}

void VirtualComponentData::SetRealSevenSegmentTubeLightLevel(bool value)
{
    if (value) //== HIGHLEVEL
        RealSSTubeDarkCode = 0x00;
    else
        RealSSTubeDarkCode = 0xFF;
}

int VirtualComponentData::SetupSwitch(int number)
{    
    if (number > SWITCH_CHAIN_LENGTH)
    {
        NumberOfSwitch = SWITCH_CHAIN_LENGTH;
        return -1;
    }
    else
        NumberOfSwitch = number;
        
    delete[] SwitchValue;
    SwitchValue = new bool[NumberOfSwitch];
    for (int i=0; i<NumberOfSwitch; i++)
    {
        SwitchValue[i] = 0;
    }
    return 0;
}

int VirtualComponentData::SetupKey(int number)
{
    if (number > KEY_CHAIN_LENGTH)
    {
        NumberOfKey = KEY_CHAIN_LENGTH;
        return -1;
    }
    else
        NumberOfKey = number;
    
	delete[] KeyValue;
    KeyValue = new bool[NumberOfKey];
    for (int i=0; i<NumberOfKey; i++)
    {
        KeyValue[i] = RealKeyReleaseLevel;
    }
    return 0;
}

int VirtualComponentData::SetupRedLED(int number)
{
    if (number > REDLED_CHAIN_LENGTH)
    {
        NumberOfRedLED = REDLED_CHAIN_LENGTH;
        return -1;
    }
    else
        NumberOfRedLED = number;
        
    delete[] RedLEDValue;
    RedLEDValue = new bool[NumberOfRedLED];
    for (int i=0; i<NumberOfRedLED; i++)
    {
        RedLEDValue[i] = 0;
    }
    return 0;
}

int VirtualComponentData::SetupGreenLED(int number)
{
    if (number > GREENLED_CHAIN_LENGTH)
    {
        NumberOfGreenLED = GREENLED_CHAIN_LENGTH;
        return -1;
    }
    else
        NumberOfGreenLED = number;
        
    delete[] GreenLEDValue;
    GreenLEDValue = new bool[NumberOfGreenLED];
    for (int i=0; i<NumberOfGreenLED; i++)
    {
        GreenLEDValue[i] = 0;
    }
    return 0;
}

int VirtualComponentData::SetupSevenSegmentTube(int number)
{
    if (number > SSTube_CHAIN_LENGTH)
    {
        NumberOfSSTube = SSTube_CHAIN_LENGTH;
        return -1;
    }
    else
        NumberOfSSTube = number;
        
    delete[] SSTubeValue;
    SSTubeValue = new char[NumberOfSSTube];
    for (int i=0; i<NumberOfSSTube; i++)
    {
        SSTubeValue[i] = RealSSTubeDarkCode;
    }
    return 0;
}

void VirtualComponentData::SwitchChange(int index, bool value)
{
    SwitchValue[index] = value;
    ChangeToFPGA();
}

int VirtualComponentData::KeyPressed(int index)
{
    KeyValue[index] = KEYPRESS ^ RealKeyReleaseLevel;
    return ChangeToFPGA();
}
int VirtualComponentData::KeyReleased(int index)
{
    KeyValue[index] = KEYRELEASE ^ RealKeyReleaseLevel;
    return ChangeToFPGA();
}


int VirtualComponentData::ChangeToFPGA(void)
{
    int error, numOfBytes, i, j;
    unsigned char * txdata;

    if (isSimulate)
    {
        if (SimulateLab == NULL)
            ErrorMessage::show(ErrorMessage::INFO_NO_SIMULATION);
        else
            (this->*SimulateLab)();
        return 0;
    }

#ifdef TRIAL_EDITION
    if (trialClickCount++ >= TrialClickLimit)
    {
        ErrorMessage::show(ErrorMessage::FAILED_TRIAL_LIMIT);//"试用版本限制点击10次！"
        return ErrorMessage::FAILED_TRIAL_LIMIT;
    }
#endif

    error = SetVirtualMode(true);
    if (error)
    {
        error = ErrorMessage::show(error);
        if (error==0)
            error = SetVirtualMode(true);
        else
            return error;
    }

    numOfBytes = (SWITCH_CHAIN_LENGTH+7)/8;
    txdata = new unsigned char[numOfBytes];
    for (i=0; i<numOfBytes; i++)
    {
        txdata[i] = 0;
        for (j=0; j<8; j++)
        {
            if (i*8+j >= NumberOfSwitch)
                break;
            txdata[i] |= SwitchValue[i*8+j] << j;
        }
    }
    error = SetSwitchStatus(txdata, SWITCH_CHAIN_LENGTH);
    delete[] txdata;
    if (error)
    {
        ErrorMessage::show(error);
        return error;
    }

    numOfBytes = (KEY_CHAIN_LENGTH+7)/8;
    txdata = new unsigned char[numOfBytes];
    for (i=0; i<numOfBytes; i++)
    {
        txdata[i] = 0;
        for (j=0; j<8; j++)
        {
            if (i*8+j >= NumberOfKey)
                break;
            txdata[i] |= KeyValue[i*8+j] << j;
        }
    }
    error = SetButtonStatus(txdata,KEY_CHAIN_LENGTH);
    delete[] txdata;
    if (error)
    {
        ErrorMessage::show(error);
        return error;
    }

    return AcquireFromFPGA();
}

int VirtualComponentData::AcquireFromFPGA(void)
{
    int error, numOfBytes, i, j;
    unsigned char * rxdata;
    //采集LED数据
    numOfBytes = (LED_CHAIN_LENGTH+7)/8;
    rxdata = new unsigned char[numOfBytes];
    error = GetLedStatus(LED_CHAIN_LENGTH,rxdata);
    if(error != 0)
    {
        delete[] rxdata;
        ErrorMessage::show(error);
        return error;
    }
    //解析采集到的LED数据
    char bitMask;
    for (i=0; i<(NumberOfRedLED+7/8); i++)
    {
        bitMask = 0x01;
        for (j=0; j<8; j++)
        {
            if (i*8+j >= NumberOfRedLED)
                break;
            RedLEDValue[i*8+j] = rxdata[i] & bitMask;
            bitMask = bitMask << 1;
        }
    }
    int gi = REDLED_CHAIN_LENGTH/8;
    int gj = REDLED_CHAIN_LENGTH%8;
    j = gj;
    bitMask = 0x01 << gj;
    for (i=0; i<(NumberOfGreenLED+7/8); i++)
    {
        for (; j<8; j++)
        {
            if (i*8+j-gj >= NumberOfGreenLED)
                break;
            GreenLEDValue[i*8+j-gj] = rxdata[i+gi] & bitMask;
            bitMask = bitMask << 1;
        }
        j=0;
        bitMask = 0x01;
    }

    delete[] rxdata;
    //采集七段数码管数据
    numOfBytes = (SSTube_CHAIN_LENGTH+7)/8;
    rxdata = new unsigned char[numOfBytes];
    error = GetHexStatus(SSTube_CHAIN_LENGTH,rxdata);
    if(error != 0)
    {
        delete[] rxdata;
        ErrorMessage::show(error);
        return error;
    }
    //解析采集到的七段数码管数据
    for (i=0; i<NumberOfSSTube; i++)
    {
        SSTubeValue[i] = rxdata[i] ^ RealSSTubeDarkCode;
    }

    delete[] rxdata;
    emit FPGAStateChanged();
    return error;
}

//仿真
void VirtualComponentData::SetLabID(QString name)
{
    if (name == "XHHCS1502") //lab1:信号和传输（2015年实践教程实验一）
        SimulateLab = SimulateLab1;
    else if (name == "JJYSJTZBZ1502")    //lab2:加减运算及特征标志（2015年实践教程实验二）
        SimulateLab = SimulateLab2;
    else if (name == "JBLJDL1708")  //基本逻辑电路（2017年8月设计）
        SimulateLab = SimulateExp1;
    else if (name == "DGNJJYSDL1512")  //多功能加减运算电路（2015年12月设计）
        SimulateLab = SimulateExp2;
    else
        SimulateLab = NULL;
}

void VirtualComponentData::SimulateLab1(void)
{
    int DATA0 = 8*SwitchValue[7] + 4*SwitchValue[6] + 2*SwitchValue[5] + SwitchValue[4];
    int DATA1 = 8*SwitchValue[16] + 4*SwitchValue[15] + 2*SwitchValue[14] + SwitchValue[13];
    int DATA2 = 8*SwitchValue[12] + 4*SwitchValue[11] + 2*SwitchValue[10] + SwitchValue[9];

    bool DATA0oe = SwitchValue[8];
    bool DATA1oe = SwitchValue[17];

    bool RESET = !KeyValue[0];
    bool CLK = KeyValue[1];
    bool REGce = SwitchValue[0];
    bool SVce = SwitchValue[1];
    bool SLce = SwitchValue[2];
    bool SRce = SwitchValue[3];

    //模拟时钟上升沿
    static bool lastCLK = true;
    bool trigger = !lastCLK && CLK;
    lastCLK = CLK;

    //内部信号定义
    int BUS;
    static int REG_Q=0, SHF_Q=0;

    //三态缓冲器逻辑描述
    switch ((DATA1oe<<1)+DATA0oe)
    {
    case 1:
        BUS = DATA0;
        break;
    case 2:
        BUS = DATA1;
        break;
    case 3:
        BUS = DATA1 & DATA0;
        break;
    default:
        BUS = 0;
        break;
    }

    //寄存器模块实例化
    if (RESET)
        REG_Q = 0;
    else if (trigger && REGce)
        REG_Q = DATA2;


    //移位寄存器模块实例化
    if (RESET)
        SHF_Q = 0;
    else if (trigger)
    {
        if (SVce)
            SHF_Q = DATA2;
        else if (SLce)
            SHF_Q = DATA2 << 1;
        else if (SRce)
            SHF_Q = DATA2 >> 1;
    }

    //内部信号赋值给输出端口观察
    RedLEDValue[3]    = REG_Q & (0x01<<3);
    RedLEDValue[2]    = REG_Q & (0x01<<2);
    RedLEDValue[1]    = REG_Q & (0x01<<1);
    RedLEDValue[0]    = REG_Q & (0x01<<0);
    RedLEDValue[7]    = DATA0 & (0x01<<3);
    RedLEDValue[6]    = DATA0 & (0x01<<2);
    RedLEDValue[5]    = DATA0 & (0x01<<1);
    RedLEDValue[4]    = DATA0 & (0x01<<0);
    RedLEDValue[8]    = DATA0oe;
    RedLEDValue[12]    = SHF_Q & (0x01<<3);
    RedLEDValue[11]    = SHF_Q & (0x01<<2);
    RedLEDValue[10]    = SHF_Q & (0x01<<1);
    RedLEDValue[9]     = SHF_Q & (0x01<<0);
    RedLEDValue[16]    = DATA1 & (0x01<<3);
    RedLEDValue[15]    = DATA1 & (0x01<<2);
    RedLEDValue[14]    = DATA1 & (0x01<<1);
    RedLEDValue[13]    = DATA1 & (0x01<<0);
    RedLEDValue[17]    = DATA1oe;

    GreenLEDValue[3]    = KeyValue[3];
    GreenLEDValue[2]    = KeyValue[2];
    GreenLEDValue[1]    = KeyValue[1];
    GreenLEDValue[0]    = KeyValue[0];
    GreenLEDValue[7]    = BUS & (0x01<<3);
    GreenLEDValue[6]    = BUS & (0x01<<2);
    GreenLEDValue[5]    = BUS & (0x01<<1);
    GreenLEDValue[4]    = BUS & (0x01<<0);
    GreenLEDValue[8]    = RESET;

    //内部信号赋值给输出端口（七段数码管）观察
    SSTubeValue[7] = hex2SevenSegment(-1); //消隐
    SSTubeValue[6] = hex2SevenSegment(SHF_Q);
    SSTubeValue[5] = hex2SevenSegment(-1); //消隐
    SSTubeValue[4] = hex2SevenSegment(REG_Q);
    SSTubeValue[3] = hex2SevenSegment(-1); //消隐
    SSTubeValue[2] = hex2SevenSegment(-1); //消隐
    SSTubeValue[1] = hex2SevenSegment(-1); //消隐
    SSTubeValue[0] = hex2SevenSegment(BUS);

    emit FPGAStateChanged();
    return;
}

void VirtualComponentData::SimulateLab2(void)
{
    //const widthMask = 0x08;
    int dst, src, M1, M0;
    bool M3, M2, Cin;
    int A, B, result, F;
    bool C0, S,Z,O,C;
    M3 = SwitchValue[12];
    M2 = SwitchValue[11];
    M1 = SwitchValue[10] ? -1 : 0;
    M0 = SwitchValue[9] ? -1 : 0;
    Cin = SwitchValue[8];
    dst = 8*SwitchValue[7] + 4*SwitchValue[6] + 2*SwitchValue[5] + 1*SwitchValue[4];
    src = 8*SwitchValue[3] + 4*SwitchValue[2] + 2*SwitchValue[1] + 1*SwitchValue[0];
    A = dst;
    B = M1 ^ (src & M0);
    C0 = (Cin & M3) | (M2);
    result = A+B+C0;
    F = result & 0x0000000F;
    S = (F&0x08);
    Z = (F==0);
    O = ((~(A&0x08)) & ~(B&0x08) & (F&0x08)) | ((A&0x08) & (B&0x08) & ~(F&0x08));
    C = result & 0x08;

    /* 红色LED在虚拟面板上并没有显示，虽然在硬件设计上存在。
    RedLEDValue[0] = B & 0x01;
    RedLEDValue[1] = B & 0x02;
    RedLEDValue[2] = B & 0x04;
    RedLEDValue[3] = B & 0x08;
    RedLEDValue[] = dst & 0x0;
    RedLEDValue[16] = F & (0x01<<15);
    RedLEDValue[15] = F & (0x01<<15);
    RedLEDValue[14] = F & (0x01<<14);
    RedLEDValue[13] = F & (0x01<<13);*/
    GreenLEDValue[8] = C0;
    GreenLEDValue[3] = S;
    GreenLEDValue[2] = Z;
    GreenLEDValue[1] = O;
    GreenLEDValue[0] = C;
    SSTubeValue[7] = hex2SevenSegment(-1); //消隐
    SSTubeValue[6] = hex2SevenSegment(dst);
    SSTubeValue[5] = hex2SevenSegment(-1); //消隐
    SSTubeValue[4] = hex2SevenSegment(B);
    SSTubeValue[3] = hex2SevenSegment(-1); //消隐
    SSTubeValue[2] = hex2SevenSegment(-1); //消隐
    SSTubeValue[1] = hex2SevenSegment(-1); //消隐
    SSTubeValue[0] = hex2SevenSegment(F);

    emit FPGAStateChanged();
    return;
}

void VirtualComponentData::SimulateExp1(void)
{
    //输入端口给内部信号
    bool RESET=KeyValue[0]^RealKeyReleaseLevel;
    bool CLK  =KeyValue[1];
    bool CNToe=SwitchValue[0];
    bool CNTinc=SwitchValue[1];
    bool CNTce=SwitchValue[2];
    bool SRoe=SwitchValue[3];
    bool S0=SwitchValue[4];
    bool S1=SwitchValue[5];
    int DATA = 8*SwitchValue[10] + 4*SwitchValue[9] + 2*SwitchValue[8] + SwitchValue[7];
    bool REGoe=SwitchValue[11];
    bool REGce=SwitchValue[12];
    //模拟时钟上升沿
    static bool lastCLK = RealKeyReleaseLevel;//true;
    bool trigger = !lastCLK && CLK;
    lastCLK = CLK;
    //内部信号定义
    int BUS;
    static int REG_Q=0,SR_Q=0,CNT_Q=0;
    //寄存器模块实例化
    if (RESET)
        REG_Q = 0;
    else if (trigger && REGce)
        REG_Q = DATA;
    //移位寄存器模块实例化
    if (RESET)
        SR_Q = 0;
    else if (trigger)
    {
        switch (S1*2 + S0)
        {
        case 3:
            SR_Q = DATA;
            break;
        case 2:
            SR_Q = (SR_Q << 1) & 0x0F;
            break;
        case 1:
            SR_Q = (SR_Q >> 1) & 0x0F;
            break;
        default:
            break;
        }
    }
    //计数器模块实例化
    if(RESET)
        CNT_Q=0;
    else if(trigger)
    {
        if (CNTce)
            CNT_Q = DATA;
        else if(CNTinc)
            CNT_Q = CNT_Q + 1;
    }
    //三态缓冲器逻辑描述
    if ((REGoe | SRoe | CNToe)==0)
        BUS = 0;
    else
    {
        BUS = -1;
        if (REGoe)
            BUS &= REG_Q;
        if (SRoe)
            BUS &= SR_Q;
        if (CNToe)
            BUS &= CNT_Q;
    }

    //内部信号赋值给输出端口观察
    RedLEDValue[3]    = CNT_Q & (0x01<<3);
    RedLEDValue[2]    = CNT_Q & (0x01<<2);
    RedLEDValue[1]    = CNT_Q & (0x01<<1);
    RedLEDValue[0]    = CNT_Q & (0x01<<0);
    RedLEDValue[7]    = SR_Q & (0x01<<3);
    RedLEDValue[6]    = SR_Q & (0x01<<2);
    RedLEDValue[5]    = SR_Q & (0x01<<1);
    RedLEDValue[4]    = SR_Q & (0x01<<0);
    RedLEDValue[11]    = REG_Q & (0x01<<3);
    RedLEDValue[10]    = REG_Q & (0x01<<2);
    RedLEDValue[9]     = REG_Q & (0x01<<1);
    RedLEDValue[8]    =  REG_Q & (0x01<<0);
    RedLEDValue[15]    = BUS & (0x01<<3);
    RedLEDValue[14]    = BUS & (0x01<<2);
    RedLEDValue[13]    = BUS & (0x01<<1);
    RedLEDValue[12]    = BUS & (0x01<<0);


    GreenLEDValue[3]    = KeyValue[3];
    GreenLEDValue[2]    = KeyValue[2];
    GreenLEDValue[1]    = CLK;
    GreenLEDValue[0]    = RESET;


    //内部信号赋值给输出端口（七段数码管）观察
    SSTubeValue[7] = hex2SevenSegment(-1); //消隐
    SSTubeValue[6] = hex2SevenSegment(-1); //消隐
    SSTubeValue[5] = hex2SevenSegment(-1); //消隐
    SSTubeValue[4] = hex2SevenSegment(-1); //消隐
    SSTubeValue[3] = hex2SevenSegment(-1); //消隐
    SSTubeValue[2] = hex2SevenSegment(-1); //消隐
    SSTubeValue[1] = hex2SevenSegment(-1); //消隐
    SSTubeValue[0] = hex2SevenSegment(BUS);

    emit FPGAStateChanged();
    return;
}

void VirtualComponentData::SimulateExp2(void)
{
    //const widthMask = 0x08;
    int dst, src, M1, M0;
    bool M3, M2, Cin;
    int A, B, result, F;
    bool C0, S,Z,O,C;
    M3 = SwitchValue[12];
    M2 = SwitchValue[11];
    M1 = SwitchValue[10] ? -1 : 0;
    M0 = SwitchValue[9] ? -1 : 0;
    Cin = SwitchValue[8];
    dst = 8*SwitchValue[7] + 4*SwitchValue[6] + 2*SwitchValue[5] + 1*SwitchValue[4];
    src = 8*SwitchValue[3] + 4*SwitchValue[2] + 2*SwitchValue[1] + 1*SwitchValue[0];
    A = dst;
    B = ((~src)&M1) |(src & M0);
    C0 = (Cin & M3) | (M2);
    result = A+B+C0;
    F = result & 0x0000000F;
    S = (F&0x08);
    Z = (F==0);
    O = ((~(A&0x08)) & ~(B&0x08) & (F&0x08)) | ((A&0x08) & (B&0x08) & ~(F&0x08));
    C = result & 0x08;

    /* 红色LED在虚拟面板上并没有显示，虽然在硬件设计上存在。
    RedLEDValue[0] = B & 0x01;
    RedLEDValue[1] = B & 0x02;
    RedLEDValue[2] = B & 0x04;
    RedLEDValue[3] = B & 0x08;
    RedLEDValue[] = dst & 0x0;
    RedLEDValue[16] = F & (0x01<<15);
    RedLEDValue[15] = F & (0x01<<15);
    RedLEDValue[14] = F & (0x01<<14);
    RedLEDValue[13] = F & (0x01<<13);*/
    GreenLEDValue[8] = C0;
    GreenLEDValue[3] = S;
    GreenLEDValue[2] = Z;
    GreenLEDValue[1] = O;
    GreenLEDValue[0] = C;
    SSTubeValue[7] = hex2SevenSegment(-1); //消隐
    SSTubeValue[6] = hex2SevenSegment(dst);
    SSTubeValue[5] = hex2SevenSegment(-1); //消隐
    SSTubeValue[4] = hex2SevenSegment(B);
    SSTubeValue[3] = hex2SevenSegment(-1); //消隐
    SSTubeValue[2] = hex2SevenSegment(-1); //消隐
    SSTubeValue[1] = hex2SevenSegment(-1); //消隐
    SSTubeValue[0] = hex2SevenSegment(F);

    emit FPGAStateChanged();
    return;
}

 char VirtualComponentData::hex2SevenSegment(int hex)
 {
     char seg;
     switch (hex & 0x0f)
     {
     case 0:
         seg = ~0x40;
         break;
     case 1:
         seg = ~0x79;
         break;
     case 2:
         seg = ~0x24;
         break;
     case 3:
         seg = ~0x30;
         break;
     case 4:
         seg = ~0x19;
         break;
     case 5:
         seg = ~0x12;
         break;
     case 6:
         seg = ~0x02;
         break;
     case 7:
         seg = ~0x78;
         break;
     case 8:
         seg = ~0x00;
         break;
     case 9:
         seg = ~0x18;    //seg = ~0x10
         break;
     case 10:
         seg = ~0x08;
         break;
     case 11:
         seg = ~0x03;
         break;
     case 12:
         seg = ~0x46;
         break;
     case 13:
         seg = ~0x21;
         break;
     case 14:
         seg = ~0x06;
         break;
     case 15:
         seg = ~0x0e;
         break;
     default:
         seg = 0x00;
         break;
     }
     return seg;
}














