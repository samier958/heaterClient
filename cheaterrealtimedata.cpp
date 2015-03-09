#include "cheaterrealtimedata.h"
#include "ui_cheaterrealtimedata.h"


#include "heatermodbusoffsetaddress.h"

CHeaterRealTimeData::CHeaterRealTimeData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterRealTimeData)
{
    ui->setupUi(this);
}

CHeaterRealTimeData::~CHeaterRealTimeData()
{
    delete ui;
}


void CHeaterRealTimeData::showHeaterRealTimeData()
{
    QString tempString;
    tempString = QString::number(((double)(m_temperatureSensor[0])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_1->setText(tempString);
    tempString = QString::number(((double)(m_temperatureSensor[1])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_2->setText(tempString);
    tempString = QString::number(((double)(m_temperatureSensor[2])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_3->setText(tempString);
    tempString = QString::number(((double)(m_temperatureSensor[3])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_4->setText(tempString);
    tempString = QString::number(((double)(m_temperatureSensor[4])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_5->setText(tempString);

    tempString = QString::number(((double)(m_temperatureSensorBackup)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->tempSensorBackup->setText(tempString);
}
void CHeaterRealTimeData::updateHeaterRealTimeData()
{
    QModbusRegisters temperatureSensorsRegisters(HEATER_TEMPERATURE_SENSOR_BASE, HEATER_TEMPERATURE_SENSOR_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(temperatureSensorsRegisters);
    pModbusMaster->close();
    for(int i = 0; i < 5; i ++){m_temperatureSensor[i] = temperatureSensorsRegisters.getInteger16(i);}
    m_temperatureSensorBackup = temperatureSensorsRegisters.getInteger16(HEATER_TEMPERATURE_SENSOR_LENGTH - 1);
}
void CHeaterRealTimeData::reflashHeaterControlForm(int index)
{

}
