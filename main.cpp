#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLibraryInfo>

#include "config.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(ICO_WINDOW_SMALL));
    //qDebug() << QLibraryInfo::LibrariesPath;
    //qDebug() << a.desktop()->logicalDpiX() / 96.0;
#ifdef Q_OS_UNIX
    //QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    //QCoreApplication::addLibraryPath("/snap/gnome-3-34-1804/36/usr/lib/x86_64-linux-gnu");
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
