#ifndef LAYOUTPRODUCTS_H
#define LAYOUTPRODUCTS_H

#include <QWidget>

namespace Ui {
class layoutProducts;
}

class layoutProducts : public QWidget
{
    Q_OBJECT

public:
    explicit layoutProducts(QWidget *parent = nullptr);
    ~layoutProducts();
    void setProduct(const QString id, const QString name, const QString price);

    QString getId() const;
    void setId(const QString &value);

private:
    Ui::layoutProducts *ui;
    QString id;

signals:
    void added(const QString id, const int ammount);

private slots:
    void on_addPB_clicked();
    void on_ammountSP_valueChanged(int arg1);
};

#endif // LAYOUTPRODUCTS_H
