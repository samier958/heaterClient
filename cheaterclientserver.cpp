#include "cheaterclientserver.h"

#include <QDebug>

CHeaterClientServer::CHeaterClientServer(QString ipAddr, QObject *parent)
{
    pModbusMaster = new QModbusMaster(ipAddr.toStdString().c_str(), MODBUS_TCP_PORT);
    pModbusMaster->setSlave(MODBUS_SLAVE_ADDR);

    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
    clientServerCommandHistory.lastCommand = MainFormReadCmd;

}
CHeaterClientServer::~CHeaterClientServer()
{
}

void CHeaterClientServer::setHeaterInfoPreview(CHeaterClient::SHeaterInfoPreview *heaterInfoPreview)
{
    pHeaterInfoPreview = heaterInfoPreview;
}
void CHeaterClientServer::setHeaterRealTimeData(CHeaterRealTimeData::SHeaterRealTimeData *heaterRealTimeData)
{
    pHeaterRealTimeData = heaterRealTimeData;
}
void CHeaterClientServer::setHeaterParameterSettings(CHeaterParameterSettings::SHeaterParameterSettings *heaterParameterSettings)
{
    pHeaterParameterSettings = heaterParameterSettings;
}

void CHeaterClientServer::setParameterSettingSyncToRemoteDevices(CHeaterParameterSettings::SHeaterParameterSettings *parameterSettingSyncToRemoteDevices)
{
    pParameterSettingSyncToRemoteDevices = parameterSettingSyncToRemoteDevices;
}
void CHeaterClientServer::setHeaterFaultInfo(CHeaterFaultInfo::SHeaterFaultInfo *heaterFaultInfo)
{
    pHeaterFaultInfo = heaterFaultInfo;
}
void CHeaterClientServer::setHeaterHistoryRecordFixedTime(CHeaterHistoryRecord::SHeaterHistoryRecordFixedTime *heaterHistoryRecordFixedTime)
{
    pHeaterHistoryRecordFixedTime = heaterHistoryRecordFixedTime;
}
void CHeaterClientServer::setHeaterHistoryRecordWorkOrFault(CHeaterHistoryRecord::SHeaterHistoryRecordWorkOrFault *heaterHistoryRecordWorkOrFault)
{
    pHeaterHistoryRecordWorkOrFault = heaterHistoryRecordWorkOrFault;
}

