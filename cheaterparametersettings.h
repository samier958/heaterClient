#ifndef CHEATERPARAMETERSETTINGS_H
#define CHEATERPARAMETERSETTINGS_H

#include <QWidget>

#include "qmodbusmaster.h"
#include "heaterConfig.h"

using namespace Modbus;

namespace Ui {
class CHeaterParameterSettings;
}

class CHeaterParameterSettings : public QWidget
{
    Q_OBJECT
    
public:
    typedef struct _heater_parameter_settings{
        int heaterIndex;

        bool enbaleAutoHeating;
        int autoHeatingStartTemp;
        int autoHeatingStopTemp;

        bool enbaleAutoRadiating;
        int autoRadiatingStartTemp;
        int autoRadiatingStopTemp;

        int debugTerminateTimer;
        int PTCDelayStart;
        int FANDelayStop;

        int startInterval;
        int minimumDowntime;
        int PT100TempSensorOffset;
    }SHeaterParameterSettings;

    explicit CHeaterParameterSettings(QWidget *parent = 0);
    ~CHeaterParameterSettings();

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

    SHeaterParameterSettings heaterParameterSetting[FAN_TOWER_GROUP];

};

#endif // CHEATERPARAMETERSETTINGS_H
