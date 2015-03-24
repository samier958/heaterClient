#include "cheatermainform.h"
#include "ui_cheatermainform.h"

#include "cheaterclient.h"

CHeaterMainForm::CHeaterMainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterMainForm)
{
    ui->setupUi(this);
    connect(ui->connect, SIGNAL(pressed()), this, SIGNAL(establishNetworkConnection()));
    connect(ui->disconnect, SIGNAL(pressed()), this, SIGNAL(disconnectNetwork()));
}

CHeaterMainForm::~CHeaterMainForm()
{
    delete ui;
}
