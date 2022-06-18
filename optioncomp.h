#ifndef OPTIONCOMP_H
#define OPTIONCOMP_H

#include <QMap>
#include <QStringList>

class OptionComp
{
public:
    static OptionComp* instance();

    const QStringList& getOptions() const {return options;}
    double getCost(const QString& opt) const {return optionCosts[opt];}
    double getSpace(const QString& opt) const {return optionSpace[opt];}
    bool useWeight(const QString& opt) const {return (opt == "Parachute") || (opt == "Re-Entry Package");}

private:
    QStringList options;
    QMap<QString,double> optionCosts;
    QMap<QString,double> optionSpace;

    OptionComp();
    ~OptionComp();
    OptionComp(const OptionComp&); // leave undefined
    OptionComp& operator=(const OptionComp&);// leave undefined
};

#endif // OPTIONCOMP_H
