#include "mouseevent.h"

mouseevent::mouseevent()
{

}
void MouseLabel::mousePressEvent(QMouseEvent *event)
{
    emit pressed(this); // 在点击事件中发送信号
}
