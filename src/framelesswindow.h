#ifndef CFRAMELESSWINDOW_H
#define CFRAMELESSWINDOW_H
#include "qsystemdetection.h"
#include <QObject>
#include <QMainWindow>
#ifdef Q_OS_WIN
#  if defined(__MINGW32__) || defined(__GNUC__)
#  else
#    include <QWidget>
#    include <QList>
#    include <QMargins>
#    include <QRect>
class CFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CFramelessWindow(QWidget *parent = nullptr);
public:
    void setResizeable(bool resizeable = true);
    bool isResizeable() { return m_bResizeable; }
    void setResizeableAreaWidth(int width = 5);
protected:
    void setTitleBar(QWidget *titlebar);
    void addIgnoreWidget(QWidget *widget);
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result);
private slots:
    void onTitleBarDestroyed();
public:
    void setContentsMargins(const QMargins &margins);
    void setContentsMargins(int left, int top, int right, int bottom);
    QMargins contentsMargins() const;
    QRect contentsRect() const;
    void getContentsMargins(int *left, int *top, int *right, int *bottom) const;
public slots:
    void showFullScreen();
private:
    QWidget *m_titlebar;
    QList<QWidget *> m_whiteList;
    int m_borderWidth;
    QMargins m_margins;
    QMargins m_frames;
    bool m_bJustMaximized;
    bool m_bResizeable;
};
#  endif
#elif defined Q_OS_MAC
#  include <QMouseEvent>
#  include <QResizeEvent>
#  include <QPoint>
class CFramelessWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit CFramelessWindow(QWidget *parent = nullptr);
private:
    void initUI();
public:
    void setDraggableAreaHeight(int height = 0);
    bool isNativeStyleOK() { return m_bNativeSystemBtn; }
    void setCloseBtnQuit(bool bQuit = true);
    void setCloseBtnEnabled(bool bEnable = true);
    void setMinBtnEnabled(bool bEnable = true);
    void setZoomBtnEnabled(bool bEnable = true);
    bool isCloseBtnEnabled() { return m_bIsCloseBtnEnabled; }
    bool isMinBtnEnabled() { return m_bIsMinBtnEnabled; }
    bool isZoomBtnEnabled() { return m_bIsZoomBtnEnabled; }
    void maximizeWindowMac();
    void setWindowAlwaysOnTopMac(bool isAlwaysOnTop);
    void setStandardWindowButtonsMacVisibility(bool isVisible);
signals:
    void toggleFullScreen(bool isFullScreen);
protected:
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
private:
    int m_draggableHeight;
    bool m_bWinMoving;
    bool m_bMousePressed;
    QPoint m_MousePos;
    QPoint m_WindowPos;
    bool m_bCloseBtnQuit;
    bool m_bNativeSystemBtn;
    bool m_bIsCloseBtnEnabled, m_bIsMinBtnEnabled, m_bIsZoomBtnEnabled;
    bool m_bInitialized;
private:
    bool m_bTitleBarVisible;
    void setTitlebarVisible(bool bTitlebarVisible = false);
    bool isTitlebarVisible() { return m_bTitleBarVisible; }
private slots:
    void onRestoreFromFullScreen();
signals:
    void restoreFromFullScreen();
protected:
    void resizeEvent(QResizeEvent *event);
};
#endif
#endif