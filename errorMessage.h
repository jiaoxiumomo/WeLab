#ifndef ERRORMESSAGE_H
#define ERRORMESSAGE_H

#include <QObject>
#include "JUDEBUG.h"

/*
 * 错误代码约定：0  - 没有错误
 *          小于0  - 显示信息，并显示错误代码
 *          大于0  - 仅显示信息
 * show函数返回值：0 - 小于0的错误已经处理，或者输入参数大于等于0
 *            小于0 - 输入参数传递进来的错误代码
*/
class ErrorMessage:public QObject
{
    Q_OBJECT

public:
    ErrorMessage();
    static int show(int error);

    enum {
        FAILED_TRIAL_LIMIT = -512, //试用版本限制操作次数
        ERROR_MATCH_SIGNED = -513, //JUPOD签名错误
        INFO_NO_SIMULATION = 1     //tr("该实验的仿真功能尚未实现。")
    };
};

#endif // ERRORMESSAGE_H
