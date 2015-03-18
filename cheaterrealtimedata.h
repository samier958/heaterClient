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

public slots:
    void reflashHeaterControlForm(int index);
private slots:
    void showHeaterRealTimeData();
    void on_heaterGroupSwith_currentIndexChanged(int index);

private:
    Ui::CHeaterRealTimeData *ui;

    int m_groupSwith;
    SHeaterRealTimeData heaterRealTimeData[FAN_TOWER_GROUP];
};
#endif // CHEATERREALTIMEDATA_H
