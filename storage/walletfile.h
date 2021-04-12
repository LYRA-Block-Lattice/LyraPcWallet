#ifndef WALLETFILE_H
#define WALLETFILE_H

#include <QString>

class walletfile
{
public:
    static bool load(QString fileName, QString password);
    static bool save(QString fileName, QString password, QString directory = "");
    static bool exists(QString fileName);
    static bool loadSettings();
    static bool saveSettings();
};

#endif // WALLETFILE_H
