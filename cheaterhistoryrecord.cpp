#include "cheaterhistoryrecord.h"
#include "ui_cheaterhistoryrecord.h"

#include "heaterConfig.h"

#include "cheaterclient.h"
#include "cheaterclientserver.h"

#include <QDate>
#include <QDateTime>

#include <QDebug>

CHeaterHistoryRecord::CHeaterHistoryRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterHistoryRecord)
{
    ui->setupUi(this);
    memset(&(heaterHistoryRecordWorkOrFault[0]), 0, sizeof(heaterHistoryRecordWorkOrFault[0]));
    memset(&(heaterHistoryRecordFixedTime[0]), 0, sizeof(heaterHistoryRecordFixedTime[0]));
    heaterHistoryRecordFixedTime[0].valid = 1;
    heaterHistoryRecordFixedTime[0].wait = 0;
    heaterHistoryRecordFixedTime[0].index = 0;

    pGreenCirclePixmap = new QPixmap(":/res/res/greenCircle.png");
    pRedCirclePixmap = new QPixmap(":/res/res/redCircle.png");

    showHeaterHistoryRecordWorkOrFault();
}

CHeaterHistoryRecord::~CHeaterHistoryRecord()
{
    delete ui;
}

void CHeaterHistoryRecord::showHeaterHistoryRecordFixedTime()
{
    if(heaterHistoryRecordFixedTime[0].temperature < -1000 || heaterHistoryRecordFixedTime[0].temperature > 1000){heaterHistoryRecordFixedTime[0].temperature = 0;}
    if(heaterHistoryRecordFixedTime[0].controlMode < -1 || heaterHistoryRecordFixedTime[0].controlMode > 5){heaterHistoryRecordFixedTime[0].controlMode = 4;}
    if(heaterHistoryRecordFixedTime[0].runningStatus < -1 || heaterHistoryRecordFixedTime[0].runningStatus > 5){heaterHistoryRecordFixedTime[0].temperature = 4;}
    if(heaterHistoryRecordFixedTime[0].faultInfo < -1 || heaterHistoryRecordFixedTime[0].faultInfo > 8){heaterHistoryRecordFixedTime[0].temperature = 7;}
    QString tempString;
    tempString = QString::number(((double)(heaterHistoryRecordFixedTime[0].temperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->temperature->setText(tempString);

    ui->controlMode->setText(controlModeString[heaterHistoryRecordFixedTime[0].controlMode]);
    ui->runningStatus->setText(runningStatusString[heaterHistoryRecordFixedTime[0].runningStatus]);
    for(int i = 0; i < 7; i ++){
        if(!(heaterHistoryRecordFixedTime[0].faultInfo)){ui->faultInfo->setText(faultInfoString[6]);break;}
        if((heaterHistoryRecordFixedTime[0].faultInfo) & (1 << i)){ui->faultInfo->setText(faultInfoString[i]);}
    }
    if((heaterHistoryRecordFixedTime[0].inputPort & 0x0001)){ui->fanPowerPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->fanPowerPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[0].inputPort & 0x0002)){ui->ptcPowerPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->ptcPowerPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[0].inputPort & 0x0003)){ui->externalBurdenPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->externalBurdenPort->setPixmap(*pRedCirclePixmap);}

    if((heaterHistoryRecordFixedTime[0].outputPort & 0x0001)){ui->heatingPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->heatingPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[0].outputPort & 0x0002)){ui->radiatingPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->radiatingPort->setPixmap(*pRedCirclePixmap);}
}
void CHeaterHistoryRecord::showHeaterHistoryRecordWorkOrFault()
{
    currentTimeSetToRecord();
    ui->workRecord->clear();
    ui->faultRecord->clear();

    for(int i = 0; i  < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        if(!(heaterHistoryRecordWorkOrFault[0].workRecord[i].year)){break;}
        m_workRecord[i]  = QString::number((i + 1)) + ".";
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].year)  + timeString[0];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].month) + timeString[1];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].day)   + timeString[2];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].hour)  + timeString[3];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].min)   + timeString[4];
        //m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].code);

        if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code == 0){
            m_workRecord[i] += workOrFaultRecordString[0];
        }else if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code == 1){
            m_workRecord[i] += workOrFaultRecordString[1];
        }else if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code == 2){
            m_workRecord[i] += workOrFaultRecordString[2];
        }else if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code >=3 && heaterHistoryRecordWorkOrFault[0].workRecord[i].code <= 7){
            m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].code - 2) + workOrFaultRecordString[3];
        }else if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code >= 10 && heaterHistoryRecordWorkOrFault[0].workRecord[i].code <= 55){
            int index = heaterHistoryRecordWorkOrFault[0].workRecord[i].code / 10;
            int code = heaterHistoryRecordWorkOrFault[0].workRecord[i].code % 10;
            m_workRecord[i] += QString::number(index) + workOrFaultRecordString[4 + code];
        }else if(heaterHistoryRecordWorkOrFault[0].workRecord[i].code >= 110 && heaterHistoryRecordWorkOrFault[0].workRecord[i].code <= 158){
            int index = heaterHistoryRecordWorkOrFault[0].workRecord[i].code / 10 - 10;
            int code = heaterHistoryRecordWorkOrFault[0].workRecord[i].code % 10;
            m_workRecord[i] += QString::number(index) + workOrFaultRecordString[10 + code];
        }else{

        }
        ui->workRecord->addItem(m_workRecord[i]);
    }


    for(int i = 0; i  < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        if(!(heaterHistoryRecordWorkOrFault[0].faultRecord[i].year)){break;}
        m_faultRecord[i]  = QString::number((i + 1)) + ".";
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].year)  + timeString[0];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].month) + timeString[1];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].day)   + timeString[2];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].hour)  + timeString[3];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].min)   + timeString[4];
        //m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code);

        if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code == 0){
            m_faultRecord[i] += workOrFaultRecordString[0];
        }else if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code == 1){
            m_faultRecord[i] += workOrFaultRecordString[1];
        }else if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code == 2){
            m_faultRecord[i] += workOrFaultRecordString[2];
        }else if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code >=3 && heaterHistoryRecordWorkOrFault[0].faultRecord[i].code <= 7){
            m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code - 2) + workOrFaultRecordString[3];
        }else if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code >= 10 && heaterHistoryRecordWorkOrFault[0].faultRecord[i].code <= 55){
            int index = heaterHistoryRecordWorkOrFault[0].faultRecord[i].code / 10;
            int code = heaterHistoryRecordWorkOrFault[0].faultRecord[i].code % 10;
            m_faultRecord[i] += QString::number(index) + workOrFaultRecordString[4 + code];
        }else if(heaterHistoryRecordWorkOrFault[0].faultRecord[i].code >= 110 && heaterHistoryRecordWorkOrFault[0].faultRecord[i].code <= 158){
            int index = heaterHistoryRecordWorkOrFault[0].faultRecord[i].code / 10 - 10;
            int code = heaterHistoryRecordWorkOrFault[0].faultRecord[i].code % 10;
            m_faultRecord[i] += QString::number(index) + workOrFaultRecordString[5 + code];
        }else{

        }
        ui->faultRecord->addItem(m_faultRecord[i]);

    }

}


