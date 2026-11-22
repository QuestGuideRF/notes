#include "qxtglobalshortcut.h"
#include "qxtglobalshortcut_p.h"
#include <QAbstractEventDispatcher>
#include <QtDebug>
#ifndef Q_OS_MAC
int QxtGlobalShortcutPrivate::ref = 0;
#endif
QHash<QPair<quint32, quint32>, QxtGlobalShortcut*> QxtGlobalShortcutPrivate::shortcuts;
QxtGlobalShortcutPrivate::QxtGlobalShortcutPrivate()
    : enabled(true)
    , key(Qt::Key(0))
    , mods(Qt::NoModifier)
    , nativeKey(0)
    , nativeMods(0)
    , registered(false)
{
#ifndef Q_OS_MAC
    if (ref == 0) {
        QAbstractEventDispatcher::instance()->installNativeEventFilter(this);
    }
    ++ref;
#endif
}
QxtGlobalShortcutPrivate::~QxtGlobalShortcutPrivate()
{
    unsetShortcut();
#ifndef Q_OS_MAC
    --ref;
    if (ref == 0) {
        QAbstractEventDispatcher *ed = QAbstractEventDispatcher::instance();
        if (ed != 0) {
            ed->removeNativeEventFilter(this);
        }
    }
#endif
}
#include <iostream>
bool QxtGlobalShortcutPrivate::setShortcut(const QKeySequence& shortcut)
{
    unsetShortcut();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Qt::Key key = shortcut[0].key();
    Qt::KeyboardModifiers mods = shortcut[0].keyboardModifiers();
#else
	const Qt::KeyboardModifiers allMods =
			Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    const uint xkeyCode = shortcut[0];
    const uint keyCode = QChar::toLower(xkeyCode & ~allMods);
    key = Qt::Key(keyCode);
    mods = Qt::KeyboardModifiers(xkeyCode & allMods);
#endif
    nativeKey = nativeKeycode(key);
    nativeMods = nativeModifiers(mods);
    registered = registerShortcut(nativeKey, nativeMods);
    if (registered)
        shortcuts.insert(qMakePair(nativeKey, nativeMods), &qxt_p());
    return registered;
}
bool QxtGlobalShortcutPrivate::unsetShortcut()
{
    if (registered
            && shortcuts.value(qMakePair(nativeKey, nativeMods)) == &qxt_p()
            && unregisterShortcut(nativeKey, nativeMods))
    {
        shortcuts.remove(qMakePair(nativeKey, nativeMods));
        registered = false;
        return true;
    }
    return false;
}
void QxtGlobalShortcutPrivate::activateShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QxtGlobalShortcut* shortcut = shortcuts.value(qMakePair(nativeKey, nativeMods));
    if (shortcut && shortcut->isEnabled())
        emit shortcut->activated(shortcut);
}
QxtGlobalShortcut::QxtGlobalShortcut(QObject* parent)
        : QObject(parent)
{
    QXT_INIT_PRIVATE(QxtGlobalShortcut);
}
QxtGlobalShortcut::QxtGlobalShortcut(const QKeySequence& shortcut, QObject* parent)
        : QObject(parent)
{
    QXT_INIT_PRIVATE(QxtGlobalShortcut);
    setShortcut(shortcut);
}
QxtGlobalShortcut::~QxtGlobalShortcut()
{
}
QKeySequence QxtGlobalShortcut::shortcut() const
{
    return QKeySequence(qxt_d().key | qxt_d().mods);
}
bool QxtGlobalShortcut::setShortcut(const QKeySequence& shortcut)
{
    return qxt_d().setShortcut(shortcut);
}
bool QxtGlobalShortcut::isEnabled() const
{
    return qxt_d().enabled;
}
bool QxtGlobalShortcut::isValid() const
{
    return qxt_d().registered;
}
void QxtGlobalShortcut::setEnabled(bool enabled)
{
    qxt_d().enabled = enabled;
}
void QxtGlobalShortcut::setDisabled(bool disabled)
{
    qxt_d().enabled = !disabled;
}