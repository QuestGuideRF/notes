#ifndef QXTGLOBALSHORTCUT_P_H
#define QXTGLOBALSHORTCUT_P_H
#include "qxtglobalshortcut.h"
#include <QAbstractEventDispatcher>
#include <QKeySequence>
#include <QHash>
#include <QAbstractNativeEventFilter>
class QxtGlobalShortcutPrivate
    : public QxtPrivate<QxtGlobalShortcut>
    , public QAbstractNativeEventFilter
{
public:
    QXT_DECLARE_PUBLIC(QxtGlobalShortcut)
    QxtGlobalShortcutPrivate();
    ~QxtGlobalShortcutPrivate();
    bool enabled;
    Qt::Key key;
    Qt::KeyboardModifiers mods;
    quint32 nativeKey;
    quint32 nativeMods;
    bool registered;
    bool setShortcut(const QKeySequence& shortcut);
    bool unsetShortcut();
#   ifndef Q_OS_MAC
    static int ref;
#   endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, qintptr *result);
#else
	virtual bool nativeEventFilter(const QByteArray &eventType, void *message, long *result);
#endif
    static void activateShortcut(quint32 nativeKey, quint32 nativeMods);
private:
    static quint32 nativeKeycode(Qt::Key keycode);
    static quint32 nativeModifiers(Qt::KeyboardModifiers modifiers);
    static bool registerShortcut(quint32 nativeKey, quint32 nativeMods);
    static bool unregisterShortcut(quint32 nativeKey, quint32 nativeMods);
    static QHash<QPair<quint32, quint32>, QxtGlobalShortcut*> shortcuts;
};
#endif