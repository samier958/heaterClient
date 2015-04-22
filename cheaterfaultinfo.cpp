#include "cheaterfaultinfo.h"
#include "ui_cheaterfaultinfo.h"


#include "heaterConfig.h"
#include "cheaterclient.h"

CHeaterFaultInfo::CHeaterFaultInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterFaultInfo)
{
    ui->setupUi(this);

    m_groupSwith = 0;
    for(int i = 0; i < FAN_TOWER_GROUP; i ++){
        memset(&(heaterFaultInfo[i]), 0, sizeof(heaterFaultInfo[i]));
    }

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
    for(int i = 0; i < 5; i ++){
        for(int j = 0; j < 7; j ++){
            if(!(heaterFaultInfo[m_groupSwith].faultInfo[i])){pFaultInfoLabel[i]->setText(faultInfoString[6]);break;}
            if((heaterFaultInfo[m_groupSwith].faultInfo[i]) & (1 << j)){pFaultInfoLabel[i]->setText(faultInfoString[j]);}
        }
    }
}

void CHeaterFaultInfo::on_heaterGroupSwith_currentIndexChanged(int index)
{
    m_groupSwith = index;
    showHeaterFaultInfo();
}
