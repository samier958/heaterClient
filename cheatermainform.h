#ifndef CHEATERMAINFORM_H
#define CHEATERMAINFORM_H

#include <QWidget>


#include "qmodbusmaster.h"

using namespace Modbus;

namespace Ui {
class CHeaterMainForm;
}

class QTimer;

class CHeaterMainForm : public QWidget
{
    Q_OBJECT
    
public:
    QString systemTimeString[5] = {
        tr("年"),
        tr("月"),
        tr("日   "),
        tr(":"),
        tr(" ")
    };
    explicit CHeaterMainForm(QWidget *parent = 0);
    ~CHeaterMainForm();

    void setModbusMaster(QModbusMaster *modbusMaster);
signals:
    void establishNetworkConnection();
    void disconnectNetwork();

public slots:
    void reflashHeaterControlForm(int index);
private slots:
    void updateHeaterSystemTime();
    void showHeaterSystemTime();


private:
    Ui::CHeaterMainForm *ui;


    QTimer *pTimer;
    QModbusMaster *pModbusMaster;

    int m_time[5];
};

#endif // CHEATERMAINFORM_H
