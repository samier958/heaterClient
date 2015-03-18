#ifndef CHEATERCLIENT_H
#define CHEATERCLIENT_H

#include <QWidget>

#include "cheaterclient.h"
#include "heaterConfig.h"

#define NETWORK_ONLINE true
#define NETWORK_OFFLINE false


namespace Ui {
class CHeaterClient;
}



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
        bool networkStatus;
        int averageTemperature;
        bool faultStatus;
    }SHeaterInfoPreview;

    explicit CHeaterClient(QWidget *parent = 0);
    ~CHeaterClient();
    void showHeaterUnitCurrentStatus();

signals:
    void updateControlForm(int);
private slots:
    void functionSwitchButtonGroupStatusChanged(int id);
    void reflashHeaterControlForm(int index);
    
private:

    Ui::CHeaterClient *ui;

    QStackedWidget *pStackedWidget;

    SHeaterInfoPreview heaterInfoPreview[FAN_TOWER_GROUP];

    CHeaterClientServer *pHeaterClientServer[FAN_TOWER_GROUP];
};

#endif // CHEATERCLIENT_H
