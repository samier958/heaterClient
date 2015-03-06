#include "cheaterclient.h"
#include "ui_cheaterclient.h"

#include "cheatermainform.h"
#include "cheaterrealtimedata.h"
#include "cheaterparametersettings.h"
#include "cheaterfaultinfo.h"
#include "cheaterhistoryrecord.h"

#include <QStackedWidget>

#include <QDebug>

CHeaterClient::CHeaterClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterClient)
{
    ui->setupUi(this);

    QWidget *pSession;
    pStackedWidget = ui->sessionStackContainer;
    pSession = new CHeaterMainForm(this);
    pStackedWidget->insertWidget(MainForm, pSession);

    pSession = new CHeaterRealTimeData(this);
    pStackedWidget->insertWidget(RealTimeData, pSession);

    pSession = new CHeaterParameterSettings(this);
    pStackedWidget->insertWidget(ParameterSetting, pSession);

    pSession = new CHeaterFaultInfo(this);
    pStackedWidget->insertWidget(FaultInfo, pSession);

    pSession = new CHeaterHistoryRecord(this);
    pStackedWidget->insertWidget(HistoryRecord, pSession);

    pStackedWidget->setCurrentIndex(MainForm);
    connect(ui->functionSwitchButtonGroup, SIGNAL(buttonReleased(int)), this, SLOT(functionSwitchButtonGroupStatusChanged(int)));

}

CHeaterClient::~CHeaterClient()
{
    delete ui;
}
void CHeaterClient::functionSwitchButtonGroupStatusChanged(int id)
{
    qDebug()<<id;
    pStackedWidget->setCurrentIndex(-(id + MainForm));
}
