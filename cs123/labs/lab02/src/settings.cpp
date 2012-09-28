#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
         loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults()
{
    // Set the default values below
    QSettings s;

    showIntersectSpheres = s.value("showIntersectSpheres", false).toBool();
}

void Settings::saveSettings()
{
    QSettings s;

    s.setValue("showIntersectSpheres", showIntersectSpheres);
}
