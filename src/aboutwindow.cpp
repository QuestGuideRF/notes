#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include <QDebug>
#include <QFile>
#include <QScrollBar>
#include <qstyle.h>
#include <sstream>
#include "editorsettingsoptions.h"
AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), m_ui(new Ui::AboutWindow), m_isProVersion(false)
{
    m_ui->setupUi(this);
    setWindowTitle(tr("About") + " " + QCoreApplication::applicationName());
    setAboutText();
    m_ui->aboutText->setTextColor(QColor(26, 26, 26));
#ifdef __APPLE__
    QFont fontToUse = QFont(QStringLiteral("SF Pro Текст")).exactMatch() ? QStringLiteral("SF Pro Текст") : QStringLiteral("Roboto");
    m_ui->aboutText->setFont(fontToUse);
#elif _WIN32
    QFont fontToUse = QFont(QStringLiteral("Следуйте пользовательскому интерфейсу")).exactMatch() ? QStringLiteral("Следуйте пользовательскому интерфейсу") : QStringLiteral("Roboto");
    m_ui->aboutText->setFont(fontToUse);
#else
    m_ui->aboutText->setFont(QFont(QStringLiteral("Roboto")));
#endif
    QFile cssFile(":/styles/about-window.css");
    cssFile.open(QFile::ReadOnly);
    m_ui->aboutText->setStyleSheet(cssFile.readAll());
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
    QFile scollBarStyleFile(QStringLiteral(":/styles/components/custom-scrollbar.css"));
    scollBarStyleFile.open(QFile::ReadOnly);
    QString scrollbarStyleSheet = QString::fromLatin1(scollBarStyleFile.readAll());
    m_ui->aboutText->verticalScrollBar()->setStyleSheet(scrollbarStyleSheet);
#endif
}
AboutWindow::~AboutWindow()
{
    delete m_ui;
}
void AboutWindow::setAboutText()
{
    QString proVersionText = m_isProVersion ? " (Про версия)" : "";
    m_ui->aboutText->setText("<strong>Version:</strong> " + QCoreApplication::applicationVersion() + proVersionText
                             + "<p>Notes was founded by <a href='https://rubymamistvalove.com'>Ruby "
                               "Мамиствалов</a>, "
                               "создать элегантную, но мощную кроссплатформенную программу для создания заметок с открытым исходным кодом. "
                               "app.</p><a href='https://www.notes-foss.com'>Notes Website"
                               "</a><br/><a href='https://github.com/nuttyartist/notes'>Source code on "
                               "Github</a><br/><a "
                               "href='https://www.notes-foss.com/notes-app-terms-privacy-policy'>Terms and Privacy "
                               "Policy</a><br/><br/><strong>Acknowledgments</strong><br/>This project couldn't "
                               "have "
                               "зайти так далеко без помощи этих удивительных "
                               "people:<br/><br/><strong>Programmers:</strong><br/>Alex Spataru<br/>Ali Diouri"
                               "<br/>David Planella<br/>Diep Ngoc<br/>Guilherme "
                               "Silva<br/>Thorbjørn Lindeijer<br/>Tuur Vanhoutte<br/>Waqar "
                               "Ahmed<br/><br/><strong>Designers:</strong><br/>Kevin Doyle<br/><br/>And to the "
                               "многие из наших любимых пользователей, которые продолжают присылать нам отзывы, вы — важная сила "
                               "in "
                               "helping us improve, thank you!<br/><br/><strong>Notes makes use of the following "
                               "third-party libraries:</strong><br/><br/>QMarkdownTextEdit<br/>"
#if defined(UPDATE_CHECKER)
                               "QSimpleUpdater<br/>"
#endif
                               "QAutostart<br/>QXT<br/><br/><strong>Notes makes use of the following open source "
                               "fonts:</strong><br/><br/>Roboto<br/>Source Sans Pro<br/>Trykker<br/>Mate<br/>iA "
                               "Writer Mono<br/>iA Writer Duo<br/>iA Writer "
                               "Quattro<br/>Font Awesome<br/>Material Symbols<br/><br/><strong>Qt version:</strong> "
                             + qVersion() + " (построен с " + QT_VERSION_STR + ")");
}
void AboutWindow::setTheme(Theme::Value theme)
{
    setCSSThemeAndUpdate(m_ui->aboutText, theme);
}
void AboutWindow::setProVersion(bool isProVersion)
{
    m_isProVersion = isProVersion;
    setAboutText();
}