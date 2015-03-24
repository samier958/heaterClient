#ifndef CHEATERMAINFORM_H
#define CHEATERMAINFORM_H

#include <QWidget>

#include "cheaterclient.h"
#include "heaterConfig.h"

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

private:
    Ui::CHeaterMainForm *ui;
};

#endif // CHEATERMAINFORM_H
