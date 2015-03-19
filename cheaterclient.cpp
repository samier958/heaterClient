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

CHeaterClient::CHeaterClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterClient)
{
    ui->setupUi(this);
    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        heaterInfoPreview[i].networkStatus = false;
        heaterInfoPreview[i].averageTemperature = 0;
        heaterInfoPreview[i].faultStatus = false;
    }

    pStackedWidget = ui->sessionStackContainer;
    pHeaterMainForm = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pHeaterMainForm);

    //connect(pHeaterMainForm, SIGNAL(establishNetworkConnection()), this, SLOT(establishNetworkConnectionHub()));
    //connect(pHeaterMainForm, SIGNAL(disconnectNetwork()), this, SLOT(disconnectNetworkHub()));
    //connect(this, SIGNAL(updateControlForm(int)), pHeaterMainForm, SLOT(reflashHeaterControlForm(int)));

    pHeaterRealTimeData = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pHeaterRealTimeData);

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

    pHeaterClientServer[0] = new CHeaterClientServer("192.168.1.111");
    pHeaterClientServer[0]->setHeaterInfoPreview(&(this->heaterInfoPreview[0]));
    pHeaterClientServer[0]->setHeaterRealTimeData(&(pHeaterRealTimeData->heaterRealTimeData[0]));
    pHeaterClientServer[0]->setHeaterParameterSettings(&(pHeaterParameterSettings->heaterParameterSetting[0]));
    pHeaterClientServer[0]->setParameterSettingSyncToRemoteDevices(&(pHeaterParameterSettings->parameterSettingSyncToRemoteDevices[0]));
    pHeaterClientServer[0]->setHeaterFaultInfo(&(pHeaterFaultInfo->heaterFaultInfo[0]));
    pHeaterClientServer[0]->setHeaterHistoryRecordFixedTime(&(pHeaterHistoryRecord->heaterHistoryRecordFixedTime[0]));
    pHeaterClientServer[0]->setHeaterHistoryRecordWorkOrFault(&(pHeaterHistoryRecord->heaterHistoryRecordWorkOrFault[0]));
    pHeaterClientServer[0]->start();


    connect(pHeaterClientServer[0], SIGNAL(clientServerCommandComplete(int)), this, SLOT(showHeaterClientCommandComplete(int)));
    connect(this, SIGNAL(sendClientServerCommand(int)), pHeaterClientServer[0], SLOT(clientServerCommandExecute(int)));

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
             emit sendClientServerCommand(CHeaterClientServer::RealTimeDataReadCmd);
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
    case CHeaterClientServer::RealTimeDataReadCmd:
         pHeaterRealTimeData->showHeaterRealTimeData();
         break;
    case CHeaterClientServer::ParameterSettingReadCmd:
         pHeaterParameterSettings->showHeaterParameterSettings();
         break;
    case CHeaterClientServer::ParameterSettingWriteCmd:
        qDebug()<<"Parameter Setting Write.";
         break;
    case CHeaterClientServer::FaultInfoReadCmd:
         pHeaterFaultInfo->showHeaterFaultInfo();
         break;
    case CHeaterClientServer::HistoryRecordFixedTimeCmd:
         break;
    case CHeaterClientServer::HistoryRecordWorkOrFaultCmd:
         pHeaterHistoryRecord->showHeaterHistoryRecord();
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

    if(heaterInfoPreview[0].faultStatus) {ui->faultStatus_0->setText(faultStatusString[0]);}
    else {ui->faultStatus_0->setText(faultStatusString[1]);}
    if(heaterInfoPreview[1].faultStatus) {ui->faultStatus_1->setText(faultStatusString[0]);}
    else {ui->faultStatus_1->setText(faultStatusString[1]);}
}




