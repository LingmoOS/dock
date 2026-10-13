// Copyright (C) 2022 ~ 2022 Deepin Technology Co., Ltd.
// SPDX-FileCopyrightText: 2018 - 2023 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "dragwidget.h"
#include "utils.h"
#include "constants.h"

#include <QCoreApplication>
#include <QMouseEvent>

DragWidget::DragWidget(QWidget *parent)
    : QWidget(parent)
    , m_dragStatus(false)
{
    setObjectName("DragWidget");
}

bool DragWidget::isDraging() const
{
    return m_dragStatus;
}

void DragWidget::onTouchMove(double scaleX, double scaleY)
{
    Q_UNUSED(scaleX);
    Q_UNUSED(scaleY);

    static QPoint lastPos;
    QPoint curPos = QCursor::pos();
    if (lastPos == curPos) {
        return;
    }
    lastPos = curPos;
    qApp->postEvent(this, new QMouseEvent(QEvent::MouseMove, mapFromGlobal(curPos)
                                                  , QPoint(), curPos, Qt::LeftButton, Qt::LeftButton
                                          , Qt::NoModifier, Qt::MouseEventSynthesizedByApplication));
}

void DragWidget::mousePressEvent(QMouseEvent *event)
{
#if !DOCK_EDGE_DRAG_RESIZE
    // 鼠标拖拽缩放已禁用：任务栏尺寸统一由控制中心调整。
    // 这里只接受触摸路径主动投递的合成事件（MainWindowBase::touchRequestResizeDock），
    // 真实鼠标事件与 Qt 自行转发的触屏事件都不响应，并交还给父窗口处理。
    if (event->source() != Qt::MouseEventSynthesizedByApplication) {
        event->ignore();
        return;
    }
#else
    // qt转发的触屏按下信号不进行响应
    if (event->source() == Qt::MouseEventSynthesizedByQt)
        return;
#endif

    if (event->button() == Qt::LeftButton) {
        m_resizePoint = event->globalPos();
        m_dragStatus = true;
        this->grabMouse();
    }
}

void DragWidget::mouseMoveEvent(QMouseEvent *)
{
    if (m_dragStatus) {
        QPoint offset = QPoint(QCursor::pos() - m_resizePoint);
        emit dragPointOffset(offset);
    }
}

void DragWidget::mouseReleaseEvent(QMouseEvent *)
{
    if (!m_dragStatus)
        return;

    m_dragStatus =  false;
    releaseMouse();
    emit dragFinished();
}

void DragWidget::enterEvent(QEvent *)
{
    QApplication::setOverrideCursor(cursor());
}

void DragWidget::leaveEvent(QEvent *)
{
    QApplication::setOverrideCursor(Qt::ArrowCursor);
}

