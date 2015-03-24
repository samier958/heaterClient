#ifndef CHEATERHISTORYRECORD_H
#define CHEATERHISTORYRECORD_H

#include <QWidget>

#include "qmodbusmaster.h"
#include "heaterConfig.h"

using namespace Modbus;

namespace Ui {
class CHeaterHistoryRecord;
}



class CHeaterHistoryRecord : public QWidget
{
    Q_OBJECT
    
public:
    typedef struct _heater_history_record_fixed_time{
        int time[5];
        int valid;
        int wait;
        int index;

        int temperature;
        int controlMode;
        int runningStatus;
        int faultInfo;

        int inputPort;
        int outputPort;
    }SHeaterHistoryRecordFixedTime;

    typedef struct _work_or_fault_record{
        int year;
        int month;
        int day;
        int hour;
        int min;
        int code;
    }SWorkOrFaultRecord;

    typedef struct _heater_history_record_work_or_fault{
        SWorkOrFaultRecord workRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
        SWorkOrFaultRecord faultRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
    }SHeaterHistoryRecordWorkOrFault;

    const QString controlModeString[5] = {
        tr("自动"),
        tr("停止"),
        tr("加热"),
        tr("吹风"),
        tr("其它")
    };
    const QString runningStatusString[5] = {
        tr("停止"),
        tr("关闭"),
        tr("加热"),
        tr("吹风"),
        tr("其它")
    };
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

    const QString workOrFaultRecordString[19] = {
        tr("未定义"),
        tr("控制器PT100温度检测模块通信故障"),
        tr("控制器内置数据存储器故障"),
        tr("#机舱加热器通信故障"),
        tr("#机舱加热器温度传感器故障"),
        tr("#机舱加热器温度检测异常"),
        tr("#机舱加热器风机过载故障"),
        tr("#机舱加热器PTC电源开关跳闸"),
        tr("#机舱加热器PTC温度异常警告"),
        tr("#机舱加热器自动启停温度设置错误"),
        tr("#机舱加热器自动控制启动加热"),
        tr("#机舱加热器手动控制启动加热"),
        tr("#机舱加热器远程控制启动加热"),
        tr("#机舱加热器网络控制启动加热"),
        tr("#机舱加热器自动控制启动散热"),
        tr("#机舱加热器手动控制启动散热"),
        tr("#机舱加热器远程控制启动散热"),
        tr("#机舱加热器网络控制启动散热"),
        tr("其它")

    };
    const QString timeString[5] = {
        tr("年"),
        tr("月"),
        tr("日  "),
        tr(":"),
        tr(" ")
    };
    const QString portString[3] = {
        tr("无输出"),
        tr("有输出"),
        tr("其它")
    };

    explicit CHeaterHistoryRecord(QWidget *parent = 0);
    ~CHeaterHistoryRecord();

    void showHeaterHistoryRecordFixedTime();
    void showHeaterHistoryRecordWorkOrFault();

    void currentTimeSetToRecord();

    SHeaterHistoryRecordFixedTime heaterHistoryRecordFixedTime[FAN_TOWER_GROUP];
    SHeaterHistoryRecordWorkOrFault heaterHistoryRecordWorkOrFault[FAN_TOWER_GROUP];
signals:
    void sendClientServerCommand(int);
public slots:

private slots:
    void on_read_released();

    void on_cancle_released();

    void on_hearterFixedTimeIndex_currentIndexChanged(int index);

    void on_clearWorkRecord_released();

    void on_clearFaultRecord_released();

private:
    Ui::CHeaterHistoryRecord *ui;

    QString m_workRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
    QString m_faultRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
};

#endif // CHEATERHISTORYRECORD_H
