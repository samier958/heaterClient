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

    typedef struct _sync_frequency{
        int infoPreviewFrequency;
        int formFrequency;
    }SSyncFrequency;

    CHeaterClientServer(QString ipAddr, QObject *parent = 0);
    ~CHeaterClientServer();
    void setHeaterInfoPreview(CHeaterClient::SHeaterInfoPreview *heaterInfoPreview);

signals:
    void updateHeaterClientForm(int);
protected:
    virtual void run();
private:
    QModbusMaster *pModbusMaster;

    SSyncFrequency syncFrequency;
    CHeaterClient::SHeaterInfoPreview *pHeaterInfoPreview;
    CHeaterRealTimeData::SHeaterRealTimeData *pHeaterRealTimeData;
    CHeaterParameterSettings::SHeaterParameterSettings *pHeaterParameterSettings;
    CHeaterFaultInfo::SHeaterFaultInfo *pHeaterFaultInfo;
};

#endif // CHEATERCLIENTSERVER_H
