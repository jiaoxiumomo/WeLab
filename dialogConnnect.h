#ifndef DIALOGCONNNECT_H
#define DIALOGCONNNECT_H

#include <QDialog>
#include "jupodAdpter.h"

namespace Ui {
class DialogConnnect;
}

class DialogConnnect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConnnect(QWidget *parent = 0);
    ~DialogConnnect();
    void setWarningText(QString);

//protected:
//    void changeEvent(QEvent *e);

private:
    Ui::DialogConnnect *ui;
    JupodAdpter *jupod;

private slots:
    void Connecting(void);
    void Refresh(bool silence = false);
};

#endif // DIALOGCONNNECT_H
