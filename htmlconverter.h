#ifndef HTMLCONVERTER_H
#define HTMLCONVERTER_H

class Mecha;
#include <QString>
class SplittablePart;

class HtmlConverter
{
public:
    static bool htmlize(Mecha *mecha, QString& html);

private:
    HtmlConverter() {}
    ~HtmlConverter() {}

    static QString htmlLocs(const SplittablePart* sp);
};

#endif // HTMLCONVERTER_H
