#include "cheaterrealtimedata.h"
#include "ui_cheaterrealtimedata.h"

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
