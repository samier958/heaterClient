#include "cheaterfaultinfo.h"
#include "ui_cheaterfaultinfo.h"


#include "heatermodbusoffsetaddress.h"
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


void CHeaterFaultInfo::setModbusMaster(QModbusMaster *modbusMaster)
{
    pModbusMaster = modbusMaster;
}
void CHeaterFaultInfo::showHeaterFaultInfo()
{
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 7; j ++){
            if(!m_faultInfo[i]){pFaultInfoLabel[i]->setText(faultInfoString[6]);break;}
            if(m_faultInfo[i] & (1 << j)){pFaultInfoLabel[i]->setText(faultInfoString[j]);}
        }
    }
}

void CHeaterFaultInfo::updateHeaterFaultInfo()
{
    QModbusRegisters faultInfoRegisters(HEATER_FAULT_STATUS_ADDR, 1);
    pModbusMaster->connect();
    for(int i = 0; i < 5; i ++){
        faultInfoRegisters.setAddress(HEATER_FAULT_STATUS_ADDR + MODBUS_OFFSET_ADDR * i);
        pModbusMaster->readInputRegisters(faultInfoRegisters);
        m_faultInfo[i] = faultInfoRegisters.getUInteger16(0);
    }
    pModbusMaster->close();


    showHeaterFaultInfo();
}

void CHeaterFaultInfo::reflashHeaterControlForm(int index)
{
    if(CHeaterClient::FaultInfo == index){updateHeaterFaultInfo();}
}
