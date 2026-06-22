// Copyright (C) 2011 ~ 2018 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "launcheritem.h"
#include "themeappicon.h"
#include "utils.h"
#include "../widgets/tipswidget.h"

#include <DDBusSender>

#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>
#include <QApplication>
#include <QGSettings>

DCORE_USE_NAMESPACE

LauncherItem::LauncherItem(QWidget *parent)
    : DockItem(parent)
    , m_gsettings(Utils::ModuleSettingsPtr("launcher", QByteArray(), this))
{
    if (m_gsettings) {
        connect(m_gsettings, &QGSettings::changed, this, &LauncherItem::onGSettingsChanged);
    }
}

void LauncherItem::refreshIcon()
{
    const int iconSize = qMin(width(), height());
    const int size = DockDisplayMode == Efficient ? iconSize * 0.7 : iconSize * 0.8;

    m_icon = QPixmap(size, size);
    m_icon.fill(Qt::transparent);

    QSvgRenderer renderer(QString(":/icons/resources/rocket.svg"));
    QPainter painter(&m_icon);
    renderer.render(&painter);
    painter.end();

    update();
}

void LauncherItem::paintEvent(QPaintEvent *e)
{
    DockItem::paintEvent(e);

    if (!isVisible())
        return;

    QPainter painter(this);

    const auto ratio = devicePixelRatioF();
    const int iconX = rect().center().x() - m_icon.rect().center().x() / ratio;
    const int iconY = rect().center().y() - m_icon.rect().center().y() / ratio;

    painter.drawPixmap(iconX, iconY, m_icon);
}

void LauncherItem::resizeEvent(QResizeEvent *e)
{
    DockItem::resizeEvent(e);

    refreshIcon();
}

void LauncherItem::mousePressEvent(QMouseEvent *e)
{
    if (checkGSettingsControl()) {
        return;
    }

    hidePopup();

    return QWidget::mousePressEvent(e);
}

void LauncherItem::mouseReleaseEvent(QMouseEvent *e)
{
    if (checkGSettingsControl()) {
        return;
    }

    if (e->button() != Qt::LeftButton)
        return;

    DDBusSender()
        .service("org.deepin.dde.Launcher1")
        .interface("org.deepin.dde.Launcher1")
        .path("/org/deepin/dde/Launcher1")
        .method(QString("Toggle"))
        .call();
}

QWidget *LauncherItem::popupTips()
{
    if (checkGSettingsControl()) {
        return nullptr;
    }

    m_tips.reset(new TipsWidget(this));
    m_tips->setVisible(false);
    m_tips->setText(tr("Launcher"));
    m_tips->setObjectName("launcher");
    return m_tips.get();
}

void LauncherItem::onGSettingsChanged(const QString& key) {
    if (key != "enable") {
        return;
    }

    if (m_gsettings && m_gsettings->keys().contains("enable")) {
        setVisible(m_gsettings->get("enable").toBool());
    }
}

bool LauncherItem::checkGSettingsControl() const
{
    return m_gsettings && m_gsettings->keys().contains("control")
            && m_gsettings->get("control").toBool();
}

void LauncherItem::showEvent(QShowEvent* event) {
    QTimer::singleShot(0, this, [=] {
        onGSettingsChanged("enable");
    });

    return DockItem::showEvent(event);
}
