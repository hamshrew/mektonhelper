#ifndef BASESPECENTRY_H
#define BASESPECENTRY_H

#include <QWidget>

class Weapon;

class BaseSpecEntry : public QWidget
{
    Q_OBJECT
public:
    explicit BaseSpecEntry(QWidget *parent = 0);
    virtual ~BaseSpecEntry();
    virtual Weapon* createWeaponPtr() = 0;

signals:

public slots:

};

#endif // BASESPECENTRY_H
