#include "errorMessage.h"
#include <QMessageBox>
#include "dialogConnnect.h"
#include "ui_dialogConnnect.h"

ErrorMessage::ErrorMessage()
{

}

int ErrorMessage::show(int error)
{
    QString mess;
    switch(error)
    {
    case SUCCESS:
        break;
    case ERROR_OPEN_JUPOD:
        mess = tr("没有发现调试适配器。");
        break;
    case ERROR_CONFIRMATION:
        mess = tr("无效的JUPOD许可。");
        break;
    case ERROR_CONNECT_JUPOD:
        mess = tr("出现未知的JUPOD连接错误。");
        break;
    case ERROR_CONNECT_FPGA:
        mess = tr("连接FPGA失败。");
        break;
    case ERROR_NOTYET_CONNECT:
        mess = tr("尚未选择调试适配器！请重新连接。");
        break;
    case ERROR_SEND_FRAME:
        mess = tr("调试适配器收到的数据格式错误。");
        break;
    case ERROR_RECIEVE_FRAME:
        mess = tr("收到的数据格式错误。");
        break;
    case ERROR_CONNECT_FRAME:
        mess = tr("调试适配器收到的连接数据格式错误。");
        break;
    case ERROR_RECIEVE_EMPTY:
        mess = tr("等待调试适配器返回数据超时。");
        break;
    case ERROR_SOFTWARE_LICENSE:
        mess = tr("无效的软件许可。");
        break;
    case ERROR_UNSUPPORTED_HARDWARE:
        mess = tr("硬件不支持该功能。");
        break;
    case ERROR_FPGA_DATA:
        mess = tr("传送FPGA配置数据出错。");
        break;
    case ERROR_BURN_FPGA_FAILED:
        mess = tr("配置FPGA失败。");
        break;
    case ERROR_SOCKET_FAILED:
        mess = tr("创建套接字错误。");
        break;
    case ERROR_HOST_NAME:
        mess = tr("服务器的IP地址错误。");
        break;
    case ERROR_CONNECT_SERVER:
        mess = tr("连接服务器出错，无法连接服务器。");
        break;
    case ERROR_SEND_DATA:
        mess = tr("要发送的数据太大，超过了缓冲区大小。");
        break;
    case ERROR_SEND_FAILED:
        mess = tr("向服务器发送数据错误。");
        break;
    case ERROR_TIMESOUT:
        mess = tr("等待服务器返回数据超时。");
        break;
    case ERROR_CONNECT_SERVERPOD:
        mess = tr("连接ServerPOD出错。");
        break;
    case ERROR_DATA_TRANSFER:
        mess = tr("远程实验服务没有返回足够的数据。");
        break;
    case PRE_NOT_FINISHED:
        mess = tr("上一步运行尚未结束，请停止运行或复位后再操作。");
        break;;
    case ERROR_OPEN_FILE:
        mess = tr("无法打开指定的文件。");
        break;;
    case ERROR_FILE_NULL:
        mess = tr("指定的文件不包含任何内容。");
        break;

    case FAILED_TRIAL_LIMIT:
        mess = tr("试用版本限制操作次数。");
        break;
    case ERROR_MATCH_SIGNED:
        mess = tr("JUPOD签名错误！");
        break;

    case INFO_NO_SIMULATION:
        mess = tr("该实验的仿真功能尚未实现。");
        break;
    default:
        mess = tr("发生未知错误。");
        break;
    }
    //mess.append(QString(tr("错误代码：%1")).arg(error));
    if (error == ERROR_NOTYET_CONNECT)
    {   //如果连接失败，自动弹出连接对话框
        DialogConnnect dialog(NULL);
        dialog.setWarningText(mess);
        if (dialog.exec() == QDialog::Accepted)
            error = 0; //连接成功，返回0
    }
    else if(error != 0)
    {
        //QMessageBox::warning(NULL, tr("注意"),mess);
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("注意"));
        msgBox.setText(mess);
        if (error < 0)
            msgBox.setInformativeText(QString(tr("错误代码：%1")).arg(error));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
    }
    if (error > 0)  //约定大于0的不是错误，仅显示信息
        error = 0;  //返回0，表示错误已经处理。
    return error;
}
