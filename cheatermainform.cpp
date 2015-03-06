#include "cheatermainform.h"
#include "ui_cheatermainform.h"

CHeaterMainForm::CHeaterMainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterMainForm)
{
    ui->setupUi(this);
}

CHeaterMainForm::~CHeaterMainForm()
{
    delete ui;
}
