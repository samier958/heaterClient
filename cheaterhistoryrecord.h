#ifndef CHEATERHISTORYRECORD_H
#define CHEATERHISTORYRECORD_H

#include <QWidget>

#include "qmodbusmaster.h"


#define HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM 20
#define HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH 6

using namespace Modbus;

namespace Ui {
class CHeaterHistoryRecord;
}



class CHeaterHistoryRecord : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterHistoryRecord(QWidget *parent = 0);
    ~CHeaterHistoryRecord();

    void setModbusMaster(QModbusMaster *modbusMaster);


    void updateHeaterHistoryRecord();
    void showHeaterHistoryRecord();

public slots:
    void reflashHeaterControlForm(int index);

private slots:
    void on_read_released();

    void on_cancle_released();

private:
    Ui::CHeaterHistoryRecord *ui;


    QModbusMaster *pModbusMaster;

    int m_time[5];

    QString m_workRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
    QString m_faultRecord[HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM];
};

#endif // CHEATERHISTORYRECORD_H
