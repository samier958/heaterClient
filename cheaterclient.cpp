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
    connect(pHeaterRealTimeData, SIGNAL(sendClientServerCommand(int)), this, SIGNAL(sendClientServerCommand(int)));

    pHeaterParameterSettings = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pHeaterParameterSettings);
    connect(pHeaterParameterSettings, SIGNAL(sendClientServerCommand(int)), this, SIGNAL(sendClientServerCommand(int)));

    pHeaterFaultInfo = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pHeaterFaultInfo);

    pHeaterHistoryRecord = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pHeaterHistoryRecord);
    connect(pHeaterHistoryRecord, SIGNAL(sendClientServerCommand(int)), this, SIGNAL(sendClientServerCommand(int)));

    pStackedWidget->setCurrentIndex(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));

    //pHeaterClientServer[0] = new CHeaterClientServer("192.168.1.111");
    pHeaterClientServer[0] = new CHeaterClientServer(m_ipAddr);
    pHeaterClientServer[0]->setHeaterInfoPreview(&(this->heaterInfoPreview[0]));
    pHeaterClientServer[0]->setHeaterRealTimeDataTemp(&(pHeaterRealTimeData->heaterRealTimeDataTemp[0]));
    pHeaterClientServer[0]->setHeaterRealTimeDataRemoteControl(&(pHeaterRealTimeData->heaterRealTimeDataRemoterControl[0]));
    pHeaterClientServer[0]->setRealTimeDataRemoterControlSyncToRemoteDevices(&(pHeaterRealTimeData->realTimeDataRemoterControlSyncToRemoteDevices[0]));
    pHeaterClientServer[0]->setHeaterParameterSettings(&(pHeaterParameterSettings->heaterParameterSetting[0]));
    pHeaterClientServer[0]->setParameterSettingSyncToRemoteDevices(&(pHeaterParameterSettings->parameterSettingSyncToRemoteDevices[0]));
    pHeaterClientServer[0]->setHeaterFaultInfo(&(pHeaterFaultInfo->heaterFaultInfo[0]));
    pHeaterClientServer[0]->setHeaterHistoryRecordFixedTime(&(pHeaterHistoryRecord->heaterHistoryRecordFixedTime[0]));
    pHeaterClientServer[0]->setHeaterHistoryRecordWorkOrFault(&(pHeaterHistoryRecord->heaterHistoryRecordWorkOrFault[0]));
    pHeaterClientServer[0]->start();


    connect(pHeaterClientServer[0], SIGNAL(clientServerCommandComplete(int)), this, SLOT(showHeaterClientCommandComplete(int)));
    connect(this, SIGNAL(sendClientServerCommand(int)), pHeaterClientServer[0], SLOT(clientServerCommandExecute(int)));

    setFixedSize(800, 600);
    setWindowTitle("风电机舱加热远程控制系统    德阳智科电子");

    emit sendClientServerCommand(CHeaterClientServer::SystimeTimeCalibrationCmd);
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
             emit sendClientServerCommand(CHeaterClientServer::MainFormReadCmd);
             break;
        case RealTimeData:
             emit sendClientServerCommand(CHeaterClientServer::RealTimeDataTempReadCmd);
             break;
        case ParameterSetting:
             emit sendClientServerCommand(CHeaterClientServer::ParameterSettingReadCmd);
             break;
        case FaultInfo:
             emit sendClientServerCommand(CHeaterClientServer::FaultInfoReadCmd);
             break;
        case HistoryRecord:
             emit sendClientServerCommand(CHeaterClientServer::HistoryRecordWorkOrFaultCmd);
             break;
        default:
            break;
    }
}
void CHeaterClient::showHeaterClientCommandComplete(int cmd)
{
    qDebug()<<"Client Show Form:"<<cmd;
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
    //if(heaterInfoPreview[1].networkStatus) {ui->networkStatus_1->setText(networkStatusString[0]);}
    //else {ui->networkStatus_1->setText(networkStatusString[1]);}

    QString tempString;
    tempString = QString::number(((double)(heaterInfoPreview[0].averageTemperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->averageTemperature_0->setText(tempString);
    //tempString = QString::number(((double)(heaterInfoPreview[1].averageTemperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    //ui->averageTemperature_1->setText(tempString);

    if(!(heaterInfoPreview[0].faultStatus)) {ui->faultStatus_0->setText(faultStatusString[0]);}
    else {ui->faultStatus_0->setText(faultStatusString[1]);}
    //if(heaterInfoPreview[1].faultStatus) {ui->faultStatus_1->setText(faultStatusString[0]);}
    //else {ui->faultStatus_1->setText(faultStatusString[1]);}
}


void CHeaterClient::loadClientConfig()
{
    QFile clientConfigFile(m_clientConfigFile);
    if (!clientConfigFile.open(QIODevice::ReadOnly | QIODevice::Text)) { m_ipAddr = LOOP_LOCAL_ADDR; return; }

    QTextStream in(&clientConfigFile);
      while (!in.atEnd()) {
          QString configLine = in.readLine();
          if(configLine.isEmpty() || '#' == configLine[0] || '$' == configLine[0]) { continue; }
          QStringList configList = configLine.split('=');
          if(configList.at(0) == "IP_Addr_1") {m_ipAddr = configList.at(1);}
      }
      if(m_ipAddr.isEmpty()){m_ipAddr = LOOP_LOCAL_ADDR;}
      qDebug()<<"IP Addr:"<<m_ipAddr;
}


void CHeaterClient::establishNetworkConnectionHub()
{
    heaterInfoPreview[0].allowed = true;
}
void CHeaterClient::disconnectNetworkHub()
{
    heaterInfoPreview[0].allowed = false;
}

