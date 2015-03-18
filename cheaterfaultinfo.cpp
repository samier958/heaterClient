#include "cheaterfaultinfo.h"
#include "ui_cheaterfaultinfo.h"


#include "heaterConfig.h"
#include "cheaterclient.h"

CHeaterFaultInfo::CHeaterFaultInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterFaultInfo)
{
    ui->setupUi(this);

    pFaultInfoLabel[0] = ui->faultInfo_1;
    pFaultInfoLabel[1] = ui->faultInfo_2;
    pFaultInfoLabel[2] = ui->faultInfo_3;
    pFaultInfoLabel[3] = ui->faultInfo_4;
    pFaultInfoLabel[4] = ui->faultInfo_5;
}

CHeaterFaultInfo::~CHeaterFaultInfo()
{
    delete ui;
}

void CHeaterFaultInfo::showHeaterFaultInfo()
{
    /*
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 7; j ++){
            if(!m_faultInfo[i]){pFaultInfoLabel[i]->setText(faultInfoString[6]);break;}
            if(m_faultInfo[i] & (1 << j)){pFaultInfoLabel[i]->setText(faultInfoString[j]);}
        }
    }
    */
}

void CHeaterFaultInfo::updateHeaterFaultInfo()
{
    /*
    QModbusRegisters faultInfoRegisters(HEATER_FAULT_STATUS_BASE, HEATER_FAULT_STATUS_LENGTH);
    pModbusMaster->connect();
    pModbusMaster->readInputRegisters(faultInfoRegisters);
    pModbusMaster->close();
    for(int i = 0; i < 5; i ++){
        m_faultInfo[i] = faultInfoRegisters.getUInteger16(i);
    }


    showHeaterFaultInfo();
    */
}

void CHeaterFaultInfo::reflashHeaterControlForm(int index)
{
    if(CHeaterClient::FaultInfo == index){updateHeaterFaultInfo();}
}
