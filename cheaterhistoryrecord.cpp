#include "cheaterhistoryrecord.h"
#include "ui_cheaterhistoryrecord.h"

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
