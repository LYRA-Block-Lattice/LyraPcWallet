#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QLibraryInfo>

#include "config.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(ICO_WINDOW_SMALL));
    //qDebug() << QLibraryInfo::LibrariesPath;
#ifdef Q_OS_UNIX
    //QCoreApplication::addLibraryPath(QCoreApplication::applicationDirPath());
    //QCoreApplication::addLibraryPath("/snap/gnome-3-34-1804/36/usr/lib/x86_64-linux-gnu");
#endif
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    MainWindow w;
    w.show();
    return a.exec();
}
