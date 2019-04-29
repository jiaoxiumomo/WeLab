#ifndef ABOUTSYXT_H
#define ABOUTSYXT_H

#include <QDialog>
#include <QLabel>



class about:public QDialog
{
    Q_OBJECT
public:
    explicit about(QWidget *parent=0);
    ~about();

     QLabel *label1,*label2,*label3;


};

#endif // ABOUTSYXT_H
