#include "layoutproducts.h"
#include "ui_layoutproducts.h"

layoutProducts::layoutProducts(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::layoutProducts)
{
    ui->setupUi(this);
}

layoutProducts::~layoutProducts()
{
    delete ui;
}

void layoutProducts::setProduct(const QString id, const QString name, const QString price)
{
    QPixmap imagen("Z:/Personal/Escritorio/TAREAS - TRABAJOS/4to SEMESTRE/SALG/Etapa 7/Interfaz/interfaz/imgs/"+id+".jpg");

    ui->imageLB->setPixmap(imagen.scaled(100,100,Qt::KeepAspectRatio));
    ui->nameProdLB->setText(name);
    ui->priceLB->setText(price);

}

QString layoutProducts::getId() const
{
    return id;
}

void layoutProducts::setId(const QString &value)
{
    id = value;
}

void layoutProducts::on_addPB_clicked()
{
    ui->ammountSP->setValue(0);
    emit added(id, ui->ammountSP->value());
}

void layoutProducts::on_ammountSP_valueChanged(int arg1)
{
    if(arg1)
        ui->addPB->setEnabled(true);
    else
        ui->addPB->setEnabled(false);

}
