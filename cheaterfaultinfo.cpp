 #include "cheaterfaultinfo.h"
#include "ui_cheaterfaultinfo.h"


#include "heaterConfig.h"
#include "cheaterclient.h"
#include "cheaterclientserver.h"

#include <QDebug>

CHeaterFaultInfo::CHeaterFaultInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CHeaterFaultInfo)
{
    ui->setupUi(this);

    m_groupSwith = 0;
    for(int i = 0; i < FAN_TOWER_GROUP; i ++){
        memset(&(heaterFaultInfo[i]), 0, sizeof(heaterFaultInfo[i]));
    }

    pFaultInfoLabel[0] = ui->faultInfo_0;
    pFaultInfoLabel[1] = ui->faultInfo_1;
    pFaultInfoLabel[2] = ui->faultInfo_2;
    pFaultInfoLabel[3] = ui->faultInfo_3;
    pFaultInfoLabel[4] = ui->faultInfo_4;
}

CHeaterFaultInfo::~CHeaterFaultInfo()
{
    delete ui;
}

void CHeaterFaultInfo::showHeaterFaultInfo()
{
    QString info[5];
    for(int i = 0; i < 5; i ++){
        info[i].clear();
        if(!(heaterFaultInfo[m_groupSwith].faultInfo[i])){info[i] = faultInfoString[6];continue;}
        for(int j = 0; j < 7; j ++){
            if((heaterFaultInfo[m_groupSwith].faultInfo[i]) & (1 << j)){info[i] += faultInfoString[j] + "  ";}
        }
        pFaultInfoLabel[i]->setText(info[i]);
    }
}

void CHeaterFaultInfo::on_heaterGroupSwith_currentIndexChanged(int index)
{
    m_groupSwith = index;
    //showHeaterFaultInfo();
    emit sendClientServerCommand(m_groupSwith, CHeaterClientServer::FaultInfoReadCmd);
}
