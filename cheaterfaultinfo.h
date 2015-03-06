#ifndef CHEATERFAULTINFO_H
#define CHEATERFAULTINFO_H

#include <QWidget>

namespace Ui {
class CHeaterFaultInfo;
}

class CHeaterFaultInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterFaultInfo(QWidget *parent = 0);
    ~CHeaterFaultInfo();
    
private:
    Ui::CHeaterFaultInfo *ui;
};

#endif // CHEATERFAULTINFO_H
