#include "cheaterrealtimedata.h"
#include "ui_cheaterrealtimedata.h"

#include "cheaterclient.h"
#include "cheaterclientserver.h"

#include <QDebug>

CHeaterRealTimeData::CHeaterRealTimeData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterRealTimeData)
{
    ui->setupUi(this);
    m_groupSwith = 0;
}

CHeaterRealTimeData::~CHeaterRealTimeData()
{
    delete ui;
}



void CHeaterRealTimeData::showHeaterRealTimeData()
{
    QString tempString;
    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensor[0])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_1->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensor[1])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_2->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensor[2])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_3->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensor[3])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_4->setText(tempString);
    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensor[4])) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->heaterTemp_5->setText(tempString);

    tempString = QString::number(((double)(heaterRealTimeData[m_groupSwith].temperatureSensorBackup)) / 10.0);if(!tempString.contains('.')){tempString += ".0";}
    ui->tempSensorBackup->setText(tempString);
}

void CHeaterRealTimeData::on_heaterGroupSwith_currentIndexChanged(int index)
{
    qDebug()<<"index"<<index;
    m_groupSwith = index;
    showHeaterRealTimeData();
}
