#ifndef CHEATERPARAMETERSETTINGS_H
#define CHEATERPARAMETERSETTINGS_H

#include <QWidget>

#include "qmodbusmaster.h"

using namespace Modbus;

namespace Ui {
class CHeaterParameterSettings;
}

class CHeaterParameterSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterParameterSettings(QWidget *parent = 0);
    ~CHeaterParameterSettings();

    void setModbusMaster(QModbusMaster *modbusMaster);

public slots:
    void reflashHeaterControlForm(int index);

private slots:
    void showHeaterParameterSettings();
    void updateHeaterParameterSettings();
    
    void on_heaterSwitch_currentIndexChanged(int index);

    void on_apply_released();

    void on_cancle_pressed();

private:
    Ui::CHeaterParameterSettings *ui;


    QModbusMaster *pModbusMaster;

    int m_heaterIndex;

    bool enbaleAutoHeating;
    int m_autoHeatingStartTemp;
    int m_autoHeatingStopTemp;

    bool enbaleAutoRadiating;
    int m_autoRadiatingStartTemp;
    int m_autoRadiatingStopTemp;

    int m_debugTerminateTimer;
    int m_PTCDelayStart;
    int m_FANDelayStop;

    int m_startInterval;
    int m_minimumDowntime;
    int m_PT100TempSensorOffset;
};

#endif // CHEATERPARAMETERSETTINGS_H
