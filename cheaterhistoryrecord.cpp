#include "cheaterhistoryrecord.h"
#include "ui_cheaterhistoryrecord.h"

#include "heatermodbusoffsetaddress.h"

#include "cheaterclient.h"

#include <QDate>
#include <QDateTime>

CHeaterHistoryRecord::CHeaterHistoryRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterHistoryRecord)
{
    ui->setupUi(this);
}

CHeaterHistoryRecord::~CHeaterHistoryRecord()
{
    delete ui;
}
void CHeaterHistoryRecord::setModbusMaster(QModbusMaster *modbusMaster)
{
    pModbusMaster = modbusMaster;
}



void CHeaterHistoryRecord::updateHeaterHistoryRecord()
{
    //work record
    QModbusRegisters workRecordRegisters(HEATER_WORK_RECORD_BASE, HEATER_WORK_RECORD_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(workRecordRegisters);
    pModbusMaster->close();
    //fault record
    QModbusRegisters faultRecordRegisters(HEATER_FAULT_RECORD_BASE, HEATER_FAULT_RECORD_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(faultRecordRegisters);
    pModbusMaster->close();

    for(int i = 0; i  < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        m_workRecord[i] = QString::number((i + 1));
        m_faultRecord[i] = QString::number((i + 1));
        for(int j = 0; j < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH; j ++){
            m_workRecord[i] +=QString::number(( workRecordRegisters.getInteger16(i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH + j)))
                    + "--";
            m_faultRecord[i] +=QString::number(( faultRecordRegisters.getInteger16(i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH + j)))
                    + "--";
        }
    }


}
void CHeaterHistoryRecord::showHeaterHistoryRecord()
{
    ui->workRecord->clear();
    ui->workRecord->clear();
    for(int i = 0; i < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        ui->workRecord->addItem(m_workRecord[i]);
        ui->faultRecord->addItem(m_faultRecord[i]);
    };
}


void CHeaterHistoryRecord::reflashHeaterControlForm(int index)
{
    if(CHeaterClient::HistoryRecord == index){updateHeaterHistoryRecord();}
}

void CHeaterHistoryRecord::on_read_released()
{
    QModbusRegisters recordTimeRegisters(HEATER_HISTORY_RECORD_BASE, HEATER_HISTORY_RECORD_ITEM_LENGTH);
    int time[HEATER_HISTORY_RECORD_TIME_ITEM];
    bool ok;
    time[0] = ui->recordTimeYear->text().toInt(&ok);
    time[1] = ui->recordTimeMonth->text().toInt(&ok);
    time[2] = ui->recordTimeDay->text().toInt(&ok);
    time[3] = ui->recordTimeHour->text().toInt(&ok);
    time[4] = ui->recordTimeMinute->text().toInt(&ok);
    time[4] = time[4] - (time[4] % 5);
    for(int i = 0; i < HEATER_HISTORY_RECORD_TIME_ITEM; i ++){
        recordTimeRegisters.setInteger16(i, time[i]);
    }
    pModbusMaster->connect();
    pModbusMaster->writeRegisters(recordTimeRegisters);
    pModbusMaster->close();


}

void CHeaterHistoryRecord::on_cancle_released()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    ui->recordTimeYear->setText(QString::number(date.year()));
    ui->recordTimeMonth->setText(QString::number(date.month()));
    ui->recordTimeDay->setText(QString::number(date.day()));
    ui->recordTimeHour->setText(QString::number(time.hour()));
    ui->recordTimeMinute->setText(QString::number(time.minute()));
}
