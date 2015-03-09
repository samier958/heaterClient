#ifndef CHEATERPARAMETERSETTINGS_H
#define CHEATERPARAMETERSETTINGS_H

#include <QWidget>

namespace Ui {
class CHeaterParameterSettings;
}

class CHeaterParameterSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterParameterSettings(QWidget *parent = 0);
    ~CHeaterParameterSettings();

public slots:
    void reflashHeaterControlForm(int index);

private slots:
    void showHeaterParameterSettings();
    void updateHeaterParameterSettings();
    
private:
    Ui::CHeaterParameterSettings *ui;


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
