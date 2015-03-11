#include "cheaterhistoryrecord.h"
#include "ui_cheaterhistoryrecord.h"

#include "heatermodbusoffsetaddress.h"

#include "cheaterclient.h"

CHeaterHistoryRecord::CHeaterHistoryRecord(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterHistoryRecord)
{
    ui->setupUi(this);
}

CHeaterHistoryRecord::~CHeaterHistoryRecord()
{
    delete ui;
}
void CHeaterHistoryRecord::setModbusMaster(QModbusMaster *modbusMaster)
{
    pModbusMaster = modbusMaster;
}
void CHeaterHistoryRecord::reflashHeaterControlForm(int index)
{

}
