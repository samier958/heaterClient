#ifndef CHEATERFAULTINFO_H
#define CHEATERFAULTINFO_H

#include <QWidget>
#include "qmodbusmaster.h"
#include "heaterConfig.h"

using namespace Modbus;

namespace Ui {
class CHeaterFaultInfo;
}

class QLabel;

class CHeaterFaultInfo : public QWidget
{
    Q_OBJECT
    
public:

    const QString faultInfoString[8] = {
        tr("温度传感器故障"),
        tr("温度检测异常"),
        tr("风机过载保护"),
        tr("加热器断电"),
        tr("加热器过热保护"),
        tr("温控设定错误"),
        tr("设备正常"),
        tr("未知")
    };

    typedef struct _heater_fault_info{
        int faultInfo[5];
    }SHeaterFaultInfo;

    explicit CHeaterFaultInfo(QWidget *parent = 0);
    ~CHeaterFaultInfo();
    void showHeaterFaultInfo();
    void updateHeaterFaultInfo();

    SHeaterFaultInfo heaterFaultInfo[FAN_TOWER_GROUP];

signals:
    void sendClientServerCommand(int, int);
public slots:
private slots:
    void on_heaterGroupSwith_currentIndexChanged(int index);

private:
    Ui::CHeaterFaultInfo *ui;

    QLabel *pFaultInfoLabel[5];
    int m_groupSwith;
};

#endif // CHEATERFAULTINFO_H
