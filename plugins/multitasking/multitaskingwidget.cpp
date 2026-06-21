// Copyright (C) 2019 ~ 2019 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "multitaskingwidget.h"
#include "multitaskingplugin.h"

#include <QPainter>
#include <QSvgRenderer>
#include <QMouseEvent>

MultitaskingWidget::MultitaskingWidget(QWidget *parent)
    : QWidget(parent)
{

}

void MultitaskingWidget::refreshIcon()
{
    update();
}

void MultitaskingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);

    const auto ratio = devicePixelRatioF();
    const int size = Dock::Fashion == qApp->property(PROP_DISPLAY_MODE).value<Dock::DisplayMode>()
        ? int(this->size().width() * 0.8) : int(this->size().width() * 0.7);

    QPixmap icon(size, size);
    icon.fill(Qt::transparent);
    icon.setDevicePixelRatio(ratio);

    QSvgRenderer renderer(QString(":/icons/resources/multitasking.svg"));
    QPainter painter(&icon);
    renderer.render(&painter);
    painter.end();

    QPainter p(this);
    const QRectF &rf = QRectF(rect());
    const QRectF &rfp = QRectF(icon.rect());
    p.drawPixmap(rf.center() - rfp.center() / ratio, icon);
}
