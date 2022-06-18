#ifndef WEAPON_H
#define WEAPON_H

#include <cmath>
#include "splittablepart.h"

#include <QString>
#include <QStringList>
#include <QPair>

class Weapon : public SplittablePart
{
public:
    Weapon();
    virtual ~Weapon();

    // Accessors
    int getAcc() const {return acc;}
    unsigned int getRange() const {return range;}
    unsigned int getDamage() const {return damage;}
    const QString& getShots() const {return shots;}
    const QString& getShotPrepend() const {return shotPrepend;}
    unsigned int getKills() const {return kills;}
    double getBaseCost() const {return cost;}
    double getBaseSpace() const {return space;}
    double getWeight() const {return kills/2.0;}
    bool isMelee() const {return melee;}
    bool isClumsy() const {return clumsy;}
    bool isUseAmmo() const {return useAmmo;}
    bool isUseClips() const {return useClips;}
    bool isHandy() const {return handy;}
    const QString& getType() const {return type;}
    const QStringList& getNotes() const {return notes;}
    QString getAllNotes() const;
    double getLinkCost() const;
    QString getLinks() const;
    const QString& getComments() const {return comments;}
    QString getFormattedComments(int brk = 50) const;
    const QList<QPair<Weapon*,bool> >& getLinkList() const {return linkList;}

    // Set Methods
    void setAcc(int newVal) {acc = newVal;}
    void setRange(unsigned int newVal) {range = newVal;}
    void setDamage(unsigned int newVal) {damage = newVal;}
    void setShots(const QString& newVal) {shots = newVal;}
    void setShotPrepend(const QString& newVal) {shotPrepend = newVal;}
    void setKills(unsigned int newVal) {kills = newVal;}
    void setCost(qreal newVal) {cost = newVal;}
    void setSpace(double newVal) {space = newVal;}
    void setMelee(bool newVal) {melee = newVal;}
    void setClumsy(bool newVal) {clumsy = newVal;}
    void setHandy(bool newVal) {handy = newVal;}
    void setUseAmmo(bool newVal) {useAmmo = newVal;}
    void setUseClips(bool newVal) {useClips = newVal;}
    void setType(const QString& newVal) {type = newVal;}
    void setNotes(const QStringList& newVal) {notes = newVal;}
    bool addNote(const QString& newVal);
    void setComments(const QString& newVal) {comments = newVal;}

    void addLink(Weapon* wp,bool cross);
    void remLink(Weapon* wp);
    void clearLinks() {linkList.clear();}
    void addLinkFrom(Weapon* wp);
    void remLinkFrom(Weapon* wp);

    bool operator<(const Weapon& rhs) const {return this->getType().compare(rhs.getType()) < 0;}

private:
    int acc;
    unsigned int range;
    unsigned int damage;
    QString shots;
    QString shotPrepend;
    unsigned int kills;
    double cost;
    double space;
    bool melee;
    bool clumsy;
    bool handy;
    bool useAmmo;
    bool useClips;
    QString type;
    QStringList notes;
    QString comments;

    // Stuff for Linkage
    QList<QPair<Weapon*,bool> > linkList;
    QList<Weapon*> linkedFrom;
};

//static bool wpLessThan(Weapon* l,Weapon* r) {return l->getType().compare(r->getType()) < 0;}

#endif // WEAPON_H
