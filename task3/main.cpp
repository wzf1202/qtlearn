#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    const QLocale locale(QLocale::Chinese, QLocale::China);
    QString region = locale.name();
    region.replace(u'-', u'_');
    QTranslator translator(&app);
    if (translator.load(QStringLiteral("task3"), QStringLiteral(":/i18n/mui/") + region))
        app.installTranslator(&translator);
    MainWindow mainWindow(nullptr);
    mainWindow.show();
    return QApplication::exec();
}
