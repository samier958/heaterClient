#include "cheaterclientserver.h"

CHeaterClientServer::CHeaterClientServer(QString ipAddr, QObject *parent)
{
    pModbusMaster = new QModbusMaster(ipAddr.toStdString().c_str(), MODBUS_TCP_PORT);
    pModbusMaster->setSlave(MODBUS_SLAVE_ADDR);

}
CHeaterClientServer::~CHeaterClientServer()
{
}

void CHeaterClientServer::setHeaterInfoPreview(CHeaterClient::SHeaterInfoPreview *heaterInfoPreview)
{
    pHeaterInfoPreview = heaterInfoPreview;
}

void CHeaterClientServer::run()
{
    QModbusError modbusErr;

    QModbusRegisters averageTemperatureRegister(HEATER_UNIT_AVERAGE_TEMP_ADDR, 1);
    QModbusRegisters faultStatusRegister(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);

    QModbusRegisters temperatureSensorsRegisters(HEATER_TEMPERATURE_SENSOR_BASE, HEATER_TEMPERATURE_SENSOR_LENGTH);

    QModbusRegisters parameterSettingsRegisters(HEATER_PARAMETER_SETTINGS_BASE, HEATER_PARAMETER_SETTINGS_LENGTH);

    QModbusRegisters faultInfoRegisters(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);
    int index = 0;
    do{
        //child form
        pModbusMaster->connect();
        switch (index) {
            case CHeaterClient::InfoPreview:
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
                    break;
            case CHeaterClient::MainForm:
                    pModbusMaster->readInputRegisters(temperatureSensorsRegisters);
                    for(int i = 0; i < 5; i ++){pHeaterRealTimeData->temperatureSensor[i] = temperatureSensorsRegisters.getInteger16(i);}
                    pHeaterRealTimeData->temperatureSensorBackup = temperatureSensorsRegisters.getInteger16(HEATER_TEMPERATURE_SENSOR_LENGTH - 1);
                    break;
            case CHeaterClient::RealTimeData:
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
                    break;
            case CHeaterClient::ParameterSetting:
                    pModbusMaster->readInputRegisters(faultInfoRegisters);
                    for(int i = 0; i < 5; i ++){
                        pHeaterFaultInfo->faultInfo[i] = faultInfoRegisters.getUInteger16(i);
                    }
                    break;
            case CHeaterClient::FaultInfo:
                    break;
            default:
                break;
        }
        pModbusMaster->close();
        emit updateHeaterClientForm(index);
        sleep(10);
    }while(1);
}
