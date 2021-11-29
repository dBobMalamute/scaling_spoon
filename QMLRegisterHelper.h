#ifndef QMLREGISTERHELPER_H
#define QMLREGISTERHELPER_H

#include "Corkboard.h"
#include "CorkboardHandler.h"
#include "Styling/CorkboardStyle.h"

#include "Casing.h"
#include "Styling/CasingStyle.h"

#include "Wire.h"
#include "Styling/WireStyle.h"

#include "PostIt.h"
#include "Styling/PostItStyle.h"

#include "Styling/ControlsStyle.h"

#include "Styling/GearStyle.h"

void registerQML(QObject* app)
{
    qmlRegisterType<Corkboard>("com.malamute.core",1,0,"MalamuteCorkboard");
    qmlRegisterType<CorkboardHandler>("com.malamute.core", 1, 0, "MalamuteCorkboardHandler");
    CorkboardStyle* corkboardStyle = new CorkboardStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "CorkboardStyle", corkboardStyle);

    qmlRegisterType<Casing>("com.malamute.core",1,0,"MalamuteCasing");
    CasingStyle* casingStyle = new CasingStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "CasingStyle", casingStyle);

    qmlRegisterType<Wire>("com.malamute.core",1,0,"MalamuteWire");
    WireStyle* wireStyle = new WireStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "WireStyle", wireStyle);

    qmlRegisterType<PostIt>("com.malamute.core",1,0,"MalamutePostIt");
    PostItStyle* postItStyle = new PostItStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "PostItStyle", postItStyle);

    ControlsStyle* controlsStyle = new ControlsStyle(app);
    qmlRegisterSingletonInstance("com.malamute.controlsStyles", 1, 0, "ControlsStyle", controlsStyle);

    GearStyle* gearStyle = new GearStyle(app);
    qmlRegisterSingletonInstance("com.malamute.core", 1, 0, "GearStyle", gearStyle);
}


#endif // QMLREGISTERHELPER_H
