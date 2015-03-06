#ifndef CHEATERMAINFORM_H
#define CHEATERMAINFORM_H

#include <QWidget>

namespace Ui {
class CHeaterMainForm;
}

class CHeaterMainForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterMainForm(QWidget *parent = 0);
    ~CHeaterMainForm();
    
private:
    Ui::CHeaterMainForm *ui;
};

#endif // CHEATERMAINFORM_H
