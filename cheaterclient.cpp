#include "cheaterclient.h"
#include "ui_cheaterclient.h"

#include "cheatermainform.h"
#include "cheaterrealtimedata.h"
#include "cheaterparametersettings.h"
#include "cheaterfaultinfo.h"
#include "cheaterhistoryrecord.h"

#include "cheaterclientserver.h"

#include <QStackedWidget>

#include <QDebug>

#include <QStandardPaths>
#include <QFile>

CHeaterClient::CHeaterClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterClient)
{
    ui->setupUi(this);


    m_clientConfigFile = /*QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + '/' +*/ CLIENT_CONFIG_FILE;
    loadClientConfig();

    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        heaterInfoPreview[i].networkStatus = false;
        heaterInfoPreview[i].averageTemperature = 0;
        heaterInfoPreview[i].faultStatus = false;
    }
    showHeaterInfoPreview();
    pStackedWidget = ui->sessionStackContainer;
    pHeaterMainForm = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pHeaterMainForm);

    connect(pHeaterMainForm, SIGNAL(establishNetworkConnection()), this, SLOT(establishNetworkConnectionHub()));
    connect(pHeaterMainForm, SIGNAL(disconnectNetwork()), this, SLOT(disconnectNetworkHub()));

    pHeaterRealTimeData = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pHeaterRealTimeData);
    connect(pHeaterRealTimeData, SIGNAL(sendClientServerCommand(int, int)), this, SIGNAL(sendClientServerCommand(int, int)));

    pHeaterParameterSettings = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pHeaterParameterSettings);
    connect(pHeaterParameterSettings, SIGNAL(sendClientServerCommand(int, int)), this, SIGNAL(sendClientServerCommand(int, int)));

    pHeaterFaultInfo = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pHeaterFaultInfo);

    pHeaterHistoryRecord = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pHeaterHistoryRecord);
    connect(pHeaterHistoryRecord, SIGNAL(sendClientServerCommand(int, int)), this, SIGNAL(sendClientServerCommand(int, int)));

    pStackedWidget->setCurrentIndex(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));

    connect(this, SIGNAL(sendClientServerCommand(int, int)), this, SLOT(clientServerCommand(int, int)));

    for(int i = 0 ;i < FAN_TOWER_GROUP; i ++){
        //pHeaterClientServer[0] = new CHeaterClientServer("192.168.1.111");
        pHeaterClientServer[i] = new CHeaterClientServer(m_ipAddr[i]);
        pHeaterClientServer[i]->setHeaterInfoPreview(&(this->heaterInfoPreview[i]));
        pHeaterClientServer[i]->setHeaterRealTimeDataTemp(&(pHeaterRealTimeData->heaterRealTimeDataTemp[i]));
        pHeaterClientServer[i]->setHeaterRealTimeDataRemoteControl(&(pHeaterRealTimeData->heaterRealTimeDataRemoterControl[i]));
        pHeaterClientServer[i]->setRealTimeDataRemoterControlSyncToRemoteDevices(&(pHeaterRealTimeData->realTimeDataRemoterControlSyncToRemoteDevices[i]));
        pHeaterClientServer[i]->setHeaterParameterSettings(&(pHeaterParameterSettings->heaterParameterSetting[i]));
        pHeaterClientServer[i]->setParameterSettingSyncToRemoteDevices(&(pHeaterParameterSettings->parameterSettingSyncToRemoteDevices[i]));
        pHeaterClientServer[i]->setHeaterFaultInfo(&(pHeaterFaultInfo->heaterFaultInfo[i]));
        pHeaterClientServer[i]->setHeaterHistoryRecordFixedTime(&(pHeaterHistoryRecord->heaterHistoryRecordFixedTime[i]));
        pHeaterClientServer[i]->setHeaterHistoryRecordWorkOrFault(&(pHeaterHistoryRecord->heaterHistoryRecordWorkOrFault[i]));
        pHeaterClientServer[i]->start();

        connect(pHeaterClientServer[i], SIGNAL(clientServerCommandComplete(int)), this, SLOT(showHeaterClientCommandComplete(int)));
        //connect(this, SIGNAL(sendClientServerCommand(int)), pHeaterClientServer[i], SLOT(clientServerCommandExecute(int)));
    }

    setFixedSize(800, 600);
    setWindowTitle("风电机舱加热远程控制系统    德阳智科电子");

    //emit sendClientServerCommand(0, CHeaterClientServer::SystimeTimeCalibrationCmd);
}

