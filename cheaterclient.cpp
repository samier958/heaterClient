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

    //pModbusMaster = new QModbusMaster("127.0.0.1", MODBUS_TCP_PORT);
    pModbusMaster = new QModbusMaster("192.168.1.111", MODBUS_TCP_PORT);
    pModbusMaster->setSlave(MODBUS_SLAVE_ADDR);


    CHeaterMainForm *pHeaterMainForm;
    pStackedWidget = ui->sessionStackContainer;
    pHeaterMainForm = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pHeaterMainForm);
    pHeaterMainForm->setModbusMaster(pModbusMaster);

    connect(pHeaterMainForm, SIGNAL(establishNetworkConnection()), this, SLOT(establishNetworkConnectionHub()));
    connect(pHeaterMainForm, SIGNAL(disconnectNetwork()), this, SLOT(disconnectNetworkHub()));
    connect(this, SIGNAL(updateControlForm(int)), pHeaterMainForm, SLOT(reflashHeaterControlForm(int)));

    CHeaterRealTimeData *pHeaterRealTimeData;
    pHeaterRealTimeData = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pHeaterRealTimeData);
    pHeaterRealTimeData->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterRealTimeData, SLOT(reflashHeaterControlForm(int)));

    CHeaterParameterSettings *pHeaterParameterSettings;
    pHeaterParameterSettings = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pHeaterParameterSettings);
    pHeaterParameterSettings->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterParameterSettings, SLOT(reflashHeaterControlForm(int)));

    CHeaterFaultInfo *pHeaterFaultInfo;
    pHeaterFaultInfo = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pHeaterFaultInfo);
    pHeaterFaultInfo->setModbusMaster(pModbusMaster);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterFaultInfo, SLOT(reflashHeaterControlForm(int)));

    CHeaterHistoryRecord *pHeaterHistoryRecord;
    pHeaterHistoryRecord = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pHeaterHistoryRecord);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterHistoryRecord, SLOT(reflashHeaterControlForm(int)));


    pStackedWidget->setCurrentIndex(MainForm);
    emit updateControlForm(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));




    m_networkStatus = false;
    m_averageTemperature = 0;
    m_faultStatus = false;

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
    if(m_networkStatus) {ui->networkStatus->setText(networkStatusString[0]);}
    else {ui->networkStatus->setText(networkStatusString[1]);}

    QString tempString;
    tempString = QString::number(((double)(m_averageTemperature)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->averageTemperature->setText(tempString);

    if(m_faultStatus) {ui->faultStatus->setText(faultStatusString[0]);}
    else {ui->faultStatus->setText(faultStatusString[1]);}
}

void CHeaterClient::updateHeaterUnitCurrentStatus()
{
    QModbusRegisters averageTemperatureRegister(HEATER_UNIT_AVERAGE_TEMP_ADDR, 1);
    QModbusError modbusErr;

    /*
    pModbusMaster->connect();
    modbusErr = pModbusMaster->lastError();
    if(modbusErr.isValid()) {m_networkStatus = false; return ;}
    pModbusMaster->readInputRegisters(averageTemperatureRegister);
    pModbusMaster->close();
    m_averageTemperature = averageTemperatureRegister.getInteger16(0);
    pModbusMaster->connect();
    QModbusRegisters faultStatusRegister(HEATER_FAULT_STATUS_ADDR, 1);
    for(int i = 0; i < 5; i ++){
        faultStatusRegister.setAddress(HEATER_FAULT_STATUS_ADDR + MODBUS_OFFSET_ADDR * i);
        modbusErr = pModbusMaster->lastError();
        if(modbusErr.isValid()) {m_networkStatus = false; return ;}
        pModbusMaster->readInputRegisters(faultStatusRegister);
        if(faultStatusRegister.getUInteger16(0)){m_faultStatus = true;break;}
        else {m_faultStatus = false;}
    }
    pModbusMaster->close();
    */
    showHeaterUnitCurrentStatus();
}






void CHeaterClient::establishNetworkConnectionHub()
{
    m_networkStatus = true;
}

void CHeaterClient::disconnectNetworkHub()
{
    m_networkStatus = false;
}











