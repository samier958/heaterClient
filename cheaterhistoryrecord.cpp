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

    m_groupSwith = 0;

    for(int i = 0; i < FAN_TOWER_GROUP; i ++){
        memset(&(heaterHistoryRecordWorkOrFault[i]), 0, sizeof(heaterHistoryRecordWorkOrFault[i]));
        memset(&(heaterHistoryRecordFixedTime[i]), 0, sizeof(heaterHistoryRecordFixedTime[i]));
        heaterHistoryRecordFixedTime[i].valid = 1;
        heaterHistoryRecordFixedTime[i].wait = 0;
        heaterHistoryRecordFixedTime[i].index = 0;
    }

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
    if(heaterHistoryRecordFixedTime[m_groupSwith].temperature < -1000 || heaterHistoryRecordFixedTime[m_groupSwith].temperature > 1000){heaterHistoryRecordFixedTime[m_groupSwith].temperature = 0;}
    if(heaterHistoryRecordFixedTime[m_groupSwith].controlMode < -1 || heaterHistoryRecordFixedTime[m_groupSwith].controlMode > 5){heaterHistoryRecordFixedTime[m_groupSwith].controlMode = 4;}
    if(heaterHistoryRecordFixedTime[m_groupSwith].runningStatus < -1 || heaterHistoryRecordFixedTime[m_groupSwith].runningStatus > 5){heaterHistoryRecordFixedTime[m_groupSwith].temperature = 4;}
    if(heaterHistoryRecordFixedTime[m_groupSwith].faultInfo < -1 || heaterHistoryRecordFixedTime[m_groupSwith].faultInfo > 8){heaterHistoryRecordFixedTime[m_groupSwith].temperature = 7;}
    QString tempString;
    tempString = QString::number(((double)(heaterHistoryRecordFixedTime[m_groupSwith].temperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->temperature->setText(tempString);

    ui->controlMode->setText(controlModeString[heaterHistoryRecordFixedTime[m_groupSwith].controlMode]);
    ui->runningStatus->setText(runningStatusString[heaterHistoryRecordFixedTime[m_groupSwith].runningStatus]);
    for(int i = 0; i < 7; i ++){
        if(!(heaterHistoryRecordFixedTime[m_groupSwith].faultInfo)){ui->faultInfo->setText(faultInfoString[6]);break;}
        if((heaterHistoryRecordFixedTime[m_groupSwith].faultInfo) & (1 << i)){ui->faultInfo->setText(faultInfoString[i]);}
    }
    if((heaterHistoryRecordFixedTime[m_groupSwith].inputPort & 0x0001)){ui->fanPowerPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->fanPowerPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[m_groupSwith].inputPort & 0x0002)){ui->ptcPowerPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->ptcPowerPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[m_groupSwith].inputPort & 0x0003)){ui->externalBurdenPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->externalBurdenPort->setPixmap(*pRedCirclePixmap);}

    if((heaterHistoryRecordFixedTime[m_groupSwith].outputPort & 0x0001)){ui->heatingPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->heatingPort->setPixmap(*pRedCirclePixmap);}
    if((heaterHistoryRecordFixedTime[m_groupSwith].outputPort & 0x0002)){ui->radiatingPort->setPixmap(*pGreenCirclePixmap);}
    else {ui->radiatingPort->setPixmap(*pRedCirclePixmap);}
}
void CHeaterHistoryRecord::showHeaterHistoryRecordWorkOrFault()
{
    currentTimeSetToRecord();
    ui->workRecord->clear();
    ui->faultRecord->clear();

    for(int i = 0; i  < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        if(!(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].year)){break;}
        m_workRecord[i]  = QString::number((i + 1)) + ".";
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].year)  + timeString[0];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].month) + timeString[1];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].day)   + timeString[2];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].hour)  + timeString[3];
        m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].min)   + timeString[4];
        //m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[0].workRecord[i].code);

        if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code == 0){
            m_workRecord[i] += workOrFaultRecordString[0];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code == 1){
            m_workRecord[i] += workOrFaultRecordString[1];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code == 2){
            m_workRecord[i] += workOrFaultRecordString[2];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code >=3 && heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code <= 7){
            m_workRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code - 2) + workOrFaultRecordString[3];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code >= 10 && heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code <= 55){
            int index = heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code / 10;
            int code = heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code % 10;
            m_workRecord[i] += QString::number(index) + workOrFaultRecordString[4 + code];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code >= 110 && heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code <= 158){
            int index = heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code / 10 - 10;
            int code = heaterHistoryRecordWorkOrFault[m_groupSwith].workRecord[i].code % 10;
            m_workRecord[i] += QString::number(index) + workOrFaultRecordString[10 + code];
        }else{

        }
        ui->workRecord->addItem(m_workRecord[i]);
    }


    for(int i = 0; i  < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
        if(!(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].year)){break;}
        m_faultRecord[i]  = QString::number((i + 1)) + ".";
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].year)  + timeString[0];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].month) + timeString[1];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].day)   + timeString[2];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].hour)  + timeString[3];
        m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].min)   + timeString[4];
        //m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code);

        if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code == 0){
            m_faultRecord[i] += workOrFaultRecordString[0];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code == 1){
            m_faultRecord[i] += workOrFaultRecordString[1];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code == 2){
            m_faultRecord[i] += workOrFaultRecordString[2];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code >=3 && heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code <= 7){
            m_faultRecord[i] += QString::number(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code - 2) + workOrFaultRecordString[3];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code >= 10 && heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code <= 55){
            int index = heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code / 10;
            int code = heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code % 10;
            m_faultRecord[i] += QString::number(index) + workOrFaultRecordString[4 + code];
        }else if(heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code >= 110 && heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code <= 158){
            int index = heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code / 10 - 10;
            int code = heaterHistoryRecordWorkOrFault[m_groupSwith].faultRecord[i].code % 10;
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
    heaterHistoryRecordFixedTime[m_groupSwith].time[0] = ui->recordTimeYear->text().toInt(&ok);
    heaterHistoryRecordFixedTime[m_groupSwith].time[1] = ui->recordTimeMonth->text().toInt(&ok);
    heaterHistoryRecordFixedTime[m_groupSwith].time[2] = ui->recordTimeDay->text().toInt(&ok);
    heaterHistoryRecordFixedTime[m_groupSwith].time[3] = ui->recordTimeHour->text().toInt(&ok);
    heaterHistoryRecordFixedTime[m_groupSwith].time[4] = ui->recordTimeMinute->text().toInt(&ok);
    heaterHistoryRecordFixedTime[m_groupSwith].time[4] = heaterHistoryRecordFixedTime[m_groupSwith].time[4] - (heaterHistoryRecordFixedTime[m_groupSwith].time[4] % 5);

    heaterHistoryRecordFixedTime[m_groupSwith].valid = 0;
    heaterHistoryRecordFixedTime[m_groupSwith].wait = 0;

    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::HistoryRecordFixedTimeCmd);
}

void CHeaterHistoryRecord::on_cancle_released()
{
    currentTimeSetToRecord();
}

void CHeaterHistoryRecord::on_hearterFixedTimeIndex_currentIndexChanged(int index)
{
    heaterHistoryRecordFixedTime[m_groupSwith].index = index;
}

void CHeaterHistoryRecord::on_clearWorkRecord_released()
{
    ui->workRecord->clear();
    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::HistoryRecordWorkClearCmd);
}

void CHeaterHistoryRecord::on_clearFaultRecord_released()
{
    ui->faultRecord->clear();
    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::HistoryRecordFaultClearCmd);
}

void CHeaterHistoryRecord::on_heaterGroupSwith_currentIndexChanged(int index)
{
    m_groupSwith = index;
    //showHeaterHistoryRecordWorkOrFault();
    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::HistoryRecordWorkOrFaultCmd);
}
