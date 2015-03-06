#ifndef CHEATERREALTIMEDATA_H
#define CHEATERREALTIMEDATA_H

#include <QWidget>

namespace Ui {
class CHeaterRealTimeData;
}

class CHeaterRealTimeData : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterRealTimeData(QWidget *parent = 0);
    ~CHeaterRealTimeData();
    
private:
    Ui::CHeaterRealTimeData *ui;
};

#endif // CHEATERREALTIMEDATA_H
