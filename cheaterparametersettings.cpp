#include "cheaterparametersettings.h"
#include "ui_cheaterparametersettings.h"

CHeaterParameterSettings::CHeaterParameterSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterParameterSettings)
{
    ui->setupUi(this);
}

CHeaterParameterSettings::~CHeaterParameterSettings()
{
    delete ui;
}


void CHeaterParameterSettings::showHeaterParameterSettings()
{

}
void CHeaterParameterSettings::updateHeaterParameterSettings()
{

}
void CHeaterParameterSettings::reflashHeaterControlForm(int index)
{

}
