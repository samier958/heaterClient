#ifndef CHEATERMAINFORM_H
#define CHEATERMAINFORM_H

#include <QWidget>


#include "qmodbusmaster.h"

using namespace Modbus;

namespace Ui {
class CHeaterMainForm;
}

class CHeaterMainForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterMainForm(QWidget *parent = 0);
    ~CHeaterMainForm();

signals:
    void establishNetworkConnection();
    void disconnectNetwork();

public slots:
    void reflashHeaterControlForm(int index);
private slots:
    void updateHeaterSystemTime();
    void showHeaterSystemTime();


private:
    Ui::CHeaterMainForm *ui;


    QModbusMaster *pModbusMaster;

    int m_time[5];
};

#endif // CHEATERMAINFORM_H