CHeaterClient::~CHeaterClient()
{
    delete ui;
}
void CHeaterClient::functionSwitchButtonGroupStatusChanged(int id)
{
    pStackedWidget->setCurrentIndex(-(id + MainForm));
    switch ((-(id + MainForm))) {
        case MainForm:
             emit sendClientServerCommand(0, CHeaterClientServer::MainFormReadCmd);
             break;
        case RealTimeData:
             emit sendClientServerCommand(0, CHeaterClientServer::RealTimeDataTempReadCmd);
             break;
        case ParameterSetting:
             emit sendClientServerCommand(0, CHeaterClientServer::ParameterSettingReadCmd);
             break;
        case FaultInfo:
             emit sendClientServerCommand(0, CHeaterClientServer::FaultInfoReadCmd);
             break;
        case HistoryRecord:
             emit sendClientServerCommand(0, CHeaterClientServer::HistoryRecordWorkOrFaultCmd);
             break;
        default:
            break;
    }
}

void CHeaterClient::clientServerCommand(int index, int cmd)
{
    pHeaterClientServer[index]->clientServerCommandExecute(cmd);
}
void CHeaterClient::showHeaterClientCommandComplete(int cmd)
{
    //qDebug()<<"Client Show Form:"<<cmd;
    switch (cmd) {
        case CHeaterClientServer::InfoPreviewReadCmd:
             this->showHeaterInfoPreview();
             break;
        case CHeaterClientServer::MainFormReadCmd:
             break;
        case CHeaterClientServer::RealTimeDataTempReadCmd:
             pHeaterRealTimeData->showHeaterRealTimeDataTemp();
             break;
        case CHeaterClientServer::RealTimeDataRemoteControlReadCmd:
             pHeaterRealTimeData->showHeaterRealTimeDataRemoteControl();
             break;
        case CHeaterClientServer::RealTimeDataRemoteControlWriteCmd:
             qDebug()<<"Real Time Data Remote Control Write Complete.";
             break;
        case CHeaterClientServer::ParameterSettingReadCmd:
             pHeaterParameterSettings->showHeaterParameterSettings();
             break;
        case CHeaterClientServer::ParameterSettingWriteCmd:
             qDebug()<<"Parameter Setting Write Complete.";
             break;
        case CHeaterClientServer::FaultInfoReadCmd:
             pHeaterFaultInfo->showHeaterFaultInfo();
             break;
        case CHeaterClientServer::HistoryRecordFixedTimeCmd:
             pHeaterHistoryRecord->showHeaterHistoryRecordFixedTime();
             break;
        case CHeaterClientServer::HistoryRecordWorkOrFaultCmd:
             pHeaterHistoryRecord->showHeaterHistoryRecordWorkOrFault();
             break;
        default:
             break;
    }
}

void CHeaterClient::showHeaterInfoPreview()
{
    if(heaterInfoPreview[0].networkStatus) {ui->networkStatus_0->setText(networkStatusString[0]);}
    else {ui->networkStatus_0->setText(networkStatusString[1]);}
    if(heaterInfoPreview[1].networkStatus) {ui->networkStatus_1->setText(networkStatusString[0]);}
    else {ui->networkStatus_1->setText(networkStatusString[1]);}

    QString tempString;
    tempString = QString::number(((double)(heaterInfoPreview[0].averageTemperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->averageTemperature_0->setText(tempString);
    tempString = QString::number(((double)(heaterInfoPreview[1].averageTemperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->averageTemperature_1->setText(tempString);

    if(!(heaterInfoPreview[0].faultStatus)) {ui->faultStatus_0->setText(faultStatusString[0]);}
    else {ui->faultStatus_0->setText(faultStatusString[1]);}
    if(!(heaterInfoPreview[1].faultStatus)) {ui->faultStatus_1->setText(faultStatusString[0]);}
    else {ui->faultStatus_1->setText(faultStatusString[1]);}
}


void CHeaterClient::loadClientConfig()
{
    QFile clientConfigFile(m_clientConfigFile);
    if (!clientConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) { m_ipAddr[0] = LOOP_LOCAL_ADDR; m_ipAddr[1] = LOOP_LOCAL_ADDR; return; }

    QTextStream in(&clientConfigFile);
      while (!in.atEnd()) {
          QString configLine = in.readLine();
          if(configLine.isEmpty() || '#' == configLine[0] || '$' == configLine[0]) { continue; }
          QStringList configList = configLine.split('=');
          if(configList.at(0) == "IP_Addr_1") {m_ipAddr[0] = configList.at(1);}
          if(configList.at(0) == "IP_Addr_2") {m_ipAddr[1] = configList.at(1);}
      }
      if(m_ipAddr[0].isEmpty()){m_ipAddr[0] = LOOP_LOCAL_ADDR;}
      if(m_ipAddr[1].isEmpty()){m_ipAddr[1] = LOOP_LOCAL_ADDR;}
      qDebug()<<"IP Addr Group:"<<m_ipAddr[0] << "&" <<m_ipAddr[1];
}


void CHeaterClient::establishNetworkConnectionHub()
{
    heaterInfoPreview[0].allowed = true;
    heaterInfoPreview[1].allowed = true;
}
void CHeaterClient::disconnectNetworkHub()
{
    heaterInfoPreview[0].allowed = false;
    heaterInfoPreview[1].allowed = false;
}

