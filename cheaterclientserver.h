#ifndef CHEATERCLIENTSERVER_H
#define CHEATERCLIENTSERVER_H

#include <QThread>

#include "qmodbusmaster.h"
#include "heaterConfig.h"

#include "cheaterclient.h"
#include "cheatermainform.h"
#include "cheaterrealtimedata.h"
#include "cheaterparametersettings.h"
#include "cheaterfaultinfo.h"
#include "cheaterhistoryrecord.h"

#define INFO_PREVIEW_FREQUENCY_NUM 10
#define FORM_FREQUENCY_NUM 5
#define FREQUENCY_RESET_NUM 0

using namespace Modbus;

class CHeaterClient;
class CHeaterRealTimeData;

class CHeaterClientServer : public QThread
{
    Q_OBJECT
public:
    enum CLIENT_SERVER_COMMAND{
        InfoPreviewReadCmd = 0,
        MainFormReadCmd,
        RealTimeDataTempReadCmd,
        RealTimeDataRemoteControlReadCmd,
        RealTimeDataRemoteControlWriteCmd,
        ParameterSettingReadCmd,
        ParameterSettingWriteCmd,
        FaultInfoReadCmd,
        HistoryRecordFixedTimeCmd,
        HistoryRecordWorkOrFaultCmd,
        ClientServerCmd_End
    };

    typedef struct _client_server_command_history{
        int currentCommand;
        int lastCommand;
    }SClientServerCommandHistory;

    CHeaterClientServer(QString ipAddr, QObject *parent = 0);
    ~CHeaterClientServer();
    void setHeaterInfoPreview(CHeaterClient::SHeaterInfoPreview *heaterInfoPreview);
    void setHeaterRealTimeDataTemp(CHeaterRealTimeData::SHeaterRealTimeDataTemp *heaterRealTimeDataTemp);
    void setHeaterRealTimeDataRemoteControl(CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *heaterRealTimeDataRemoterControl);
    void setRealTimeDataRemoterControlSyncToRemoteDevices(CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *realTimeDataRemoterControlSyncToRemoteDevices);


    void setHeaterParameterSettings(CHeaterParameterSettings::SHeaterParameterSettings *heaterParameterSettings);
    void setParameterSettingSyncToRemoteDevices(CHeaterParameterSettings::SHeaterParameterSettings *parameterSettingSyncToRemoteDevices);
    void setHeaterFaultInfo(CHeaterFaultInfo::SHeaterFaultInfo *heaterFaultInfo);
    void setHeaterHistoryRecordFixedTime(CHeaterHistoryRecord::SHeaterHistoryRecordFixedTime *heaterHistoryRecordFixedTime);
    void setHeaterHistoryRecordWorkOrFault(CHeaterHistoryRecord::SHeaterHistoryRecordWorkOrFault *heaterHistoryRecordWorkOrFault);

protected:
    virtual void run();
signals:
    void clientServerCommandComplete(int);
private slots:
    void clientServerCommandExecute(int cmd);
private:
    QModbusMaster *pModbusMaster;

    SClientServerCommandHistory clientServerCommandHistory;
    CHeaterClient::SHeaterInfoPreview *pHeaterInfoPreview;
    CHeaterRealTimeData::SHeaterRealTimeDataTemp *pHeaterRealTimeDataTemp;
    CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *pHeaterRealTimeDataRemoterControl;
    CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *pRealTimeDataRemoterControlSyncToRemoteDevices;
    CHeaterParameterSettings::SHeaterParameterSettings *pHeaterParameterSettings;
    CHeaterParameterSettings::SHeaterParameterSettings *pParameterSettingSyncToRemoteDevices;
    CHeaterFaultInfo::SHeaterFaultInfo *pHeaterFaultInfo;
    CHeaterHistoryRecord::SHeaterHistoryRecordFixedTime *pHeaterHistoryRecordFixedTime;
    CHeaterHistoryRecord::SHeaterHistoryRecordWorkOrFault *pHeaterHistoryRecordWorkOrFault;
};

#endif // CHEATERCLIENTSERVER_H
