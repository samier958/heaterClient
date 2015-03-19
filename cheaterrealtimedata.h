#ifndef CHEATERREALTIMEDATA_H
#define CHEATERREALTIMEDATA_H

#include <QWidget>

#include "qmodbusmaster.h"
#include "heaterConfig.h"

using namespace Modbus;

namespace Ui {
class CHeaterRealTimeData;
}

class CHeaterRealTimeData : public QWidget
{
    Q_OBJECT
    
public:
    typedef struct _heater_real_time_data{
        int temperatureSensor[5];
        int temperatureSensorBackup;
    }SHeaterRealTimeData;

    explicit CHeaterRealTimeData(QWidget *parent = 0);
    ~CHeaterRealTimeData();

    SHeaterRealTimeData heaterRealTimeData[FAN_TOWER_GROUP];

signals:
    void sendClientServerCommand(int);
public slots:
    void showHeaterRealTimeData();
private slots:
    void on_heaterGroupSwith_currentIndexChanged(int index);

private:
    Ui::CHeaterRealTimeData *ui;

    int m_groupSwith;
};
#endif // CHEATERREALTIMEDATA_H
