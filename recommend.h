#ifndef RECOMMEND_H
#define RECOMMEND_H
#include <QString>

class Recommend
{
public:
    Recommend();
    QString getId() const;
    void setId(const QString &value);

    int getPeso() const;
    void setPeso(int value);

private:
    QString id;
    int peso;
};

#endif // RECOMMEND_H
