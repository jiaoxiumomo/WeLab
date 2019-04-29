#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class mouseevent
{
public:
    mouseevent();
};

//重载label类
class MouseLabel : public QLabel
{
    Q_OBJECT
public:
    MouseLabel(QWidget *parent=0): QLabel(parent){}
    ~MouseLabel() {}
signals:
    void pressed(MouseLabel* press); // 点击信号
protected:
     void mousePressEvent(QMouseEvent *event); // 重载了鼠标释放事件

};

#endif // MOUSEEVENT_H
