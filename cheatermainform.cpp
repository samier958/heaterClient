#include "cheatermainform.h"
#include "ui_cheatermainform.h"

#include "cheaterclient.h"

CHeaterMainForm::CHeaterMainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterMainForm)
{
    ui->setupUi(this);
    connect(ui->connect, SIGNAL(pressed()), this, SIGNAL(establishNetworkConnection()));
    connect(ui->disconnect, SIGNAL(pressed()), this, SIGNAL(disconnectNetwork()));
}

CHeaterMainForm::~CHeaterMainForm()
{
    delete ui;
}

void CHeaterMainForm::updateHeaterSystemTime()
{
    /*
    QModbusRegisters systemTimeRegisters(HEATER_UNIT_SYSTEM_TIME_BASE, HEATER_UNIT_SYSTEM_TIME_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(systemTimeRegisters);
    pModbusMaster->close();
    for(int i = 0; i < HEATER_UNIT_SYSTEM_TIME_LENGTH; i ++){m_time[i] = systemTimeRegisters.getInteger16(i);}
    showHeaterSystemTime();
    */
}
void CHeaterMainForm::showHeaterSystemTime()
{
    /*
    QString timeString;
    for(int i = 0; i < HEATER_UNIT_SYSTEM_TIME_LENGTH; i ++){
        timeString += ((m_time[i] < 10)?"0":"") + QString::number(m_time[i]) + systemTimeString[i];
    }
    ui->systemTime->setText(timeString);
    */
}
void CHeaterMainForm::reflashHeaterControlForm(int index)
{
    if(CHeaterClient::MainForm == index){}
    else{}
}
