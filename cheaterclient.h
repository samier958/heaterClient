#ifndef CHEATERCLIENT_H
#define CHEATERCLIENT_H

#include <QWidget>

#include "qmodbusmaster.h"

#define MODBUS_TCP_PORT 502
#define MODBUS_SLAVE_ADDR 6

using namespace Modbus;

namespace Ui {
class CHeaterClient;
}



class QStackedWidget;

class CHeaterClient : public QWidget
{
    Q_OBJECT
    
public:
    enum CHILD_SEESION_FORM{
        MainForm = 1,
        RealTimeData,
        ParameterSetting,
        FaultInfo,
        HistoryRecord,
        ChildSessionForm_Num
    };
    QString networkStatusString[3] = {
        tr("连接"),
        tr("断开"),
        tr("其它")
    };
    QString faultStatusString[3] = {
        tr("正常"),
        tr("故障"),
        tr("其它")
    };
    explicit CHeaterClient(QWidget *parent = 0);
    ~CHeaterClient();

    void updateHeaterUnitCurrentStatus();

private slots:
    void functionSwitchButtonGroupStatusChanged(int id);
    void establishNetworkConnectionHub();
    void disconnectNetworkHub();
    void showHeaterUnitCurrentStatus();
    
private:
    Ui::CHeaterClient *ui;


    QStackedWidget *pStackedWidget;

    QTimer *pTimer;

    bool m_networkStatus;
    int m_averageTemperature;
    bool m_faultStatus;

    QModbusMaster *pModbusMaster;
};

#endif // CHEATERCLIENT_H
