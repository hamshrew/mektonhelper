#include "optioncomp.h"

OptionComp::OptionComp()
{
    options.push_back("Stereo");
    options.push_back("Liftwire");
    options.push_back("Antitheft Code Lock");
    options.push_back("Antitheft Code Lock w/ Alarm");
    options.push_back("Spotlights");
    options.push_back("Nightlights");
    options.push_back("Storage Module");
    options.push_back("Micromanipulators");
    options.push_back("Slick Spray");
    options.push_back("Bogg Spray");
    options.push_back("Damage Control Package");
    options.push_back("Quick-Change Mount");
    options.push_back("Silent Running");
    options.push_back("Parachute");
    options.push_back("Re-Entry Package");
    options.push_back("Ejection Seat");
    options.push_back("Escape Pod");
    options.push_back("Maneuver Pod");
    options.push_back("Vehicle Pod");

    optionCosts["Stereo"] = 0.1;
    optionCosts["Liftwire"] = 0.3;
    optionCosts["Antitheft Code Lock"] = 0.2;
    optionCosts["Antitheft Code Lock w/ Alarm"] = 0.4;
    optionCosts["Spotlights"] = 0.2;
    optionCosts["Nightlights"] = 0.5;
    optionCosts["Storage Module"] = 1;
    optionCosts["Micromanipulators"] = 1;
    optionCosts["Slick Spray"] = 3;
    optionCosts["Bogg Spray"] = 3;
    optionCosts["Damage Control Package"] = 1;
    optionCosts["Quick-Change Mount"] = 1;
    optionCosts["Silent Running"] = 1;
    optionCosts["Parachute"] = 0.5;
    optionCosts["Re-Entry Package"] = 1;
    optionCosts["Ejection Seat"] = 1;
    optionCosts["Escape Pod"] = 2;
    optionCosts["Maneuver Pod"] = 4;
    optionCosts["Vehicle Pod"] = 6;

    optionSpace["Stereo"] = 0;
    optionSpace["Liftwire"] = 0;
    optionSpace["Antitheft Code Lock"] = 0;
    optionSpace["Antitheft Code Lock w/ Alarm"] = 0;
    optionSpace["Spotlights"] = 0;
    optionSpace["Nightlights"] = 0;
    optionSpace["Storage Module"] = 1;
    optionSpace["Micromanipulators"] = 1;
    optionSpace["Slick Spray"] = 1;
    optionSpace["Bogg Spray"] = 1;
    optionSpace["Damage Control Package"] = 1;
    optionSpace["Quick-Change Mount"] = 0;
    optionSpace["Silent Running"] = 0;
    optionSpace["Parachute"] = 0.5;
    optionSpace["Re-Entry Package"] = 1;
    optionSpace["Ejection Seat"] = 0;
    optionSpace["Escape Pod"] = 1;
    optionSpace["Maneuver Pod"] = 2;
    optionSpace["Vehicle Pod"] = 3;
}

OptionComp* OptionComp::instance()
{
    static OptionComp* _instance = 0;
    if (_instance)
    {
        return _instance;
    }

    _instance = new OptionComp();
    return _instance;
}
