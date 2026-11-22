#include "qxtglobalshortcut_p.h"
#include <QApplication>
#include <qpa/qplatformnativeinterface.h>
#include <xcb/xcb.h>
#include <QVector>
#include <X11/Xlib.h>
namespace {
QVector<quint32> maskModifiers()
{
    return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
}
typedef int (*X11ErrorHandler)(Display* display, XErrorEvent* event);
class QxtX11ErrorHandler {
public:
    static bool error;
    static int qxtX11ErrorHandler(Display* display, XErrorEvent *event)
    {
        Q_UNUSED(display);
        switch (event->error_code)
        {
            case BadAccess:
            case BadValue:
            case BadWindow:
                if (event->request_code == 33  ||
                        event->request_code == 34 )
                {
                    error = true;
                }
        }
        return 0;
    }
    QxtX11ErrorHandler()
        : m_previousErrorHandler(0)
    {
        error = false;
        m_previousErrorHandler = XSetErrorHandler(qxtX11ErrorHandler);
    }
    ~QxtX11ErrorHandler()
    {
        XSetErrorHandler(m_previousErrorHandler);
    }
private:
    X11ErrorHandler m_previousErrorHandler;
};
bool QxtX11ErrorHandler::error = false;
class QxtX11Data {
public:
    QxtX11Data()
        : m_display(0)
    {
        QPlatformNativeInterface *native = qApp->platformNativeInterface();
        void *display = native->nativeResourceForScreen(QByteArray("display"),
                                                        QGuiApplication::primaryScreen());
        m_display = reinterpret_cast<Display *>(display);
    }
    bool isValid()
    {
        return m_display != 0;
    }
    Display *display()
    {
        Q_ASSERT(isValid());
        return m_display;
    }
    Window rootWindow()
    {
        return DefaultRootWindow(display());
    }
    bool grabKey(quint32 keycode, quint32 modifiers, Window window)
    {
        QxtX11ErrorHandler errorHandler;
        foreach (quint32 maskMods, maskModifiers()) {
            XGrabKey(display(), keycode, modifiers | maskMods, window, True,
                     GrabModeAsync, GrabModeAsync);
            if (errorHandler.error)
                break;
        }
        if (errorHandler.error) {
            ungrabKey(keycode, modifiers, window);
            return false;
        }
        return true;
    }
    bool ungrabKey(quint32 keycode, quint32 modifiers, Window window)
    {
        QxtX11ErrorHandler errorHandler;
        foreach (quint32 maskMods, maskModifiers()) {
            XUngrabKey(display(), keycode, modifiers | maskMods, window);
        }
        return !errorHandler.error;
    }
private:
    Display *m_display;
};
}
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
bool QxtGlobalShortcutPrivate::nativeEventFilter(const QByteArray & eventType,
	void * message, qintptr * result)
#else
bool QxtGlobalShortcutPrivate::nativeEventFilter(const QByteArray & eventType,
	void * message, long * result)
#endif
{
    Q_UNUSED(result);
    xcb_key_press_event_t *kev = 0;
    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t* ev = static_cast<xcb_generic_event_t *>(message);
        if ((ev->response_type & 127) == XCB_KEY_PRESS)
            kev = static_cast<xcb_key_press_event_t *>(message);
    }
    if (kev != 0) {
        unsigned int keycode = kev->detail;
        unsigned int keystate = 0;
        if(kev->state & XCB_MOD_MASK_1)
            keystate |= Mod1Mask;
        if(kev->state & XCB_MOD_MASK_CONTROL)
            keystate |= ControlMask;
        if(kev->state & XCB_MOD_MASK_4)
            keystate |= Mod4Mask;
        if(kev->state & XCB_MOD_MASK_SHIFT)
            keystate |= ShiftMask;
        activateShortcut(keycode,
            keystate & (ShiftMask | ControlMask | Mod1Mask | Mod4Mask));
    }
    return false;
}
quint32 QxtGlobalShortcutPrivate::nativeModifiers(Qt::KeyboardModifiers modifiers)
{
    quint32 native = 0;
    if (modifiers & Qt::ShiftModifier)
        native |= ShiftMask;
    if (modifiers & Qt::ControlModifier)
        native |= ControlMask;
    if (modifiers & Qt::AltModifier)
        native |= Mod1Mask;
    if (modifiers & Qt::MetaModifier)
        native |= Mod4Mask;
    return native;
}
quint32 QxtGlobalShortcutPrivate::nativeKeycode(Qt::Key key)
{
    QxtX11Data x11;
    if (!x11.isValid())
        return 0;
    KeySym keysym = XStringToKeysym(QKeySequence(key).toString().toLatin1().data());
    if (keysym == NoSymbol)
        keysym = static_cast<ushort>(key);
    return XKeysymToKeycode(x11.display(), keysym);
}
bool QxtGlobalShortcutPrivate::registerShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QxtX11Data x11;
    return x11.isValid() && x11.grabKey(nativeKey, nativeMods, x11.rootWindow());
}
bool QxtGlobalShortcutPrivate::unregisterShortcut(quint32 nativeKey, quint32 nativeMods)
{
    QxtX11Data x11;
    return x11.isValid() && x11.ungrabKey(nativeKey, nativeMods, x11.rootWindow());
}