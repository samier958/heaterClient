#include "cheaterparametersettings.h"
#include "ui_cheaterparametersettings.h"

#include "heaterConfig.h"
#include "cheaterclient.h"
#include "cheaterclientserver.h"

#include <QDebug>

CHeaterParameterSettings::CHeaterParameterSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterParameterSettings)
{
    ui->setupUi(this);
    m_groupSwith = 0;
    heaterParameterSetting[0].heaterIndex = 0;
    heaterParameterSetting[1].heaterIndex = 0;
}

CHeaterParameterSettings::~CHeaterParameterSettings()
{
    delete ui;
}

void CHeaterParameterSettings::showHeaterParameterSettings()
{
    QString tempString;
    ui->autoHeating->setChecked(heaterParameterSetting[m_groupSwith].enbaleAutoHeating);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].autoHeatingStartTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoHeatingStartTemp->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].autoHeatingStopTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoHeatingStopTemp->setText(tempString);

    ui->autoRadiating->setChecked(heaterParameterSetting[m_groupSwith].enbaleAutoRadiating);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].autoRadiatingStartTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoRadiatingStartTemp->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].autoRadiatingStopTemp)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->autoRadiatingStopTemp->setText(tempString);

    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].debugTerminateTimer)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->debugTerminateTimer->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].PTCDelayStart)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->PTCDelayStart->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].FANDelayStop)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->FANDelayStop->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].startInterval)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->startInterval->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].minimumDowntime)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->minimumDowntime->setText(tempString);
    tempString = QString::number(((double)(heaterParameterSetting[m_groupSwith].PT100TempSensorOffset)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->PT100TempSensorOffset->setText(tempString);
}
void CHeaterParameterSettings::on_heaterSwitch_currentIndexChanged(int index)
{
    heaterParameterSetting[m_groupSwith].heaterIndex = index;
    emit sendClientServerCommand(CHeaterClientServer::ParameterSettingReadCmd);
}

void CHeaterParameterSettings::on_apply_released()
{
    QString autoHeatingStartTempString, autoHeatingStopTempSting,autoRadiatingStartTempString,autoRadiatingStopTempString,debugTerminateTimerString,PTCDelayStartString,FANDelayStopString,startIntervalString,minimumDowntimeString,PT100TempSensorOffsetString;
    float tmp;
    int value;
    bool ok;
    //auto heating
    parameterSettingSyncToRemoteDevices[m_groupSwith].heaterIndex = ui->heaterSwitch->currentIndex();
    parameterSettingSyncToRemoteDevices[m_groupSwith].enbaleAutoHeating = ui->autoHeating->isChecked();

    autoHeatingStartTempString = ui->autoHeatingStartTemp->text();
    tmp = autoHeatingStartTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].autoHeatingStartTemp = value;

    autoHeatingStopTempSting = ui->autoHeatingStopTemp->text();
    tmp = autoHeatingStopTempSting.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].autoHeatingStopTemp = value;

    //auto radiating
    parameterSettingSyncToRemoteDevices[m_groupSwith].enbaleAutoRadiating = ui->autoRadiating->isChecked();

    autoRadiatingStartTempString = ui->autoRadiatingStartTemp->text();
    tmp = autoRadiatingStartTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].autoRadiatingStartTemp = value;

    autoRadiatingStopTempString = ui->autoRadiatingStopTemp->text();
    tmp = autoRadiatingStopTempString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].autoRadiatingStopTemp = value;

    //left settings
    debugTerminateTimerString = ui->debugTerminateTimer->text();
    tmp = debugTerminateTimerString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].debugTerminateTimer = value;

    PTCDelayStartString = ui->PTCDelayStart->text();
    tmp = PTCDelayStartString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].PTCDelayStart = value;

    FANDelayStopString = ui->FANDelayStop->text();
    tmp = FANDelayStopString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].FANDelayStop = value;

    //right settings
    startIntervalString = ui->startInterval->text();
    tmp = startIntervalString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].startInterval = value;

    minimumDowntimeString = ui->minimumDowntime->text();
    tmp = minimumDowntimeString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].minimumDowntime = value;

    PT100TempSensorOffsetString = ui->PT100TempSensorOffset->text();
    tmp = PT100TempSensorOffsetString.toFloat(&ok) * 10.0;
    value = static_cast<int>(tmp);
    parameterSettingSyncToRemoteDevices[m_groupSwith].PT100TempSensorOffset = value;

    emit sendClientServerCommand(CHeaterClientServer::ParameterSettingWriteCmd);
}

void CHeaterParameterSettings::on_cancle_pressed()
{
    //emit sendClientServerCommand(CHeaterClientServer::ParameterSettingReadCmd);
    showHeaterParameterSettings();
}
