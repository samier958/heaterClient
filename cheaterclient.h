#ifndef CHEATERCLIENT_H
#define CHEATERCLIENT_H

#include <QWidget>

namespace Ui {
class CHeaterClient;
}

class QStackedWidget;

class CHeaterClient : public QWidget
{
    Q_OBJECT
    
public:
    enum CHILD_SEESION_FORM{
        MainForm = 1,
        RealTimeData,
        ParameterSetting,
        FaultInfo,
        HistoryRecord,
        ChildSessionForm_Num
    };
    explicit CHeaterClient(QWidget *parent = 0);
    ~CHeaterClient();

private slots:
    void functionSwitchButtonGroupStatusChanged(int id);
    
private:
    Ui::CHeaterClient *ui;


    QStackedWidget *pStackedWidget;
};

#endif // CHEATERCLIENT_H
