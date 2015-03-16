#include "cheaterclient.h"
#include "ui_cheaterclient.h"

#include "cheatermainform.h"
#include "cheaterrealtimedata.h"
#include "cheaterparametersettings.h"
#include "cheaterfaultinfo.h"
#include "cheaterhistoryrecord.h"

#include "heatermodbusoffsetaddress.h"

#include <QStackedWidget>
#include <QTimer>

#include <QDebug>

CHeaterClient::CHeaterClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterClient)
{
    ui->setupUi(this);

    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        pTcpSocket[i] = new QTcpSocket;
        heaterInfoPreview[i].networkStatus = false;
        heaterInfoPreview[i].averageTemperature = 0;
        heaterInfoPreview[i].faultStatus = false;
        heaterInfoPreview[i].pModbusMaster = new QModbusMaster("127.0.0.1", MODBUS_TCP_PORT);
        //pModbusMaster = new QModbusMaster("192.168.1.111", MODBUS_TCP_PORT);
        heaterInfoPreview[i].pModbusMaster->setSlave(MODBUS_SLAVE_ADDR);
    }


    CHeaterMainForm *pHeaterMainForm;
    pStackedWidget = ui->sessionStackContainer;
    pHeaterMainForm = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pHeaterMainForm);
    //pHeaterMainForm->setModbusMaster(pModbusMaster);

    connect(pHeaterMainForm, SIGNAL(establishNetworkConnection()), this, SLOT(establishNetworkConnectionHub()));
    connect(pHeaterMainForm, SIGNAL(disconnectNetwork()), this, SLOT(disconnectNetworkHub()));
    connect(this, SIGNAL(updateControlForm(int)), pHeaterMainForm, SLOT(reflashHeaterControlForm(int)));

    CHeaterRealTimeData *pHeaterRealTimeData;
    pHeaterRealTimeData = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pHeaterRealTimeData);
    //pHeaterRealTimeData->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterRealTimeData, SLOT(reflashHeaterControlForm(int)));

    CHeaterParameterSettings *pHeaterParameterSettings;
    pHeaterParameterSettings = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pHeaterParameterSettings);
    pHeaterParameterSettings->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterParameterSettings, SLOT(reflashHeaterControlForm(int)));

    CHeaterFaultInfo *pHeaterFaultInfo;
    pHeaterFaultInfo = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pHeaterFaultInfo);
    //pHeaterFaultInfo->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterFaultInfo, SLOT(reflashHeaterControlForm(int)));

    CHeaterHistoryRecord *pHeaterHistoryRecord;
    pHeaterHistoryRecord = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pHeaterHistoryRecord);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterHistoryRecord, SLOT(reflashHeaterControlForm(int)));


    pStackedWidget->setCurrentIndex(MainForm);
    emit updateControlForm(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));


    pTimer = new QTimer;
    pTimer->start(1000);
    connect(pTimer, SIGNAL(timeout()), this, SLOT(updateHeaterUnitCurrentStatus()));

}

CHeaterClient::~CHeaterClient()
{
    delete ui;
}
void CHeaterClient::functionSwitchButtonGroupStatusChanged(int id)
{
    qDebug()<<-(id + MainForm);
    pStackedWidget->setCurrentIndex(-(id + MainForm));
    emit updateControlForm(-(id + MainForm));
}

void CHeaterClient::showHeaterUnitCurrentStatus()
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

void CHeaterClient::updateHeaterUnitCurrentStatus()
{

    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        heaterInfoPreview[i].pTcpSocket->connectToHost("127.0.0.1", 502);
        if(heaterInfoPreview[i].pTcpSocket->waitForConnected(1000)){heaterInfoPreview[i].networkStatus = NETWORK_ONLINE;}
        else {heaterInfoPreview[i].networkStatus = NETWORK_OFFLINE;}
        heaterInfoPreview[i].pTcpSocket->abort();
    }
    QModbusRegisters averageTemperatureRegister(HEATER_UNIT_AVERAGE_TEMP_ADDR, 1);
    QModbusRegisters faultStatusRegister(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);
    QModbusError modbusErr;


    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        heaterInfoPreview[i].pModbusMaster->connect();
        modbusErr = heaterInfoPreview[i].pModbusMaster->lastError();
        if(modbusErr.isValid()) {heaterInfoPreview[i].networkStatus = NETWORK_OFFLINE; return ;}
        heaterInfoPreview[i].pModbusMaster->readInputRegisters(averageTemperatureRegister);
        heaterInfoPreview[i].pModbusMaster->close();
        heaterInfoPreview[i].averageTemperature = averageTemperatureRegister.getInteger16(0);
    }

    for(int i = 0; i < FAN_TOWER_GROUP; i ++) {
        heaterInfoPreview[i].pModbusMaster->connect();
        heaterInfoPreview[i].pModbusMaster->readInputRegisters(faultStatusRegister);
        if(faultStatusRegister.getUInteger16(0)){heaterInfoPreview[i].networkStatus = NETWORK_OFFLINE;break;}
        else {heaterInfoPreview[i].networkStatus = NETWORK_ONLINE;}
        heaterInfoPreview[i].pModbusMaster->close();
    }

    showHeaterUnitCurrentStatus();
}










