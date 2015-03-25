#include "cheaterrealtimedata.h"
#include "ui_cheaterrealtimedata.h"

#include "cheaterclient.h"
#include "cheaterclientserver.h"

#include <QDebug>

CHeaterRealTimeData::CHeaterRealTimeData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterRealTimeData)
{
    ui->setupUi(this);
    m_groupSwith = 0;
    memset(&(heaterRealTimeDataTemp[0]), 0, sizeof(heaterRealTimeDataTemp[0]));
    memset(&(heaterRealTimeDataRemoterControl[0]), 0, sizeof(heaterRealTimeDataRemoterControl[0]));
    memset(&(realTimeDataRemoterControlSyncToRemoteDevices[0]), 0, sizeof(realTimeDataRemoterControlSyncToRemoteDevices[0]));

    pWorkSwitchGroup[0] = ui->workSwitch_1;
    pWorkSwitchGroup[1] = ui->workSwitch_2;
    pWorkSwitchGroup[2] = ui->workSwitch_3;
    pWorkSwitchGroup[3] = ui->workSwitch_4;
    pWorkSwitchGroup[4] = ui->workSwitch_5;

    showHeaterRealTimeDataTemp();
}

CHeaterRealTimeData::~CHeaterRealTimeData()
{
    delete ui;
}



void CHeaterRealTimeData::showHeaterRealTimeDataTemp()
{
    QString tempString;
    tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[0])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_1->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[1])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_2->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[2])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_3->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[3])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_4->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[4])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_5->setText(tempString);

    //tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensorBackup)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    //ui->tempSensorBackup->setText(tempString);
}

void CHeaterRealTimeData::showHeaterRealTimeDataRemoteControl()
{
    for(int i = 0; i <HEATER_QUANITY_NUM; i ++){
        pWorkSwitchGroup[i]->setCurrentIndex(heaterRealTimeDataRemoterControl[0].remoteControl[i]);
    }
}
/*
void CHeaterRealTimeData::on_heaterGroupSwith_currentIndexChanged(int index)
{
    qDebug()<<"index"<<index;
    m_groupSwith = index;
    //showHeaterRealTimeData();
}
*/
void CHeaterRealTimeData::on_apply_released()
{
    for(int i = 0; i <HEATER_QUANITY_NUM; i ++){
        realTimeDataRemoterControlSyncToRemoteDevices[0].remoteControl[i] = pWorkSwitchGroup[i]->currentIndex();
    }
    emit sendClientServerCommand(CHeaterClientServer::RealTimeDataRemoteControlWriteCmd);
}

void CHeaterRealTimeData::on_cancle_released()
{
    //emit sendClientServerCommand(CHeaterClientServer::RealTimeDataRemoteControlReadCmd);
    showHeaterRealTimeDataRemoteControl();
}

void CHeaterRealTimeData::on_realTimeDataTabWidget_currentChanged(int index)
{
    qDebug()<<"Real-time data tab page:"<<index;
    switch (index) {
        case TemperatureTabPage:
            emit sendClientServerCommand(CHeaterClientServer::RealTimeDataTempReadCmd);
            break;
        case RemoteControl:
            emit sendClientServerCommand(CHeaterClientServer::RealTimeDataRemoteControlReadCmd);
            break;
        default:
            break;
    }
}
