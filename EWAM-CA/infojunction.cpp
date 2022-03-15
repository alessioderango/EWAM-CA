#include "infojunction.h"
#include "ui_infojunction.h"
#include "ewamca.h"

InfoJunction::InfoJunction(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InfoJunction)
{
    ui->setupUi(this);
    openDialog = false;
}

InfoJunction::~InfoJunction()
{
    delete ui;
}


void InfoJunction::on_pushButton_clicked()
{
    emit closeEventInfoJunction(this);
    openDialog = false;
    this->close();
}



void InfoJunction::on_InfoJunction_accepted()
{
    openDialog = true;
}
