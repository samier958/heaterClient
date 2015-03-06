#include "cheaterfaultinfo.h"
#include "ui_cheaterfaultinfo.h"

CHeaterFaultInfo::CHeaterFaultInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterFaultInfo)
{
    ui->setupUi(this);
}

CHeaterFaultInfo::~CHeaterFaultInfo()
{
    delete ui;
}
