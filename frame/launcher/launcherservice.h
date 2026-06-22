#ifndef LAUNCHERSERVICE_H
#define LAUNCHERSERVICE_H

#include <QObject>

class LauncherService : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.deepin.dde.Launcher1")
    Q_PROPERTY(bool Visible READ visible)

public:
    explicit LauncherService(QObject *parent = nullptr);

    static LauncherService *instance();
    static bool registerService();

    bool visible() const;
    void setVisible(bool visible);

public Q_SLOTS:
    void Show();
    void Hide();
    void Toggle();
    void Exit();
    void ShowByMode(qlonglong in0);

Q_SIGNALS:
    void Closed();
    void Shown();
    void VisibleChanged(bool visible);
    void toggleRequested();

private:
    bool m_visible;
};

#endif
