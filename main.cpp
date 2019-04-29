#include "mainwindow.h"
#include <QApplication>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //根据Language.json文件中的设置，装载语言包
    bool isEnglish = false;
    QTranslator translator;
    if (translator.load(":/trans/english"))
    {   //默认英文界面
        a.installTranslator(&translator);
        isEnglish = true;
        SetDllLanguage(1);
    }
    QString localLanguage;
    QFile file(qApp->applicationDirPath() + "\\Language.json");
    if(!file.exists()) //如果应用程序文件夹内没有"Language.json"，再看看用户数据文件夹内有没有
        file.setFileName(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "\\Language.json");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        QJsonParseError e;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
        if(e.error == QJsonParseError::NoError && !jsonDoc.isNull())
        {
            QJsonObject objLanguage = jsonDoc.object();
            localLanguage = objLanguage["language"].toString();
        }
        file.close();
        if (localLanguage.toLower() == "chinese")
        {
            a.removeTranslator(&translator);
            isEnglish = false;
            SetDllLanguage(0);
            // QTranslator translator; 这个变量声明如果放在这里就不会显示英文界面，必须放到前面
            /*if (translator.load(":/trans/english"))
            {
                a.installTranslator(&translator);
                isEnglish = true;
            }*/
        }
    }

    MainWindow w;
    w.setLanguage(isEnglish); //主窗口创建之后，将当前语言传递进去。没有找到Qt获取当前语言的方法，只好这样做了。
    w.show();

    return a.exec();
}