void CHeaterHistoryRecord::currentTimeSetToRecord()
{
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();
    ui->recordTimeYear->setText(QString::number(date.year()));
    ui->recordTimeMonth->setText(QString::number(date.month()));
    ui->recordTimeDay->setText(QString::number(date.day()));
    ui->recordTimeHour->setText(QString::number(time.hour()));
    ui->recordTimeMinute->setText(QString::number(time.minute()));

}

void CHeaterHistoryRecord::on_read_released()
{
    bool ok;
    heaterHistoryRecordFixedTime[0].time[0] = ui->recordTimeYear->text().toInt(&ok);
    heaterHistoryRecordFixedTime[0].time[1] = ui->recordTimeMonth->text().toInt(&ok);
    heaterHistoryRecordFixedTime[0].time[2] = ui->recordTimeDay->text().toInt(&ok);
    heaterHistoryRecordFixedTime[0].time[3] = ui->recordTimeHour->text().toInt(&ok);
    heaterHistoryRecordFixedTime[0].time[4] = ui->recordTimeMinute->text().toInt(&ok);
    heaterHistoryRecordFixedTime[0].time[4] = heaterHistoryRecordFixedTime[0].time[4] - (heaterHistoryRecordFixedTime[0].time[4] % 5);

    heaterHistoryRecordFixedTime[0].valid = 0;
    heaterHistoryRecordFixedTime[0].wait = 0;

    emit sendClientServerCommand(CHeaterClientServer::HistoryRecordFixedTimeCmd);
}

void CHeaterHistoryRecord::on_cancle_released()
{
    currentTimeSetToRecord();
}

void CHeaterHistoryRecord::on_hearterFixedTimeIndex_currentIndexChanged(int index)
{
    heaterHistoryRecordFixedTime[0].index = index;
}

void CHeaterHistoryRecord::on_clearWorkRecord_released()
{
    ui->workRecord->clear();
    emit sendClientServerCommand(CHeaterClientServer::HistoryRecordWorkClearCmd);
}

void CHeaterHistoryRecord::on_clearFaultRecord_released()
{
    ui->faultRecord->clear();
    emit sendClientServerCommand(CHeaterClientServer::HistoryRecordFaultClearCmd);
}
