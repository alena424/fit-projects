#ifndef LUT_H
#define LUT_H

#include <QObject>

class LUT : public QObject
{
    Q_OBJECT
public:
    explicit LUT(QObject *parent = 0);

    // Funkce vrací hodnotu Look up table danou indexem
    // Makro Q_INVOKABLE umožní volání funkce z QML
    Q_INVOKABLE double getValue(int index);

signals:

public slots:

};

#endif // LUT_H
