#include "products.h"

Products::Products()
{

}

QString Products::getId() const
{
    return id;
}

void Products::setId(const QString &value)
{
    id = value;
}

QString Products::getName() const
{
    return name;
}

void Products::setName(const QString &value)
{
    name = value;
}

double Products::getPrice() const
{
    return price;
}

void Products::setPrice(double value)
{
    price = value;
}

int Products::getSold() const
{
    return sold;
}

void Products::setSold(int value)
{
    sold = value;
}


