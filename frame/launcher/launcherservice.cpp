#include "launcherservice.h"
#include "dbusutil.h"

#include <QDBusConnection>
#include <QProcess>
#include <QDebug>

LauncherService *LauncherService::instance()
{
    static LauncherService s_instance;
    return &s_instance;
}

bool LauncherService::registerService()
{
    auto bus = QDBusConnection::sessionBus();
    auto service = launcherService;
    auto path = launcherPath;

    if (!bus.registerService(service)) {
        qDebug() << "LauncherService: service already registered by another process, skip";
        return false;
    }

    if (!bus.registerObject(path, instance())) {
        qDebug() << "LauncherService: failed to register object";
        bus.unregisterService(service);
        return false;
    }

    qDebug() << "LauncherService: registered" << service;
    return true;
}

LauncherService::LauncherService(QObject *parent)
    : QObject(parent)
    , m_visible(false)
{
}

bool LauncherService::visible() const
{
    return m_visible;
}

void LauncherService::setVisible(bool visible)
{
    if (visible == m_visible)
        return;

    m_visible = visible;
    emit VisibleChanged(m_visible);

    if (m_visible) {
        emit Shown();
    } else {
        emit Closed();
    }
}

void LauncherService::Show()
{
    setVisible(true);
}

void LauncherService::Hide()
{
    setVisible(false);
}

void LauncherService::Toggle()
{
    setVisible(!m_visible);
    emit toggleRequested();
}

void LauncherService::Exit()
{
    setVisible(false);
}

void LauncherService::ShowByMode(qlonglong in0)
{
    Q_UNUSED(in0);
    Show();
}
