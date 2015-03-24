#ifndef CHEATERCLIENT_H
#define CHEATERCLIENT_H

#include <QWidget>

#include "cheaterclient.h"
#include "heaterConfig.h"

#define NETWORK_ONLINE true
#define NETWORK_OFFLINE false

#define CLIENT_CONFIG_FILE "HeaterClientConfig.ini"

#define LOOP_LOCAL_ADDR "127.0.0.1"

namespace Ui {
class CHeaterClient;
}

class CHeaterMainForm;
class CHeaterRealTimeData;
class CHeaterParameterSettings;
class CHeaterFaultInfo;
class CHeaterHistoryRecord;

class QStackedWidget;
class CHeaterClientServer;

class CHeaterClient : public QWidget
{
    Q_OBJECT
    
public:
    enum CHILD_SEESION_FORM{
        InfoPreview = 0,
        MainForm,
        RealTimeData,
        ParameterSetting,
        FaultInfo,
        HistoryRecord,
        ChildSessionForm_End
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
        bool allowed;
        bool networkStatus;
        int averageTemperature;
        bool faultStatus;
    }SHeaterInfoPreview;

    explicit CHeaterClient(QWidget *parent = 0);
    ~CHeaterClient();
    void showHeaterInfoPreview();
    void loadClientConfig();

    SHeaterInfoPreview heaterInfoPreview[FAN_TOWER_GROUP];

signals:
    void sendClientServerCommand(int);
private slots:
    void functionSwitchButtonGroupStatusChanged(int id);
    void showHeaterClientCommandComplete(int cmd);
    void establishNetworkConnectionHub();
    void disconnectNetworkHub();
    
private:

    Ui::CHeaterClient *ui;

    QString m_clientConfigFile;
    QString m_ipAddr;

    CHeaterMainForm *pHeaterMainForm;
    CHeaterRealTimeData *pHeaterRealTimeData;
    CHeaterParameterSettings *pHeaterParameterSettings;
    CHeaterFaultInfo *pHeaterFaultInfo;
    CHeaterHistoryRecord *pHeaterHistoryRecord;
    QStackedWidget *pStackedWidget;

    CHeaterClientServer *pHeaterClientServer[FAN_TOWER_GROUP];
};

#endif // CHEATERCLIENT_H