void CHeaterClientServer::run()
{
    QModbusError modbusErr;

    QModbusRegisters averageTemperatureRegister(HEATER_UNIT_AVERAGE_TEMP_ADDR, 1);
    QModbusRegisters faultStatusRegister(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);

    QModbusRegisters temperatureSensorsRegisters(HEATER_TEMPERATURE_SENSOR_BASE, HEATER_TEMPERATURE_SENSOR_LENGTH);

    QModbusRegisters parameterSettingsRegisters(HEATER_PARAMETER_SETTINGS_BASE, HEATER_PARAMETER_SETTINGS_LENGTH);

    QModbusRegisters faultInfoRegisters(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);

    QModbusRegisters recordTimeRegisters(HEATER_RECORD_TIME_BASE, HEATER_RECORD_TIME_LENGTH);
    QModbusRegisters recordTimeValidTagRegisters(HEATER_RECORD_TIME_VALID_TAG_ADDR, 1);
    QModbusRegisters recordDataRegisters(HEATER_HISTORY_RECORD_BASE, HEATER_HISTORY_RECORD_ITEM_LENGTH);

    QModbusRegisters workRecordRegisters(HEATER_WORK_RECORD_BASE, HEATER_WORK_RECORD_LENGTH);
    QModbusRegisters faultRecordRegisters(HEATER_FAULT_RECORD_BASE, HEATER_FAULT_RECORD_LENGTH);

    int serverCommand = 0, cmd = 0;

    uint controlFunctionSetting = 0;

    do{
        //child form
        cmd = clientServerCommandHistory.currentCommand;
        qDebug()<<"Client Server Cmd:"<<cmd;
        pModbusMaster->connect();
        switch (cmd) {
            case InfoPreviewReadCmd:
                    //info preview
                    modbusErr = pModbusMaster->lastError();
                    if(modbusErr.isValid()) {pHeaterInfoPreview->networkStatus = NETWORK_OFFLINE;continue;}
                    else {pHeaterInfoPreview->networkStatus = NETWORK_ONLINE;}
                    pModbusMaster->readInputRegisters(averageTemperatureRegister);
                    pModbusMaster->readInputRegisters(faultStatusRegister);
                    pHeaterInfoPreview->averageTemperature = averageTemperatureRegister.getInteger16(0);
                    for(int i = 0; i < HEATER_FAULT_STATUS_LENGTH; i ++){
                        if(faultStatusRegister.getUInteger16(i)){pHeaterInfoPreview->faultStatus = true;continue;}
                        else {pHeaterInfoPreview->faultStatus = false;}
                    }

                    //switch command execute
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;
                    break;
            case MainFormReadCmd:
                    //switch command execute
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;
                    break;
            case RealTimeDataReadCmd:
                    pModbusMaster->readInputRegisters(temperatureSensorsRegisters);
                    for(int i = 0; i < 5; i ++){pHeaterRealTimeData->temperatureSensor[i] = temperatureSensorsRegisters.getInteger16(i);}
                    pHeaterRealTimeData->temperatureSensorBackup = temperatureSensorsRegisters.getInteger16(HEATER_TEMPERATURE_SENSOR_LENGTH - 1);
                    //switch command execute
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;

                    break;
            case ParameterSettingReadCmd:
                    parameterSettingsRegisters.setAddress(HEATER_PARAMETER_SETTINGS_BASE + pHeaterParameterSettings->heaterIndex * HEATER_MODBUS_OFFSET_ADDR);
                    pModbusMaster->readInputRegisters(parameterSettingsRegisters);

                    pHeaterParameterSettings->enbaleAutoHeating = parameterSettingsRegisters.getUInteger16(0) & (0x0001);
                    pHeaterParameterSettings->autoHeatingStartTemp = parameterSettingsRegisters.getInteger16(1);
                    pHeaterParameterSettings->autoHeatingStopTemp = parameterSettingsRegisters.getInteger16(2);

                    pHeaterParameterSettings->enbaleAutoRadiating = parameterSettingsRegisters.getUInteger16(0) & (0x0002);
                    pHeaterParameterSettings->autoRadiatingStartTemp = parameterSettingsRegisters.getInteger16(3);
                    pHeaterParameterSettings->autoRadiatingStopTemp = parameterSettingsRegisters.getInteger16(4);

                    pHeaterParameterSettings->debugTerminateTimer = parameterSettingsRegisters.getInteger16(10);
                    pHeaterParameterSettings->PTCDelayStart = parameterSettingsRegisters.getInteger16(11);
                    pHeaterParameterSettings->FANDelayStop = parameterSettingsRegisters.getInteger16(12);
                    pHeaterParameterSettings->startInterval = parameterSettingsRegisters.getInteger16(13);
                    pHeaterParameterSettings->minimumDowntime = parameterSettingsRegisters.getInteger16(14);

                    pHeaterParameterSettings->PT100TempSensorOffset = parameterSettingsRegisters.getInteger16(19);
                    //switch command execute
                    /*
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;
                    */
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;

                    break;
            case ParameterSettingWriteCmd:
                    parameterSettingsRegisters.setAddress(HEATER_PARAMETER_SETTINGS_BASE + pParameterSettingSyncToRemoteDevices->heaterIndex * HEATER_MODBUS_OFFSET_ADDR);

                    controlFunctionSetting = parameterSettingsRegisters.getUInteger16(0);
                    if(pParameterSettingSyncToRemoteDevices->enbaleAutoHeating){controlFunctionSetting |= 0x0001;}
                    else{controlFunctionSetting &= ~(0x0001);}
                    if(pParameterSettingSyncToRemoteDevices->enbaleAutoRadiating){controlFunctionSetting |= 0x0002;}
                    else{controlFunctionSetting &= ~(0x0002);}
                    parameterSettingsRegisters.setUInteger16(0, controlFunctionSetting);

                    parameterSettingsRegisters.setInteger16(1, pParameterSettingSyncToRemoteDevices->autoHeatingStartTemp);
                    parameterSettingsRegisters.setInteger16(2, pParameterSettingSyncToRemoteDevices->autoHeatingStopTemp);

                    parameterSettingsRegisters.setInteger16(3, pParameterSettingSyncToRemoteDevices->autoRadiatingStartTemp);
                    parameterSettingsRegisters.setInteger16(4, pParameterSettingSyncToRemoteDevices->autoRadiatingStopTemp);

                    parameterSettingsRegisters.setInteger16(10, pParameterSettingSyncToRemoteDevices->debugTerminateTimer);
                    parameterSettingsRegisters.setInteger16(11, pParameterSettingSyncToRemoteDevices->PTCDelayStart);
                    parameterSettingsRegisters.setInteger16(12, pParameterSettingSyncToRemoteDevices->FANDelayStop);
                    parameterSettingsRegisters.setInteger16(13, pParameterSettingSyncToRemoteDevices->startInterval);
                    parameterSettingsRegisters.setInteger16(14, pParameterSettingSyncToRemoteDevices->minimumDowntime);

                    parameterSettingsRegisters.setInteger16(19, pParameterSettingSyncToRemoteDevices->PT100TempSensorOffset);

                    pModbusMaster->writeRegisters(parameterSettingsRegisters);

                    //switch command execute
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;

                    break;
            case FaultInfoReadCmd:
                    pModbusMaster->readInputRegisters(faultInfoRegisters);
                    for(int i = 0; i < 5; i ++){
                        pHeaterFaultInfo->faultInfo[i] = faultInfoRegisters.getUInteger16(i);
                    }
                    //switch command execute
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;

                    break;
            case HistoryRecordFixedTimeCmd:
                    #define RECORD_VALID 1
                    #define RECORD_NO_VALID 0
                    if(pHeaterHistoryRecordFixedTime->wait){
                        for(int i = 0; i < HEATER_RECORD_TIME_BASE; i ++) { recordTimeRegisters.setInteger16(i, pHeaterHistoryRecordFixedTime->time[i]);}
                        pModbusMaster->writeRegisters(recordTimeRegisters);
                        pHeaterHistoryRecordFixedTime->wait = 1;
                    }
                    pModbusMaster->readInputRegisters(recordTimeValidTagRegisters);
                    pHeaterHistoryRecordFixedTime->valid = recordTimeValidTagRegisters.getInteger16(0);
                    //switch command execute
                    if(RECORD_NO_VALID == pHeaterHistoryRecordFixedTime->valid){
                        serverCommand = clientServerCommandHistory.lastCommand;
                        clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                        clientServerCommandHistory.currentCommand  = serverCommand;
                    }
                    else {
                        recordDataRegisters.setAddress((HEATER_HISTORY_RECORD_BASE + HEATER_HISTORY_RECORD_ITEM_LENGTH * (pHeaterHistoryRecordFixedTime->index - 1)));
                        pModbusMaster->readInputRegisters(recordDataRegisters);
                        pHeaterHistoryRecordFixedTime->temperature   = recordDataRegisters.getInteger16(0);
                        pHeaterHistoryRecordFixedTime->controlMode   = recordDataRegisters.getInteger16(1);
                        pHeaterHistoryRecordFixedTime->runningStatus = recordDataRegisters.getInteger16(2);
                        pHeaterHistoryRecordFixedTime->inputPort     = recordDataRegisters.getInteger16(3);
                        pHeaterHistoryRecordFixedTime->outputPort    = recordDataRegisters.getInteger16(4);
                        pHeaterHistoryRecordFixedTime->faultInfo     = recordDataRegisters.getInteger16(5);

                        clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                        clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
                    }

                    break;
            case HistoryRecordWorkOrFaultCmd:
                    pModbusMaster->readInputRegisters(workRecordRegisters);
                    pModbusMaster->readInputRegisters(faultRecordRegisters);
                    for(int i = 0; i < HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_NUM; i ++){
                        //work record
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].year  = workRecordRegisters.getInteger16(0 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].month = workRecordRegisters.getInteger16(1 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].day   = workRecordRegisters.getInteger16(2 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].hour  = workRecordRegisters.getInteger16(3 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].min   = workRecordRegisters.getInteger16(4 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->workRecord[i].code  = workRecordRegisters.getInteger16(5 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        //fault record
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].year  = workRecordRegisters.getInteger16(0 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].month = workRecordRegisters.getInteger16(1 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].day   = workRecordRegisters.getInteger16(2 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].hour  = workRecordRegisters.getInteger16(3 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].min   = workRecordRegisters.getInteger16(4 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                        pHeaterHistoryRecordWorkOrFault->faultRecord[i].code  = workRecordRegisters.getInteger16(5 + (i * HEATER_WORK_OR_FAULT_HISTORY_RECORD_TIEM_LENGTH));
                    }
                    //switch command execute
                    /*serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;*/

                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;

                    break;
            default:
                    break;
        }
        pModbusMaster->close();
        emit clientServerCommandComplete(cmd);
        sleep(1);
    }while(1);
}


void CHeaterClientServer::clientServerCommandExecute(int cmd)
{
    clientServerCommandHistory.currentCommand = cmd;
    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
}