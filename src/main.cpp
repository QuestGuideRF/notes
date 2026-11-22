#include "mainwindow.h"
#include "singleinstance.h"
#include <QApplication>
#include <QFontDatabase>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName("Notes");
    QApplication::setApplicationVersion(APP_VERSION);
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
    QApplication::setDesktopFileName(APP_ID);
#endif
    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-solid-900.ttf") < 0)
        qWarning() << "FontAwesome Solid не может быть загружен!";
    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-regular-400.ttf") < 0)
        qWarning() << "FontAwesome Regular не может быть загружен!";
    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-brands-400.ttf") < 0)
        qWarning() << "FontAwesome Brands не может быть загружен!";
    if (QFontDatabase::addApplicationFont(":/fonts/material/material-symbols-outlined.ttf") < 0)
        qWarning() << "Символы материалов не могут быть загружены!";
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Black.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-ExtraLight.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-ExtraLightItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-SemiBoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/trykker/Trykker-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/mate/Mate-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/mate/Mate-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ptserif/PTSerif-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ptserif/PTSerif-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ptserif/PTSerif-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/ptserif/PTSerif-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iamono/iAWriterMonoS-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iamono/iAWriterMonoS-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iamono/iAWriterMonoS-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iamono/iAWriterMonoS-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaduo/iAWriterDuoS-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaduo/iAWriterDuoS-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaduo/iAWriterDuoS-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaduo/iAWriterDuoS-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaquattro/iAWriterQuattroS-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaquattro/iAWriterQuattroS-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaquattro/iAWriterQuattroS-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/iaquattro/iAWriterQuattroS-BoldItalic.ttf");
    QString name = "com.awsomeness.notes";
    SingleInstance instance;
    if (SingleInstance::hasPrevious(name)) {
        return EXIT_SUCCESS;
    }
    instance.listen(name);
    MainWindow w;
    w.show();
    QObject::connect(&instance, &SingleInstance::newInstance, &w, [&]() { (&w)->setMainWindowVisibility(true); });
    return QApplication::exec();
}