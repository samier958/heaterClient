#ifndef CHEATERPARAMETERSETTINGS_H
#define CHEATERPARAMETERSETTINGS_H

#include <QWidget>

namespace Ui {
class CHeaterParameterSettings;
}

class CHeaterParameterSettings : public QWidget
{
    Q_OBJECT
    
public:
    explicit CHeaterParameterSettings(QWidget *parent = 0);
    ~CHeaterParameterSettings();
    
private:
    Ui::CHeaterParameterSettings *ui;
};

#endif // CHEATERPARAMETERSETTINGS_H
