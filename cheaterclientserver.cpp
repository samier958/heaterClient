#include "cheaterclientserver.h"

#include <QDate>
#include <QTime>
#include <QDebug>

CHeaterClientServer::CHeaterClientServer(QString ipAddr, QObject *parent)
{
    qDebug()<<ipAddr;
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
void CHeaterClientServer::setHeaterRealTimeDataTemp(CHeaterRealTimeData::SHeaterRealTimeDataTemp *heaterRealTimeDataTemp)
{
    pHeaterRealTimeDataTemp = heaterRealTimeDataTemp;
}
void CHeaterClientServer::setHeaterRealTimeDataRemoteControl(CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *heaterRealTimeDataRemoterControl)
{
    pHeaterRealTimeDataRemoterControl = heaterRealTimeDataRemoterControl;
}
void CHeaterClientServer::setRealTimeDataRemoterControlSyncToRemoteDevices(CHeaterRealTimeData::SHeaterRealTimeDataRemoteControl *realTimeDataRemoterControlSyncToRemoteDevices)
{
    pRealTimeDataRemoterControlSyncToRemoteDevices = realTimeDataRemoterControlSyncToRemoteDevices;
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

    QModbusRegisters systemTimeRegisters(HEATER_UNIT_SYSTEM_TIME_BASE, HEATER_UNIT_SYSTEM_TIME_LENGTH);

    //QModbusRegisters averageTemperatureRegister(HEATER_UNIT_AVERAGE_TEMP_ADDR, 1);
    //QModbusRegisters faultStatusRegister(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);
    QModbusRegisters infoPreviewRegisters(HEATER_UNIT_AVERAGE_TEMP_ADDR, HEATER_FAULT_STATUS_LENGTH + (HEATER_FAULT_STATUS_BASE - HEATER_UNIT_AVERAGE_TEMP_ADDR));

    QModbusRegisters temperatureSensorsRegisters(HEATER_TEMPERATURE_SENSOR_BASE, HEATER_TEMPERATURE_SENSOR_LENGTH);
    QModbusRegisters remoteControlRegisters(HEATER_WORK_SWITCH_ADDR, 1);

    QModbusRegisters parameterSettingsRegisters(HEATER_PARAMETER_SETTINGS_BASE, HEATER_PARAMETER_SETTINGS_LENGTH);

    QModbusRegisters faultInfoRegisters(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);

    QModbusRegisters recordTimeRegisters(HEATER_RECORD_TIME_BASE, HEATER_RECORD_TIME_LENGTH + 1);
    QModbusRegisters recordTimeValidTagRegisters(HEATER_RECORD_TIME_VALID_TAG_ADDR, 1);
    QModbusRegisters recordDataRegisters(HEATER_HISTORY_RECORD_BASE, HEATER_HISTORY_RECORD_ITEM_LENGTH);

    QModbusRegisters workRecordRegisters(HEATER_WORK_RECORD_BASE, HEATER_WORK_RECORD_LENGTH);
    QModbusRegisters faultRecordRegisters(HEATER_FAULT_RECORD_BASE, HEATER_FAULT_RECORD_LENGTH);

    QModbusBits controlCommandBit(HEATER_CONTROL_REGISTER_BASE * 8, 1);

    QDate date;
    QTime time;

    int serverCommand = 0, cmd = 0;

    uint controlFunctionSetting = 0;

    do{
        qDebug()<<"Heater Connection Allowed:"<<pHeaterInfoPreview->allowed;
        if(!(pHeaterInfoPreview->allowed)){
            pHeaterInfoPreview->networkStatus = NETWORK_OFFLINE;
            pHeaterInfoPreview->averageTemperature = 0;
            pHeaterInfoPreview->faultStatus = true;
            emit clientServerCommandComplete(InfoPreviewReadCmd);
            sleep(1);
            continue;
        }
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
                    pModbusMaster->readInputRegisters(infoPreviewRegisters);
                    pHeaterInfoPreview->averageTemperature = infoPreviewRegisters.getInteger16(0);
                    for(int i = (HEATER_FAULT_STATUS_BASE - HEATER_UNIT_AVERAGE_TEMP_ADDR); i < HEATER_FAULT_STATUS_LENGTH + (HEATER_FAULT_STATUS_BASE - HEATER_UNIT_AVERAGE_TEMP_ADDR); i ++){
                        if(infoPreviewRegisters.getUInteger16(i)){pHeaterInfoPreview->faultStatus = true;break;}
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
            case RealTimeDataTempReadCmd:
                    pModbusMaster->readInputRegisters(temperatureSensorsRegisters);
                    for(int i = 0; i < 5; i ++){pHeaterRealTimeDataTemp->temperatureSensor[i] = temperatureSensorsRegisters.getInteger16(i);}
                    pHeaterRealTimeDataTemp->temperatureSensorBackup = temperatureSensorsRegisters.getInteger16(HEATER_TEMPERATURE_SENSOR_LENGTH - 1);
                    //switch command execute
                    serverCommand = clientServerCommandHistory.lastCommand;
                    clientServerCommandHistory.lastCommand = clientServerCommandHistory.currentCommand ;
                    clientServerCommandHistory.currentCommand  = serverCommand;
                    break;
            case RealTimeDataRemoteControlReadCmd:
                    for(int i = 0; i < HEATER_QUANITY_NUM; i ++){
                        remoteControlRegisters.setAddress(HEATER_WORK_SWITCH_ADDR + HEATER_MODBUS_OFFSET_ADDR * i);
                        pModbusMaster->readInputRegisters(remoteControlRegisters);
                        pHeaterRealTimeDataRemoterControl->remoteControl[i] = remoteControlRegisters.getInteger16(0);
                    }
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
                    break;
            case RealTimeDataRemoteControlWriteCmd:
                    for(int i = 0; i < HEATER_QUANITY_NUM; i ++){
                        remoteControlRegisters.setAddress(HEATER_WORK_SWITCH_ADDR + HEATER_MODBUS_OFFSET_ADDR * i);
                        remoteControlRegisters.setInteger16(0, pRealTimeDataRemoterControlSyncToRemoteDevices->remoteControl[i]);
                        pModbusMaster->writeRegisters(remoteControlRegisters);
                    }
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
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
            case ParameterSettingResetCmd:
                    controlCommandBit.setAddress((HEATER_CONTROL_REGISTER_BASE * 16) + HEATER_PARAMETER_SETTINGS_RESET_OFFSET);
                    controlCommandBit.setValue(0, 1);
                    pModbusMaster->writeBit(controlCommandBit);
                    clientServerCommandHistory.currentCommand = ParameterSettingReadCmd;
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
                    #define RECORD_VALID 0
                    #define RECORD_NO_VALID 1
                    if(!(pHeaterHistoryRecordFixedTime->wait)){
                        for(int i = 0; i < HEATER_RECORD_TIME_LENGTH; i ++) {recordTimeRegisters.setInteger16(i, pHeaterHistoryRecordFixedTime->time[i]);}
                        recordTimeRegisters.setInteger16(HEATER_RECORD_TIME_LENGTH, 0x00fe);
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
                        recordDataRegisters.setAddress((HEATER_HISTORY_RECORD_BASE + HEATER_HISTORY_RECORD_ITEM_LENGTH * (pHeaterHistoryRecordFixedTime->index)));
                        pModbusMaster->readInputRegisters(recordDataRegisters);
                        pHeaterHistoryRecordFixedTime->temperature   = recordDataRegisters.getInteger16(0);
                        pHeaterHistoryRecordFixedTime->controlMode   = recordDataRegisters.getInteger16(1);
                        pHeaterHistoryRecordFixedTime->runningStatus = recordDataRegisters.getInteger16(2);
                        pHeaterHistoryRecordFixedTime->inputPort     = recordDataRegisters.getInteger16(3);
                        pHeaterHistoryRecordFixedTime->outputPort    = recordDataRegisters.getInteger16(4);
                        pHeaterHistoryRecordFixedTime->faultInfo     = recordDataRegisters.getInteger16(5);

                        pHeaterHistoryRecordFixedTime->wait = 0;
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
            case HistoryRecordFaultClearCmd:
                    controlCommandBit.setAddress((HEATER_CONTROL_REGISTER_BASE * 16) + HEATER_FAULT_RECORD_CLEAR_OFFSET);
                    controlCommandBit.setValue(0, 1);
                    pModbusMaster->writeBit(controlCommandBit);
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
                    break;
            case HistoryRecordWorkClearCmd:
                    controlCommandBit.setAddress((HEATER_CONTROL_REGISTER_BASE * 16) + HEATER_WORK_RECORD_CLEAR_OFFSET);
                    controlCommandBit.setValue(0, 1);
                    pModbusMaster->writeBit(controlCommandBit);
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = InfoPreviewReadCmd;
                    break;
            case SystimeTimeCalibrationCmd:
                    date = QDate::currentDate();
                    time = QTime::currentTime();
                    systemTimeRegisters.setUInteger16(0, date.year());
                    systemTimeRegisters.setUInteger16(1, date.month());
                    systemTimeRegisters.setUInteger16(2, date.day());
                    systemTimeRegisters.setUInteger16(3, time.hour());
                    systemTimeRegisters.setUInteger16(4, time.minute());
                    pModbusMaster->writeRegisters(systemTimeRegisters);
                    clientServerCommandHistory.currentCommand = InfoPreviewReadCmd;
                    clientServerCommandHistory.lastCommand = MainFormReadCmd;
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
