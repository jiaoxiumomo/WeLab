#ifndef MOUSELABEL_H
#define MOUSELABEL_H
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>


class mouseLabel: public QLabel
{
    Q_OBJECT
public:
    MouseLabel(QWidget *parent=0);
    ~MouseLabel() ;
signals:
    void pressed(MouseLabel* press); // 点击信号
protected:
     void mousePressEvent(QMouseEvent *event); // 重载了鼠标释放事件

};

#endif // MOUSELABEL_H
