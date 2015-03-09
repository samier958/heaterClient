#include "cheatermainform.h"
#include "ui_cheatermainform.h"

#include "heatermodbusoffsetaddress.h"

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
    QModbusRegisters systemTimeRegisters(HEATER_UNIT_AVERAGE_TEMP_ADDR, HEATER_UNIT_SYSTEM_TIME_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(systemTimeRegisters);
    pModbusMaster->close();
    for(int i = 0; i < HEATER_UNIT_SYSTEM_TIME_LENGTH; i ++){m_time[i] = systemTimeRegisters.getInteger16(i);}
}
void CHeaterMainForm::showHeaterSystemTime()
{
    /*
    QString timeString;
    for(int i = 0; i < HEATER_UNIT_SYSTEM_TIME_LENGTH; i ++){
        timeString =
    }*/

}

void CHeaterMainForm::reflashHeaterControlForm(int index)
{

}
