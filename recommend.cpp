#include "recommend.h"

Recommend::Recommend()
{

}

QString Recommend::getId() const
{
    return id;
}

void Recommend::setId(const QString &value)
{
    id = value;
}

int Recommend::getPeso() const
{
    return peso;
}

void Recommend::setPeso(int value)
{
    peso = value;
}
