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
        tr("PT100温度传感器连接故障"),
        tr("温度检测异常警告"),
        tr("风机断路器过载跳闸"),
        tr("PTC断路器过载跳闸"),
        tr("PTC表面温度超高警告"),
        tr("启停温度设定错误"),
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

public slots:
private slots:
private:
    Ui::CHeaterFaultInfo *ui;

    QLabel *pFaultInfoLabel[5];
};

#endif // CHEATERFAULTINFO_H
