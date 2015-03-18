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

    CHeaterMainForm *pHeaterMainForm;
    pStackedWidget = ui->sessionStackContainer;
    pHeaterMainForm = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pHeaterMainForm);

    //connect(pHeaterMainForm, SIGNAL(establishNetworkConnection()), this, SLOT(establishNetworkConnectionHub()));
    //connect(pHeaterMainForm, SIGNAL(disconnectNetwork()), this, SLOT(disconnectNetworkHub()));
    connect(this, SIGNAL(updateControlForm(int)), pHeaterMainForm, SLOT(reflashHeaterControlForm(int)));

    CHeaterRealTimeData *pHeaterRealTimeData;
    pHeaterRealTimeData = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pHeaterRealTimeData);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterRealTimeData, SLOT(reflashHeaterControlForm(int)));

    CHeaterParameterSettings *pHeaterParameterSettings;
    pHeaterParameterSettings = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pHeaterParameterSettings);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterParameterSettings, SLOT(reflashHeaterControlForm(int)));

    CHeaterFaultInfo *pHeaterFaultInfo;
    pHeaterFaultInfo = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pHeaterFaultInfo);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterFaultInfo, SLOT(reflashHeaterControlForm(int)));

    CHeaterHistoryRecord *pHeaterHistoryRecord;
    pHeaterHistoryRecord = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pHeaterHistoryRecord);
    connect(this, SIGNAL(updateControlForm(int)), pHeaterHistoryRecord, SLOT(reflashHeaterControlForm(int)));


    pStackedWidget->setCurrentIndex(MainForm);
    emit updateControlForm(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));

    pHeaterClientServer[0] = new CHeaterClientServer("192.168.1.111");
    pHeaterClientServer[0]->setHeaterInfoPreview(&(heaterInfoPreview[0]));
    pHeaterClientServer[0]->start();
    connect(pHeaterClientServer[0], SIGNAL(updateHeaterClientForm(int)), this, SLOT(reflashHeaterControlForm(int)));

}

CHeaterClient::~CHeaterClient()
{
    delete ui;
}
void CHeaterClient::functionSwitchButtonGroupStatusChanged(int id)
{
    pStackedWidget->setCurrentIndex(-(id + MainForm));
    emit updateControlForm(-(id + MainForm));
}

void CHeaterClient::reflashHeaterControlForm(int index)
{
    if(CHeaterClient::InfoPreview == index){showHeaterUnitCurrentStatus();}
    else {emit updateControlForm(index);}
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




