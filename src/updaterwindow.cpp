#include "updaterwindow.h"
#include "ui_updaterwindow.h"
#include <cmath>
#include <QTimer>
#include <QMessageBox>
#include <QMouseEvent>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QNetworkAccessManager>
#include <QWindow>
#include <QSimpleUpdater.h>
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
#  define UseXdgOpen
#else
#  ifdef UseXdgOpen
#    undef UseXdgOpen
#  endif
#endif
#ifdef UseXdgOpen
#  include <QProcess>
static QProcess XDGOPEN_PROCESS;
#endif
static const QString UPDATES_URL("https://raw.githubusercontent.com/nuttyartist/notes/master/UPDATES_FOSS.json");
UpdaterWindow::UpdaterWindow(QWidget *parent)
    : QDialog(parent),
      m_downloadDir(QDir::homePath() + "/Downloads/"),
      m_ui(new Ui::UpdaterWindow),
      m_canMoveWindow(false),
      m_checkingForUpdates(false),
      m_dontShowUpdateWindow(false),
      m_forced(false),
      m_reply(nullptr),
      m_updater(QSimpleUpdater::getInstance()),
      m_manager(new QNetworkAccessManager(this))
{
    QFile file(":/styles/updater-window.css");
    if (file.open(QIODevice::ReadOnly)) {
        setStyleSheet(QString::fromUtf8(file.readAll()).toLatin1());
        file.close();
    }
    m_ui->setupUi(this);
    setWindowTitle(qApp->applicationName() + " " + tr("Updater"));
#ifdef __APPLE__
    QFont fontToUse = QFont(QStringLiteral("SF Pro Текст")).exactMatch() ? QStringLiteral("SF Pro Текст") : QStringLiteral("Roboto");
#elif _WIN32
    QFont fontToUse = QFont(QStringLiteral("Следуйте пользовательскому интерфейсу")).exactMatch() ? QStringLiteral("Следуйте пользовательскому интерфейсу") : QStringLiteral("Roboto");
#else
    QFont fontToUse = QFont(QStringLiteral("Roboto"));
#endif
    setFont(fontToUse);
    m_ui->changelog->setFont(fontToUse);
    m_ui->changelog->setTextColor(QColor(26, 26, 26));
    foreach (QWidget *widgetChild, findChildren<QWidget *>()) {
        widgetChild->setFont(fontToUse);
    }
    connect(m_ui->closeButton, &QPushButton::clicked, this, &UpdaterWindow::close);
    connect(m_ui->updateButton, &QPushButton::clicked, this, &UpdaterWindow::onDownloadButtonClicked);
    connect(m_updater, &QSimpleUpdater::checkingFinished, this, &UpdaterWindow::onCheckFinished);
    connect(m_ui->checkBox, &QCheckBox::toggled, this, &UpdaterWindow::dontShowUpdateWindowChanged);
    updateTitleLabel();
#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
#else
    setWindowFlags(windowFlags() | Qt::CustomizeWindowHint);
#endif
#ifdef UseXdgOpen
    connect(&XDGOPEN_PROCESS, SIGNAL(finished(int)), this, SLOT(onXdgOpenFinished(int)));
#endif
    setWindowModality(Qt::ApplicationModal);
}
UpdaterWindow::~UpdaterWindow()
{
#ifdef UseXdgOpen
    if (XDGOPEN_PROCESS.isOpen())
        XDGOPEN_PROCESS.close();
#endif
    delete m_ui;
}
void UpdaterWindow::setShowWindowDisable(const bool dontShowWindow)
{
    m_dontShowUpdateWindow = dontShowWindow;
    m_ui->checkBox->setChecked(m_dontShowUpdateWindow);
}
void UpdaterWindow::checkForUpdates(bool force)
{
    if (!m_updater->getUpdateAvailable(UPDATES_URL)) {
        m_checkingForUpdates = true;
        m_forced = force;
        m_updater->setNotifyOnFinish(UPDATES_URL, false);
        m_updater->setNotifyOnUpdate(UPDATES_URL, false);
        m_updater->setDownloaderEnabled(UPDATES_URL, false);
        m_updater->setUseCustomInstallProcedures(UPDATES_URL, true);
        m_updater->setModuleVersion(UPDATES_URL, qApp->applicationVersion());
        m_updater->checkForUpdates(UPDATES_URL);
    }
    if (force) {
        if (!m_updater->getUpdateAvailable(UPDATES_URL)) {
            m_ui->updateButton->setEnabled(false);
            m_ui->title->setText(tr("Проверяю наличие обновлений...."));
        }
        resetControls();
        showNormal();
    }
}
void UpdaterWindow::resetControls()
{
    m_ui->updateButton->setEnabled(false);
    m_ui->installedVersion->setText(qApp->applicationVersion());
    m_ui->availableVersion->setText(m_updater->getLatestVersion(UPDATES_URL));
    if (m_updater->getUpdateAvailable(UPDATES_URL)) {
        m_ui->title->setText(tr("Доступна новая версия!"));
    } else {
        m_ui->title->setText(tr("Вы в курсе!"));
    }
    m_ui->progressControls->hide();
    m_ui->progressBar->setValue(0);
    m_ui->downloadLabel->setText(tr("Загрузка обновлений") + "...");
    m_ui->timeLabel->setText(tr("Оставшееся время") + ": " + tr("unknown"));
    QString changelogText = m_updater->getChangelog(UPDATES_URL);
    m_ui->changelog->setText(changelogText);
    if (m_ui->changelog->toPlainText().isEmpty()) {
        m_ui->changelog->setText("<p>No changelog found...</p>");
    } else {
        m_ui->changelog->setText(changelogText.append("\n"));
    }
    bool available = m_updater->getUpdateAvailable(UPDATES_URL);
    bool validOpenUrl = !m_updater->getOpenUrl(UPDATES_URL).isEmpty();
    bool validDownUrl = !m_updater->getDownloadUrl(UPDATES_URL).isEmpty();
    m_ui->updateButton->setEnabled(available && (validOpenUrl || validDownUrl));
    bool showAgain = isVisible();
    int height = minimumSizeHint().height();
    int width = qMax(minimumSizeHint().width(), int(height * 1.2));
    resize(QSize(width, height));
    if (showAgain) {
        showNormal();
    }
}
void UpdaterWindow::updateTitleLabel()
{
    if (m_checkingForUpdates) {
        static int num = 0;
        QString base = tr("Проверка обновлений");
        QString dot = "";
        num++;
        dot.fill('.', num);
        m_ui->title->setText(base + dot);
        if (num == 4)
            num = 0;
    }
    QTimer::singleShot(500, this, SLOT(updateTitleLabel()));
}
void UpdaterWindow::onUpdateAvailable()
{
    if (!m_dontShowUpdateWindow) {
        resetControls();
        showNormal();
    }
}
void UpdaterWindow::onDownloadButtonClicked()
{
    if (m_updater->getUpdateAvailable(UPDATES_URL)) {
        m_ui->updateButton->setEnabled(false);
        startDownload(m_updater->getDownloadUrl(UPDATES_URL));
    }
}
void UpdaterWindow::startDownload(const QUrl &url)
{
    if (url.isEmpty()) {
        QDesktopServices::openUrl(QUrl(m_updater->getOpenUrl(UPDATES_URL)));
        return;
    }
    if (m_reply) {
        m_reply->abort();
        m_reply->deleteLater();
    }
    m_startTime = QDateTime::currentDateTime().toSecsSinceEpoch();
    QNetworkRequest netReq(url);
    netReq.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    m_reply = m_manager->get(netReq);
    m_fileName = m_updater->getDownloadUrl(UPDATES_URL).split("/").last();
    if (m_fileName.isEmpty()) {
        m_fileName = QStringLiteral("%1_Update_%2.bв").arg(QCoreApplication::applicationName(), m_updater->getLatestVersion(UPDATES_URL));
    }
    if (!m_downloadDir.exists())
        m_downloadDir.mkpath(".");
    QFile::remove(m_downloadDir.filePath(m_fileName));
    m_ui->progressControls->show();
    showNormal();
    connect(m_reply, &QNetworkReply::downloadProgress, this, &UpdaterWindow::updateProgress);
    connect(m_reply, &QNetworkReply::finished, this, &UpdaterWindow::onDownloadFinished);
}
void UpdaterWindow::openDownload(const QString &file)
{
    if (file.isEmpty()) {
        return;
    }
    m_ui->downloadLabel->setText(tr("Загрузка завершена!"));
    m_ui->timeLabel->setText(tr("Открытие скачанного файла") + "...");
#ifndef UseXdgOpen
    bool openUrl = QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    if (!openUrl) {
        openDownloadFolder(file);
        m_ui->progressControls->hide();
        qApp->processEvents();
        showNormal();
        close();
    } else {
        qApp->quit();
    }
#endif
#ifdef UseXdgOpen
    XDGOPEN_PROCESS.start("xdg-open", QStringList() << file);
#endif
}
void UpdaterWindow::onCheckFinished(const QString &url)
{
    Q_UNUSED(url)
    m_checkingForUpdates = false;
    if (m_updater->getUpdateAvailable(url) && (UPDATES_URL == url)) {
        onUpdateAvailable();
    } else if (m_forced) {
        m_forced = false;
        resetControls();
        showNormal();
    }
}
void UpdaterWindow::onXdgOpenFinished(const int exitCode)
{
#ifdef UseXdgOpen
    const auto arguments = XDGOPEN_PROCESS.arguments();
    if (exitCode != 0 && !arguments.isEmpty()) {
        openDownloadFolder(arguments.first());
    } else {
        QCoreApplication::quit();
    }
#else
    Q_UNUSED(exitCode);
#endif
}
void UpdaterWindow::openDownloadFolder(const QString &file)
{
    QString extension = file.split(".").last();
    QMessageBox::information(this, tr("Ошибка открытия"),
                             tr("Похоже, в вашей ОС нет "
                                "приложение, которое может обрабатывать файлы *.%1. "
                                "Мы откроем для вас папку загрузок.")
                                     .arg(extension),
                             QMessageBox::Ok);
    QString nativePath = QDir::toNativeSeparators(QDir(file).absolutePath());
    QStringList directories = nativePath.split(QDir::separator());
    directories.removeLast();
    QString path = directories.join(QDir::separator());
    QUrl url = QUrl::fromLocalFile(QDir(path).absolutePath());
    QDesktopServices::openUrl(url);
}
void UpdaterWindow::calculateSizes(qint64 received, qint64 total)
{
    QString totalSize;
    QString receivedSize;
    if (total < 1024) {
        totalSize = tr("%1 байт").arg(total);
    } else if (total < (1024 * 1024)) {
        totalSize = tr("%1 КБ").arg(round(total / 1024));
    } else {
        totalSize = tr("%1 МБ").arg(round(total / (1024 * 1024)));
    }
    if (received < 1024) {
        receivedSize = tr("%1 байт").arg(received);
    } else if (received < (1024 * 1024)) {
        receivedSize = tr("%1 КБ").arg(received / 1024);
    } else {
        receivedSize = tr("%1 МБ").arg(received / (1024 * 1024));
    }
    m_ui->downloadLabel->setText(tr("Загрузка обновлений") + " (" + receivedSize + " " + tr("of") + " " + totalSize + ")");
}
void UpdaterWindow::updateProgress(qint64 received, qint64 total)
{
    if (total > 0) {
        m_ui->progressBar->setMinimum(0);
        m_ui->progressBar->setMaximum(100);
        m_ui->progressBar->setValue(int((received * 100) / total));
        calculateSizes(received, total);
        calculateTimeRemaining(received, total);
    } else {
        m_ui->progressBar->setMinimum(0);
        m_ui->progressBar->setMaximum(0);
        m_ui->progressBar->setValue(-1);
        m_ui->downloadLabel->setText(tr("Загрузка обновлений") + "...");
        m_ui->timeLabel->setText(QStringLiteral("%1: %2").arg(tr("Оставшееся время"), tr("unknown")));
    }
}
void UpdaterWindow::calculateTimeRemaining(qint64 received, qint64 total)
{
    uint difference = QDateTime::currentDateTime().toSecsSinceEpoch() - m_startTime;
    if (difference > 0) {
        QString timeString;
        qreal timeRemaining = total / (received / difference);
        if (timeRemaining > 7200) {
            timeRemaining /= 3600;
            int hours = int(timeRemaining + 0.5);
            if (hours > 1) {
                timeString = tr("около %1 часов").arg(hours);
            } else {
                timeString = tr("около часа");
            }
        } else if (timeRemaining > 60) {
            timeRemaining /= 60;
            int minutes = int(timeRemaining + 0.5);
            if (minutes > 1) {
                timeString = tr("%1 минут").arg(minutes);
            } else {
                timeString = tr("1 минута");
            }
        } else {
            int seconds = int(timeRemaining + 0.5);
            if (seconds > 1) {
                timeString = tr("%1 секунды").arg(seconds);
            } else {
                timeString = tr("1 секунда");
            }
        }
        m_ui->timeLabel->setText(tr("Оставшееся время") + ": " + timeString);
    }
}
void UpdaterWindow::onDownloadFinished()
{
    QString redirectedUrl = m_reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();
    if (redirectedUrl.isEmpty()) {
        const QString filePath = m_downloadDir.filePath(m_fileName);
        QFile file(filePath);
        if (file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            file.write(m_reply->readAll());
            file.close();
            qApp->processEvents();
        }
        openDownload(filePath);
    } else {
        startDownload(redirectedUrl);
    }
}
void UpdaterWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (event->position().x() < width() - 5 && event->position().x() > 5 && event->position().toPoint().y() < height() - 5
            && event->position().toPoint().y() > 5) {
            m_canMoveWindow = !window()->windowHandle()->startSystemMove();
            m_mousePressX = event->position().toPoint().x();
            m_mousePressY = event->position().toPoint().y();
        }
    }
    event->accept();
}
void UpdaterWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_canMoveWindow) {
        int dx = event->globalPosition().x() - m_mousePressX;
        int dy = event->globalPosition().y() - m_mousePressY;
        move(dx, dy);
    }
}
void UpdaterWindow::mouseReleaseEvent(QMouseEvent *event)
{
    m_canMoveWindow = false;
    unsetCursor();
    event->accept();
}
qreal UpdaterWindow::round(qreal input)
{
    return qreal(roundf(float(input * 100)) / 100);
}