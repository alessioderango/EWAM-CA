#ifndef INFOJUNCTION_H
#define INFOJUNCTION_H

#include <QDialog>
#include <QAbstractButton>

namespace Ui {
class InfoJunction;
}

class InfoJunction : public QDialog
{
    Q_OBJECT

public:
    explicit InfoJunction(QWidget *parent = nullptr);
    ~InfoJunction();
    Ui::InfoJunction *ui;

    bool openDialog;

private slots:
    void on_pushButton_clicked();

    void on_InfoJunction_accepted();

signals:
    void closeEventInfoJunction(InfoJunction*);

private:

};

#endif // INFOJUNCTION_H
