#include "ammo.h"

Ammo::Ammo():
        clip(false),
        eff(0),
        loc("None")
{
}

bool Ammo::addNote(const QString &newVal)
{
    if (notes.contains(newVal))
    {
        return false;
    }
    notes.push_back(newVal);
    return true;
}

QString Ammo::getAllNotes() const
{
    QString result = "";

    QString note;
    foreach (note,notes)
    {
        if (note.isEmpty())
            continue;
        result+=note;
        result+=", ";
    }
    return result.remove(result.length() - 2,2);
}
