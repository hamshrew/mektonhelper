#ifndef MEKTON_DEFS_H
#define MEKTON_DEFS_H

#include <QString>
#include <QMap>

enum ServoSizes {
    SUPERLIGHT,LIGHT_WEIGHT,STRIKER,MED_STRIKER,HVY_STRIKER,MED_WT,LT_HVY,MED_HVY,AR_HVY,SUP_HVY,MEGA_HVY
};

enum ServoTypes {
    TORSO,HEAD,ARM,LEG,POD,WING,TAIL
};

enum ArmorTypes {
    ABLATIVE,
    NORMAL,
    ALPHA,
    BETA,
    GAMMA
};

enum RAMTypes {
    NONE,
    FIFTH,
    FOURTH,
    THIRD,
    HALF
};

enum EnvTypes {
    ARCTIC,
    DESERT,
    UNDERWATER,
    HIGH_PRESSURE,
    SPACE,
    EM,
    RE_ENTRY
};

enum PlantSources {
    FUSION,
    BIOENERGY,
    POWERCELL,
    COMBUSTION
};

#endif // MEKTON_DEFS_H
