#ifndef CHEATERCLIENT_H
#define CHEATERCLIENT_H

#include <QWidget>

#include "qmodbusmaster.h"

#define MODBUS_TCP_PORT 502
#define MODBUS_SLAVE_ADDR 6

#define FAN_TOWER_GROUP 2

#define NETWORK_ONLINE true
#define NETWORK_OFFLINE false

using namespace Modbus;

namespace Ui {
class CHeaterClient;
}



class QStackedWidget;
class QTcpSocket;


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

    typedef struct _heater_info_preview{
        QTcpSocket *pTcpSocket;
        bool networkStatus;
        int averageTemperature;
        bool faultStatus;
        QModbusMaster *pModbusMaster;
    }SHeaterInfoPreview;
    explicit CHeaterClient(QWidget *parent = 0);
    ~CHeaterClient();


signals:
    void updateControlForm(int);
private slots:
    void functionSwitchButtonGroupStatusChanged(int id);
    void showHeaterUnitCurrentStatus();
    void updateHeaterUnitCurrentStatus();
    
private:
    Ui::CHeaterClient *ui;


    QStackedWidget *pStackedWidget;

    QTimer *pTimer;

    SHeaterInfoPreview heaterInfoPreview[FAN_TOWER_GROUP];
};

#endif // CHEATERCLIENT_H
