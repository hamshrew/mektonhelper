#ifndef AMMO_H
#define AMMO_H

#include <QStringList>

class Ammo
{
public:
    Ammo();

    // Accessors
    const QString& getWeaponName() const {return weaponName;}
    int getAcc() const {return acc;}
    int getDamage() const {return damage;}
    int getShots() const {return shots;}
    int getClips() const {return clips;}
    unsigned int getKills() const {return kills;}
    const QString& getLoc() const {return loc;}
    float getSpace() const {return (space - eff) < 0 ? 0 : (space - eff);}
    float getBaseSpace() const {return space;}
    float getCost() const {return cost + eff/2.0;}
    float getWeight() const {return kills/2.0;}
    double getEff() const {return eff;}
    bool isClip() const {return clip;}
    const QStringList& getNotes() const {return notes;}
    QString getAllNotes() const;

    // Set Methods
    void setWeaponName(const QString& newVal) {weaponName = newVal;}
    void setAcc(int newVal) {acc = newVal;}
    void setDamage(unsigned int newVal) {damage = newVal;}
    void setShots(int newVal) {shots = newVal;}
    void setClips(int newVal) {clips = newVal;}
    void setKills(unsigned int newVal) {kills = newVal;}
    void setLoc(const QString& newVal) {loc = newVal;}
    void setCost(float newVal) {cost = newVal;}
    void setSpace(float newVal) {space = newVal;}
    void setIsClip(bool newVal) {clip = newVal;}
    void setEff(double newVal) {eff = (newVal > space) ? space: newVal;}
    void setNotes(const QStringList& newVal) {notes = newVal;}
    bool addNote(const QString& newVal);

private:
    QString weaponName;
    bool clip;
    int damage;
    int acc;
    int shots;
    int clips;
    float cost;
    float space;
    double eff;
    unsigned int kills;
    QString loc;
    QStringList notes;
};

#endif // AMMO_H
