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
        tr("PT100故障"),
        tr("温度异常"),
        tr("风机过载"),
        tr("PTC无电"),
        tr("干烧故障"),
        tr("设定错误"),
        tr("无故障"),
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
