#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QGraphicsView>
#include <QTranslator>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QMouseEvent>
#include <QImage>
#include <QButtonGroup>
#include <QLayout>
#include <QDockWidget>
#include <QDrag>
#include <QMimeData>
#include <QPainter>
#include <QSlider>
#include <QTableView>
#include <QTableWidget>
#include <QComboBox>
#include <QFileInfo>
#include "VirtualComponentData.h"
#include "VirtualPanelData.h"
#include "qselectwidget.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
   // void leaveEvent(QEvent *);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool isEnglish;
    bool Designing;//记录是否已打开设计模式
    bool keyReleasesign,ssTubeLightsign;
    //bool isDelete;
    int  isNew;//判断是否创建了新面板
    int realPanelName;
    void setRealPanelInfo(int a,bool b,bool c);
    QString experimentName,experimentDescribe ;//虚拟实验板名字和描述
    QString fileName,filePath,baseName,imagePathName,suffixName,imgsuffix,imgBaseName;
    QFileInfo fi;
    void setLanguage(bool isEnglish);
    const VirtualComponentData * getComponent(void)const {return component;}
    QPushButton  *objctl;//接受进入事件过滤器的对象
    void settingCtlNumber(int a,int b,int c,int d,int e);//新建板子时设定各控件数量
public slots:
    void refreshVirtualPanel(void);
    //设计模式相关
    bool eventFilter(QObject *obj, QEvent *evt);//事件过滤器
    void returnKey(int index);//返回进入过滤器相关控件序号
    void turnDesign(bool isDesign);//切换到设计模式
    void judgeModel();//打开面板时判断是否已经进入设计模式
    void designUIChange();//进入设计模式后UI改变
    void designUIrecovery();//退出后UI恢复
    void createNewPanel();//新建虚拟实验板
    void removingCtl(QWidget *useLessCtl);//删除所选控件，目前是设置为不可见
    void changeBackgroundSize(int index);//改变背景图大小
    void changeCTLSize(int index);//改变控件大小
    void createCtlTable();//创建控件信息表
    void createShowTable();
    void showTable();//显示控件信息表
    void createCtl();//创建控件对象
    void addCombo();//打开或创建面板时在comBoBox中加入标签
    void removeCombo();
    void addoneSwitch(int index);
    void addoneKey(int index);
    void addoneRedLed(int index);
    void addoneGreenled(int index);
    void addoneSStube(int index);
    void addBackground();
    void setupModel();
    void setupShowModel();
    void saveAs();
    void showPanelInfo();
    //待定
//    void mousePressEvent(QMouseEvent *event);
//    //void mouseReleaseEvent(QMouseEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event);
//    void dropEvent(QDropEvent *event);
    void createContainer(QString name, QMultiMap<QPushButton *, QString> &mapI, QPushButton *buttonI, int index);
    void saveing(QTextStream &out, QPushButton *btn, QMultiMap<QPushButton *, QString> &mapI);
    void updateToTable();
private:
    int NumberOfSwitch;
    int NumberOfKey;
    int NumberOfRedLed;
    int NumberOfGreenLed;
    int NumberOfSSTube;
    //菜单和工具按钮
    //                                    右击出菜单
    QMenu *virtualboard,*help, *menuMode,*ongoingTest; //*system,, *menuLanguage,*debugadapter
    QAction *actionOpenPanel, *actionSavePanel,*actionSavePanelAs;  //,*actionRealBoardSettings,*cpanelcomposition,*opanelnotes,*spanelnotes;
    QAction *actionFpgaProgram,*actionLinkAdapter,*actionExit;  //,*powersettings,*measuringvoltage;
    QAction *aboutAction,*debugadapternotes,*testcircuitnotes, *actionChangeLanguage;
    QAction *actionSimulate, *actionDesign;
    QAction *removectl;//右击控件出删除选项
    QToolBar *toolBar1;

    //设计停靠窗口
    QDockWidget *controlDock,*ctlTableDock;
    QWidget *dockWidget;
    QGridLayout *ctldocklayout;
    QSlider *ctlSizeSlider,*backgroundSizeSlider;
    //虚拟面板元件
    QPushButton   **sstubeButton;
    QGraphicsView **sstubeViewFace;
    QGraphicsScene **sstubeViewSence;
    QPixmap sstubePixmap[8];
    QPixmap nullPixmap;

    QGraphicsScene scene;
    QGraphicsView *graphicsView,*mainView;
    QLabel *ctlSizeLabel,*backgroundSizeLabel;
    QPushButton **switchFace,**keyFace,**greenLedFace,**redLedFace,*addctl[4],*setPannelInfo;//,*pannelInfo
    QButtonGroup *keyFaceGroup,*switchFaceGroup,*sstubeGroup,*greenLedFaceGroup,*redLedFaceGroup;
    //虚拟元件外观图片
    QPixmap ledOffImage,greenLedImage,redLedImage,sstubeOffImage;
    QPixmap keyupIcon,keydownIcon,myImage,switch0Icon, switch1Icon;//,switchPixmap;
    //设计面板
    QTableView *ctlIfoTable,*showctlIfoTable;
    QStandardItemModel *ctlModel,*showModel;
   // QDrag *drag;
    QComboBox *switchComboBox,*keyComboBox,*greenLedComboBox,*redLedComboBox,*sstubeComboBox;
    //虚拟元件的外观尺寸
    int wSwitch = 20;
    int hSwitch = 35;
    int wKey = 50;   //48;    //36;    //
    int hKey = 50;   //32;    //36;    //
    int wLED = 30;
    int hLED = 30;
    int wSSTube = 43/2;
    int hSSTube = 67/2;

    //数据
    VirtualComponentData * component;
    VirtualPanelData *pannel;    
    int errorKeyDown;


    void createMenuAndBar(void);
    void createDock(void);
    void showRedLED(int index, bool state);
    void showGreenLED(int index, bool state);
    void showSSTube(int index, char seg);
    void showPannelSetting();
    void deleteAllFace();   
    void loadKeyImage(void);
    void designConnect();//设计模式的信号和槽连接
    void experimentConnect();//实验和仿真模式的信号和槽连接
    void ctlToolConnect();
private slots:
    void onOpenPanel();
    void readVPL(QString fileName,QString filePath,QString baseName);
    void readVP2(QString fileName,QString filePath);
    void readVPjson(QString fileName);
  void onRealBoardSettings(void);
    void onSwitchToggle(int index,bool state);
    void onKeyUp(int index);
    void onKeyDown(int index);
    void fpgaProgram();
    void showAdapterInfo();
    void showAbout();
    void showFPGAInfo();
    void showlinkDlg();
    void changeLanguage(void);
    void turnSimulation(bool checked);
    void savePanelInfo();
signals:
    //void sendObject(QPushButton *deleteCtl);


};

#endif // MAINWINDOW_H
