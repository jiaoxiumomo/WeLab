#include "VirtualPanelData.h"
#include <QFile>
#include <QDataStream>
#include "ErrorMessage.h"

VirtualPanelData::VirtualPanelData(QObject *parent) : QObject(parent)
{
//    NumberOfSwitch = 0;
//    NumberOfGreenLED=0;
//    NumberOfRedLED=0;
//    NumberOfSSTube=0;
//    NumberOfKey=0;
}


void VirtualPanelData::ImportFromFile(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
       ErrorMessage::show(ERROR_OPEN_FILE);
       return;
    }
    QByteArray ba = file.readAll();
    memcpy(&m_VLF,ba.data(),ba.size());

//    NumberOfSwitch = 18;
//    NumberOfGreenLED=9;
//    NumberOfRedLED=18;
//    NumberOfSSTube=8;
//    NumberOfKey=4;
}

