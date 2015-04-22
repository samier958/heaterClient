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
    for(int i = 0; i < FAN_TOWER_GROUP; i ++){
        memset(&(heaterRealTimeDataTemp[i]), 0, sizeof(heaterRealTimeDataTemp[i]));
        memset(&(heaterRealTimeDataRemoterControl[i]), 0, sizeof(heaterRealTimeDataRemoterControl[i]));
        memset(&(realTimeDataRemoterControlSyncToRemoteDevices[i]), 0, sizeof(realTimeDataRemoterControlSyncToRemoteDevices[i]));
    }

    pHeaterTemp[0] = ui->heaterTemp_1;
    pHeaterTemp[1] = ui->heaterTemp_2;
    pHeaterTemp[2] = ui->heaterTemp_3;
    pHeaterTemp[3] = ui->heaterTemp_4;
    pHeaterTemp[4] = ui->heaterTemp_5;

    pControlMode[0] = ui->controlMode_1;
    pControlMode[1] = ui->controlMode_2;
    pControlMode[2] = ui->controlMode_3;
    pControlMode[3] = ui->controlMode_4;
    pControlMode[4] = ui->controlMode_5;

    pRunningStatus[0] = ui->runningStatus_1;
    pRunningStatus[1] = ui->runningStatus_2;
    pRunningStatus[2] = ui->runningStatus_3;
    pRunningStatus[3] = ui->runningStatus_4;
    pRunningStatus[4] = ui->runningStatus_5;

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
    for(int i = 0; i < 5; i ++){
        tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensor[i])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
        pHeaterTemp[i]->setText(tempString);
        if((heaterRealTimeDataTemp[m_groupSwith].controlMode[i] >=0) && (heaterRealTimeDataTemp[m_groupSwith].controlMode[i] <=3)){pControlMode[i]->setText(controlModeString[heaterRealTimeDataTemp[m_groupSwith].controlMode[i]]);}
        else {pControlMode[i]->setText(controlModeString[4]);}
        if((heaterRealTimeDataTemp[m_groupSwith].runningStatus[i] >=0) && (heaterRealTimeDataTemp[m_groupSwith].runningStatus[i] <=3)){pRunningStatus[i]->setText(runningStatusString[heaterRealTimeDataTemp[m_groupSwith].runningStatus[i]]);}
        else {pRunningStatus[i]->setText(runningStatusString[4]);}
        //qDebug()<<"i:"<<i<<"Control Mode:"<<heaterRealTimeDataTemp[m_groupSwith].controlMode[i]<<"Running Status:"<<heaterRealTimeDataTemp[m_groupSwith].runningStatus[i];
    }

    //tempString = QString::number(((double)(heaterRealTimeDataTemp[m_groupSwith].temperatureSensorBackup)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    //ui->tempSensorBackup->setText(tempString);
}

void CHeaterRealTimeData::showHeaterRealTimeDataRemoteControl()
{
    for(int i = 0; i <HEATER_QUANITY_NUM; i ++){
        pWorkSwitchGroup[i]->setCurrentIndex(heaterRealTimeDataRemoterControl[m_groupSwith].remoteControl[i]);
    }
}

void CHeaterRealTimeData::on_heaterGroupSwith_currentIndexChanged(int index)
{
    qDebug()<<"index"<<index;
    m_groupSwith = index;
    showHeaterRealTimeDataTemp();
    showHeaterRealTimeDataRemoteControl();
}

void CHeaterRealTimeData::on_apply_released()
{
    for(int i = 0; i <HEATER_QUANITY_NUM; i ++){
        realTimeDataRemoterControlSyncToRemoteDevices[m_groupSwith].remoteControl[i] = pWorkSwitchGroup[i]->currentIndex();
    }
    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::RealTimeDataRemoteControlWriteCmd);
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
            emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::RealTimeDataTempReadCmd);
            break;
        case RemoteControl:
            emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::RealTimeDataRemoteControlReadCmd);
            break;
        default:
            break;
    }
}
