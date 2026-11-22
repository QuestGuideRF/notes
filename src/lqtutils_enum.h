#pragma once
#include <QObject>
#include <QQmlEngine>
#define L_DECLARE_ENUM(enumName, ...)                                                                                                      \
    namespace enumName {                                                                                                                   \
    Q_NAMESPACE                                                                                                                            \
    enum Value : uint8_t { __VA_ARGS__ };                                                                                                            \
    Q_ENUM_NS(Value)                                                                                                                       \
    inline int qmlRegister##enumName(const char *uri, int major, int minor)                                                                \
    {                                                                                                                                      \
        return qmlRegisterUncreatableMetaObject(enumName::staticMetaObject, uri, major, minor, #enumName, "Доступ только к перечислениям и флагам"); \
    }                                                                                                                                      \
    inline int qRegisterMetaType()                                                                                                         \
    {                                                                                                                                      \
        return ::qRegisterMetaType<enumName::Value>(#enumName);                                                                            \
    }                                                                                                                                      \
    inline void registerEnum(const char *uri, int major, int minor)                                                                        \
    {                                                                                                                                      \
        enumName::qmlRegister##enumName(uri, major, minor);                                                                                \
        enumName::qRegisterMetaType();                                                                                                     \
    }                                                                                                                                      \
    }