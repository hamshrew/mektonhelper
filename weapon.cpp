#include "weapon.h"

Weapon::Weapon():
        melee(false),
        clumsy(false),
        handy(false),
        useAmmo(false),
        useClips(false)
{
}

Weapon::~Weapon()
{
    foreach(Weapon* wp,linkedFrom)
    {
        wp->remLink(this);
    }

    QPair<Weapon*,bool> wp;

    foreach(wp,linkList)
    {
        wp.first->remLinkFrom(this);
    }
}

QString Weapon::getFormattedComments(int brk) const
{
    QString final;

    QString temp;
    QString last;

    int total = 0;
    int newNum = 0;

    QRegExp re("\\s+");

    while (1)
    {
        temp = comments.section(re,total,newNum);
        if (temp == last)
        {
            final += last;
            break;
        }

        if (temp.length() > brk)
        {
            final += last + "\n";
            last = "";
            total = newNum;
        }
        else
        {
            last = temp;
            ++newNum;
        }
    }

    return final;
}

bool Weapon::addNote(const QString &newVal)
{
    if (notes.contains(newVal))
    {
        return false;
    }
    notes.push_back(newVal);
    return true;
}

QString Weapon::getAllNotes() const
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

void Weapon::addLink(Weapon *wp, bool cross)
{
    // Check to see if the weapon is in the list already
    QPair<Weapon*,bool> pr;

    foreach (pr,linkList)
    {
        if (pr.first == wp)
        {
            pr.second = cross;
            return;
        }
    }

    // Otherwise, add it!
    linkList.push_back(QPair<Weapon*,bool>(wp,cross));
    wp->addLinkFrom(this);
}

void Weapon::remLink(Weapon *wp)
{
    QList<QPair<Weapon*,bool> >::iterator it = linkList.begin();

    while (it != linkList.end())
    {
        if ((*it).first == wp)
        {
            (*it).first->remLinkFrom(this);
            it = linkList.erase(it);
            return;
        }
        ++it;
    }
}

void Weapon::addLinkFrom(Weapon *wp)
{
    if (!linkedFrom.contains(wp))
        linkedFrom.push_back(wp);
}

void Weapon::remLinkFrom(Weapon *wp)
{
    if (linkedFrom.contains(wp))
        linkedFrom.removeOne(wp);
}

double Weapon::getLinkCost() const
{
    double cost = 0;

    QPair<Weapon*,bool> pr;

    foreach (pr,linkList)
    {
        cost += 2;

        // Need to account for Splits somehow, so we'll assume they cost extra
        if (pr.first->getLoc() != getLoc() || pr.first->getLoc() == "Split" || getLoc() == "Split")
        {
            ++cost;
        }

        if (pr.second)
            ++cost;

        if ((pr.first->getType() == "Melee" || pr.first->getType() == "EMW")
            && (getType() == "Melee" || getType() == "EMW"))
            cost+= 2;
    }

    return cost;
}

QString Weapon::getLinks() const
{
    QString linkStr;

    if (linkList.empty())
        return linkStr;

    QList<QPair<Weapon*,bool> >::const_iterator it = linkList.constBegin();

    while (it != linkList.constEnd())
    {
        if ((*it).second)
            linkStr += "Cross-";

        linkStr += "Linked to " + (*it).first->getName();

        ++it;

        if (it != linkList.constEnd())
            linkStr += ", ";
    }

    return linkStr;
}
