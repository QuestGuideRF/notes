#include "framelesswindow.h"
#include <QApplication>
#include <QPoint>
#include <QSize>
#ifdef Q_OS_WIN
#  if defined(__MINGW32__) || defined(__GNUC__)
#  else
#    include <windows.h>
#    include <WinUser.h>
#    include <windowsx.h>
#    include <dwmapi.h>
#    include <objidl.h>
#    include <gdiplus.h>
#    include <gdipluscolor.h>
#    pragma comment(lib, "Dwmapi.lib")
#    pragma comment(lib, "user32.lib")
CFramelessWindow::CFramelessWindow(QWidget *parent) : QMainWindow(parent), m_titlebar(Q_NULLPTR), m_borderWidth(5), m_bJustMaximized(false), m_bResizeable(true)
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setResizeable(m_bResizeable);
}
void CFramelessWindow::setResizeable(bool resizeable)
{
    bool visible = isVisible();
    m_bResizeable = resizeable;
    if (m_bResizeable) {
        setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
        HWND hwnd = (HWND)this->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);
    } else {
        setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
        HWND hwnd = (HWND)this->winId();
        DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
        ::SetWindowLong(hwnd, GWL_STYLE, style & ~WS_MAXIMIZEBOX & ~WS_CAPTION);
    }
    const MARGINS shadow = { 1, 1, 1, 1 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
    setVisible(visible);
}
void CFramelessWindow::setResizeableAreaWidth(int width)
{
    if (1 > width)
        width = 1;
    m_borderWidth = width;
}
void CFramelessWindow::setTitleBar(QWidget *titlebar)
{
    m_titlebar = titlebar;
    if (!titlebar)
        return;
    connect(titlebar, SIGNAL(destroyed(QObject *)), this, SLOT(onTitleBarDestroyed()));
}
void CFramelessWindow::onTitleBarDestroyed()
{
    if (m_titlebar == QObject::sender()) {
        m_titlebar = Q_NULLPTR;
    }
}
void CFramelessWindow::addIgnoreWidget(QWidget *widget)
{
    if (!widget)
        return;
    if (m_whiteList.contains(widget))
        return;
    m_whiteList.append(widget);
}
bool CFramelessWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
    case WM_NCCALCSIZE: {
        NCCALCSIZE_PARAMS &params = *reinterpret_cast<NCCALCSIZE_PARAMS *>(msg->lParam);
        if (params.rgrc[0].top != 0)
            params.rgrc[0].top -= 1;
        *result = WVR_REDRAW;
        return true;
    }
    case WM_NCHITTEST: {
        *result = 0;
        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);
        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);
        if (m_bResizeable) {
            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();
            if (resizeWidth) {
                if (x >= winrect.left && x < winrect.left + border_width) {
                    *result = HTLEFT;
                }
                if (x < winrect.right && x >= winrect.right - border_width) {
                    *result = HTRIGHT;
                }
            }
            if (resizeHeight) {
                if (y < winrect.bottom && y >= winrect.bottom - border_width) {
                    *result = HTBOTTOM;
                }
                if (y >= winrect.top && y < winrect.top + border_width) {
                    *result = HTTOP;
                }
            }
            if (resizeWidth && resizeHeight) {
                if (x >= winrect.left && x < winrect.left + border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                    *result = HTBOTTOMLEFT;
                }
                if (x < winrect.right && x >= winrect.right - border_width && y < winrect.bottom && y >= winrect.bottom - border_width) {
                    *result = HTBOTTOMRIGHT;
                }
                if (x >= winrect.left && x < winrect.left + border_width && y >= winrect.top && y < winrect.top + border_width) {
                    *result = HTTOPLEFT;
                }
                if (x < winrect.right && x >= winrect.right - border_width && y >= winrect.top && y < winrect.top + border_width) {
                    *result = HTTOPRIGHT;
                }
            }
        }
        if (0 != *result)
            return true;
        /