#ifndef CHEATERREALTIMEDATA_H
#define CHEATERREALTIMEDATA_H

#include <QWidget>

#include "qmodbusmaster.h"
#include "heaterConfig.h"

using namespace Modbus;

namespace Ui {
class CHeaterRealTimeData;
}

class QLabel;
class QComboBox;

class CHeaterRealTimeData : public QWidget
{
    Q_OBJECT
    
public:
    enum REAL_TIME_DATA_TAB_WIDGET_PAGE{
        TemperatureTabPage = 0,
        RemoteControl,
        TabPage_End

    }realTimeDataTabWidgetPage;

    const QString controlModeString[5] = {
        tr("自动"),
        tr("手动"),
        tr("远程"),
        tr("网络"),
        tr("其它")
    };
    const QString runningStatusString[5] = {
        tr("停止"),
        tr("关闭"),
        tr("加热"),
        tr("吹风"),
        tr("其它")
    };
    typedef struct _heater_real_time_data_temp{
        int temperatureSensor[5];
        //int temperatureSensorBackup;
        int controlMode[5];
        int runningStatus[5];
    }SHeaterRealTimeDataTemp;
    typedef struct _heater_real_time_data_remote_control{
        int remoteControl[HEATER_QUANITY_NUM];
    }SHeaterRealTimeDataRemoteControl;

    explicit CHeaterRealTimeData(QWidget *parent = 0);
    ~CHeaterRealTimeData();

    SHeaterRealTimeDataTemp heaterRealTimeDataTemp[FAN_TOWER_GROUP];
    SHeaterRealTimeDataRemoteControl heaterRealTimeDataRemoterControl[FAN_TOWER_GROUP];
    SHeaterRealTimeDataRemoteControl realTimeDataRemoterControlSyncToRemoteDevices[FAN_TOWER_GROUP];

signals:
    void sendClientServerCommand(int);
public slots:
    void showHeaterRealTimeDataTemp();
    void showHeaterRealTimeDataRemoteControl();
private slots:
    //void on_heaterGroupSwith_currentIndexChanged(int index);

    void on_apply_released();

    void on_cancle_released();

    void on_realTimeDataTabWidget_currentChanged(int index);

private:
    Ui::CHeaterRealTimeData *ui;

    QLabel *pHeaterTemp[5];
    QLabel *pControlMode[5];
    QLabel *pRunningStatus[5];

    QComboBox *pWorkSwitchGroup[5];
    int m_groupSwith;
};
#endif // CHEATERREALTIMEDATA_H
