#ifndef INTERFACE_H
#define INTERFACE_H

#include <QWidget>

namespace Ui {
class TInterface;
}

class TInterface : public QWidget
{
    Q_OBJECT

public:
    explicit TInterface(QWidget *parent = nullptr);
    ~TInterface();

private:
    Ui::TInterface *ui;
};

#endif // INTERFACE_H
