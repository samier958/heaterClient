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
    QString systemTimeString[5] = {
        tr("年"),
        tr("月"),
        tr("日   "),
        tr(":"),
        tr(" ")
    };
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
};

#endif // CHEATERMAINFORM_H
