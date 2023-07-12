#include <QtQml/qqmlprivate.h>
#include <QtCore/qdir.h>
#include <QtCore/qurl.h>

static const unsigned char qt_resource_tree[] = {
0,
0,0,0,0,2,0,0,0,8,0,0,0,1,0,0,0,
148,0,0,0,0,0,1,0,0,0,0,0,0,0,30,0,
0,0,0,0,1,0,0,0,0,0,0,0,78,0,0,0,
0,0,1,0,0,0,0,0,0,0,246,0,0,0,0,0,
1,0,0,0,0,0,0,0,8,0,2,0,0,0,1,0,
0,0,9,0,0,0,180,0,0,0,0,0,1,0,0,0,
0,0,0,0,116,0,0,0,0,0,1,0,0,0,0,0,
0,0,212,0,0,0,0,0,1,0,0,0,0,0,0,0,
30,0,0,0,0,0,1,0,0,0,0};
static const unsigned char qt_resource_names[] = {
0,
1,0,0,0,47,0,47,0,8,8,75,147,212,0,43,0,
97,0,110,0,100,0,114,0,111,0,105,0,100,0,21,3,
241,144,92,0,76,0,111,0,97,0,100,0,80,0,114,0,
111,0,103,0,114,0,101,0,115,0,115,0,83,0,116,0,
121,0,108,0,101,0,46,0,113,0,109,0,108,0,16,6,
151,249,124,0,82,0,101,0,103,0,105,0,115,0,116,0,
101,0,114,0,80,0,97,0,103,0,101,0,46,0,113,0,
109,0,108,0,13,12,101,153,252,0,77,0,97,0,112,0,
87,0,105,0,110,0,100,0,111,0,119,0,46,0,113,0,
109,0,108,0,13,1,108,83,188,0,76,0,111,0,103,0,
105,0,110,0,80,0,97,0,103,0,101,0,46,0,113,0,
109,0,108,0,13,10,113,11,252,0,77,0,115,0,103,0,
68,0,105,0,97,0,108,0,111,0,103,0,46,0,113,0,
109,0,108,0,14,14,58,214,51,0,113,0,119,0,101,0,
98,0,99,0,104,0,97,0,110,0,110,0,101,0,108,0,
46,0,106,0,115,0,8,8,1,90,92,0,109,0,97,0,
105,0,110,0,46,0,113,0,109,0,108};
static const unsigned char qt_resource_empty_payout[] = { 0, 0, 0, 0, 0 };
QT_BEGIN_NAMESPACE
extern Q_CORE_EXPORT bool qRegisterResourceData(int, const unsigned char *, const unsigned char *, const unsigned char *);
QT_END_NAMESPACE
namespace QmlCacheGeneratedCode {
namespace _0x5f__main_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__qwebchannel_js { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__MsgDialog_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__LoadProgressStyle_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__LoginPage_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__MapWindow_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace _0x5f__RegisterPage_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}
namespace __0x2b_android_LoadProgressStyle_qml { 
    extern const unsigned char qmlData[];
    const QQmlPrivate::CachedQmlUnit unit = {
        reinterpret_cast<const QV4::CompiledData::Unit*>(&qmlData), nullptr, nullptr
    };
}

}
namespace {
struct Registry {
    Registry();
    ~Registry();
    QHash<QString, const QQmlPrivate::CachedQmlUnit*> resourcePathToCachedUnit;
    static const QQmlPrivate::CachedQmlUnit *lookupCachedUnit(const QUrl &url);
};

Q_GLOBAL_STATIC(Registry, unitRegistry)


Registry::Registry() {
        resourcePathToCachedUnit.insert(QStringLiteral("/main.qml"), &QmlCacheGeneratedCode::_0x5f__main_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/qwebchannel.js"), &QmlCacheGeneratedCode::_0x5f__qwebchannel_js::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MsgDialog.qml"), &QmlCacheGeneratedCode::_0x5f__MsgDialog_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/LoadProgressStyle.qml"), &QmlCacheGeneratedCode::_0x5f__LoadProgressStyle_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/LoginPage.qml"), &QmlCacheGeneratedCode::_0x5f__LoginPage_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/MapWindow.qml"), &QmlCacheGeneratedCode::_0x5f__MapWindow_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/RegisterPage.qml"), &QmlCacheGeneratedCode::_0x5f__RegisterPage_qml::unit);
        resourcePathToCachedUnit.insert(QStringLiteral("/+android/LoadProgressStyle.qml"), &QmlCacheGeneratedCode::__0x2b_android_LoadProgressStyle_qml::unit);
    QQmlPrivate::RegisterQmlUnitCacheHook registration;
    registration.version = 0;
    registration.lookupCachedQmlUnit = &lookupCachedUnit;
    QQmlPrivate::qmlregister(QQmlPrivate::QmlUnitCacheHookRegistration, &registration);
QT_PREPEND_NAMESPACE(qRegisterResourceData)(/*version*/0x01, qt_resource_tree, qt_resource_names, qt_resource_empty_payout);
}

Registry::~Registry() {
    QQmlPrivate::qmlunregister(QQmlPrivate::QmlUnitCacheHookRegistration, quintptr(&lookupCachedUnit));
}

const QQmlPrivate::CachedQmlUnit *Registry::lookupCachedUnit(const QUrl &url) {
    if (url.scheme() != QLatin1String("qrc"))
        return nullptr;
    QString resourcePath = QDir::cleanPath(url.path());
    if (resourcePath.isEmpty())
        return nullptr;
    if (!resourcePath.startsWith(QLatin1Char('/')))
        resourcePath.prepend(QLatin1Char('/'));
    return unitRegistry()->resourcePathToCachedUnit.value(resourcePath, nullptr);
}
}
int QT_MANGLE_NAMESPACE(qInitResources_qrc)() {
    ::unitRegistry();
    Q_INIT_RESOURCE(qrc_qmlcache);
    return 1;
}
Q_CONSTRUCTOR_FUNCTION(QT_MANGLE_NAMESPACE(qInitResources_qrc))
int QT_MANGLE_NAMESPACE(qCleanupResources_qrc)() {
    Q_CLEANUP_RESOURCE(qrc_qmlcache);
    return 1;
}
