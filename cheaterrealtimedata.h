#ifndef CHEATERREALTIMEDATA_H
#define CHEATERREALTIMEDATA_H

#include <QWidget>

#include "qmodbusmaster.h"

using namespace Modbus;

namespace Ui {
class CHeaterRealTimeData;
}

class CHeaterRealTimeData : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterRealTimeData(QWidget *parent = 0);
    ~CHeaterRealTimeData();

public slots:
    void reflashHeaterControlForm(int index);
private slots:
    void showHeaterRealTimeData();
    void updateHeaterRealTimeData();
private:
    Ui::CHeaterRealTimeData *ui;

    QModbusMaster *pModbusMaster;
    int m_temperatureSensor[5];
    int m_temperatureSensorBackup;
};

#endif // CHEATERREALTIMEDATA_H
