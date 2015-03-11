#include "cheaterparametersettings.h"
#include "ui_cheaterparametersettings.h"

#include "heatermodbusoffsetaddress.h"
#include "cheaterclient.h"

#include <QDebug>

CHeaterParameterSettings::CHeaterParameterSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterParameterSettings)
{
    ui->setupUi(this);

    m_heaterIndex = 0;
}

CHeaterParameterSettings::~CHeaterParameterSettings()
{
    delete ui;
}

void CHeaterParameterSettings::setModbusMaster(QModbusMaster *modbusMaster)
{
    pModbusMaster = modbusMaster;
}


void CHeaterParameterSettings::showHeaterParameterSettings()
{
    QString tempString;
    ui->autoHeating->setChecked(enbaleAutoHeating);
    tempString = QString::number(((double)(m_autoHeatingStartTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoHeatingStartTemp->setText(tempString);
    tempString = QString::number(((double)(m_autoHeatingStopTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoHeatingStopTemp->setText(tempString);


    ui->autoRadiating->setChecked(enbaleAutoRadiating);
    tempString = QString::number(((double)(m_autoRadiatingStartTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoRadiatingStartTemp->setText(tempString);
    tempString = QString::number(((double)(m_autoRadiatingStopTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoRadiatingStopTemp->setText(tempString);


    tempString = QString::number(((double)(m_debugTerminateTimer)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->debugTerminateTimer->setText(tempString);
    tempString = QString::number(((double)(m_PTCDelayStart)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->PTCDelayStart->setText(tempString);
    tempString = QString::number(((double)(m_FANDelayStop)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->FANDelayStop->setText(tempString);
    tempString = QString::number(((double)(m_startInterval)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->startInterval->setText(tempString);
    tempString = QString::number(((double)(m_minimumDowntime)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->minimumDowntime->setText(tempString);
    tempString = QString::number(((double)(m_PT100TempSensorOffset)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->PT100TempSensorOffset->setText(tempString);

}
void CHeaterParameterSettings::updateHeaterParameterSettings()
{
    QModbusRegisters parameterSettingsRegisters(HEATER_PARAMETER_SETTINGS_BASE + MODBUS_OFFSET_ADDR * m_heaterIndex, HEATER_PARAMETER_SETTINGS_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(parameterSettingsRegisters);
    pModbusMaster->close();

    enbaleAutoHeating = parameterSettingsRegisters.getUInteger16(0) & (0x0001);
    m_autoHeatingStartTemp = parameterSettingsRegisters.getInteger16(1);
    m_autoHeatingStopTemp = parameterSettingsRegisters.getInteger16(2);

    enbaleAutoRadiating = parameterSettingsRegisters.getUInteger16(0) & (0x0002);
    m_autoRadiatingStartTemp = parameterSettingsRegisters.getInteger16(3);
    m_autoRadiatingStopTemp = parameterSettingsRegisters.getInteger16(4);

    m_debugTerminateTimer = parameterSettingsRegisters.getInteger16(10);
    m_PTCDelayStart = parameterSettingsRegisters.getInteger16(11);
    m_FANDelayStop = parameterSettingsRegisters.getInteger16(12);
    m_startInterval = parameterSettingsRegisters.getInteger16(13);
    m_minimumDowntime = parameterSettingsRegisters.getInteger16(14);

    m_PT100TempSensorOffset = parameterSettingsRegisters.getInteger16(19);

    showHeaterParameterSettings();

}
void CHeaterParameterSettings::reflashHeaterControlForm(int index)
{  
    if(CHeaterClient::ParameterSetting == index){updateHeaterParameterSettings();}
}

void CHeaterParameterSettings::on_heaterSwitch_currentIndexChanged(int index)
{
    m_heaterIndex = index;
    updateHeaterParameterSettings();
}

void CHeaterParameterSettings::on_apply_released()
{
    QModbusRegisters parameterSettingsRegisters(HEATER_PARAMETER_SETTINGS_BASE + MODBUS_OFFSET_ADDR * m_heaterIndex, HEATER_PARAMETER_SETTINGS_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(parameterSettingsRegisters);
    pModbusMaster->close();

    QString autoHeatingStartTempString, autoHeatingStopTempSting,autoRadiatingStartTempString,autoRadiatingStopTempString,debugTerminateTimerString,PTCDelayStartString,FANDelayStopString,startIntervalString,minimumDowntimeString,PT100TempSensorOffsetString;
    float tmp;
    int value;
    bool ok;

    uint controlFunctionSetting;
    controlFunctionSetting = parameterSettingsRegisters.getUInteger16(0);
    if(ui->autoHeating->isChecked()){controlFunctionSetting |= 0x0001;}
    else{controlFunctionSetting &= ~(0x0001);}
    if(ui->autoRadiating->isChecked()){controlFunctionSetting |= 0x0002;}
    else{controlFunctionSetting &= ~(0x0002);}
    parameterSettingsRegisters.setUInteger16(0, controlFunctionSetting);

    autoHeatingStartTempString = ui->autoHeatingStartTemp->text();
    tmp = autoHeatingStartTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(1, value);

    autoHeatingStopTempSting = ui->autoHeatingStopTemp->text();
    tmp = autoHeatingStopTempSting.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(2, value);

    autoRadiatingStartTempString = ui->autoRadiatingStartTemp->text();
    tmp = autoRadiatingStartTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(3, value);

    autoRadiatingStopTempString = ui->autoRadiatingStopTemp->text();
    tmp = autoRadiatingStopTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(4, value);

    debugTerminateTimerString = ui->debugTerminateTimer->text();
    tmp = debugTerminateTimerString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(10, value);

    PTCDelayStartString = ui->PTCDelayStart->text();
    tmp = PTCDelayStartString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(11, value);

    FANDelayStopString = ui->FANDelayStop->text();
    tmp = FANDelayStopString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(12, value);

    startIntervalString = ui->startInterval->text();
    tmp = startIntervalString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(13, value);

    minimumDowntimeString = ui->minimumDowntime->text();
    tmp = minimumDowntimeString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(14, value);

    PT100TempSensorOffsetString = ui->PT100TempSensorOffset->text();
    tmp = PT100TempSensorOffsetString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingsRegisters.setInteger16(19, value);

    pModbusMaster->connect();
    pModbusMaster->writeRegisters(parameterSettingsRegisters);
    pModbusMaster->close();


}

void CHeaterParameterSettings::on_cancle_pressed()
{
    showHeaterParameterSettings();
}
