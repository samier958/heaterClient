#ifndef CHEATERHISTORYRECORD_H
#define CHEATERHISTORYRECORD_H

#include <QWidget>

namespace Ui {
class CHeaterHistoryRecord;
}

class CHeaterHistoryRecord : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterHistoryRecord(QWidget *parent = 0);
    ~CHeaterHistoryRecord();
    
private:
    Ui::CHeaterHistoryRecord *ui;
};

#endif // CHEATERHISTORYRECORD_H
