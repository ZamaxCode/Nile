#ifndef PRODUCTS_H
#define PRODUCTS_H
#include <QString>

class Products
{
public:
    Products();

    QString getId() const;
    void setId(const QString &value);

    QString getName() const;
    void setName(const QString &value);

    double getPrice() const;
    void setPrice(double value);

    int getSold() const;
    void setSold(int value);

private:
    QString id;
    QString name;
    double price;
    int sold;
};

#endif // PRODUCTS_H
