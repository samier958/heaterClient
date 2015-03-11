#ifndef CHEATERHISTORYRECORD_H
#define CHEATERHISTORYRECORD_H

#include <QWidget>

#include "qmodbusmaster.h"

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

public slots:
    void reflashHeaterControlForm(int index);

private:
    Ui::CHeaterHistoryRecord *ui;


    QModbusMaster *pModbusMaster;

    int m_time[5];
};

#endif // CHEATERHISTORYRECORD_H
