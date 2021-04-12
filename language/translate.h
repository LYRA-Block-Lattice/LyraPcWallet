#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QString>

QString _tr(QString str);
int _scale(int value, double extra = 1.0);

class translate {
public:
    static bool loadLanguage(QString lahguage);
    static QString getCurrentLang();
    static QString getCurrentFontLight();
    static int getNumberFontSize(double scale = 1.0);
    static int getCurrentFontSizeLight(double scale = 1.0);
    static QString getCurrentFontMedium();
    static int getCurrentFontSizeMedium(double scale = 1.0);
    static QString getCurrentFontRegular();
    static int getCurrentFontSizeRegular(double scale = 1.0);
};

#endif // TRANSLATE_H
