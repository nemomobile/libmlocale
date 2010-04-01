/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "ut_duipannableviewport.h"
#include <duitheme.h>
#include <duipositionindicator.h>
#include <duipannableviewport.h>
#include "../../src/corelib/widgets/duipannableviewport_p.h"
#include <QGraphicsWidget>
#include <QGraphicsLinearLayout>
#include <QTest>
#include <duiapplication.h>
#include <duiwidgetview.h>

DuiApplication *app;

void Ut_DuiPannableViewport::initTestCase()
{
    static int argc = 1;
    static char *app_name[1] = { (char *) "./ut_duipannableviewport" };
    app = new DuiApplication(argc, app_name);
}


void Ut_DuiPannableViewport::cleanupTestCase()
{
    delete app;
}


void Ut_DuiPannableViewport::init()
{
    subject = new DuiPannableViewport();
}

void Ut_DuiPannableViewport::cleanup()
{
    delete subject;
    subject = 0;
}

void Ut_DuiPannableViewport::constructor()
{
    QCOMPARE(subject->flags(), QGraphicsItem::ItemClipsChildrenToShape);
}

void Ut_DuiPannableViewport::setWidget()
{
    QGraphicsWidget *widget = new QGraphicsWidget();
    subject->setWidget(widget);

    widget->setZValue(1);
    QCOMPARE(widget->zValue(), 1.0);

    QCOMPARE(static_cast<DuiPannableViewportPrivate *>(subject->d_ptr)->pannedWidget, widget);
    QCOMPARE(static_cast<DuiPannableViewportPrivate *>(subject->d_ptr)->pannedWidget->parentItem(), subject);

    /* There is a bug in duipannableviewport that causes this testcase to crash
     * randomly, deleting widget and setting it to 0 seems to make this
     * testcase to work but it might hide the actual bug from test! So, this
     * code is here just for a reference - Jani Mikkonen
     *
     * delete widget;
     * widget = 0;
     * */
}

void Ut_DuiPannableViewport::setGeometry_data()
{
    QTest::addColumn< QSizeF >("viewportSize");
    QTest::addColumn< QSizeF >("pannedSize");
    QTest::addColumn< QSizeF >("physicsRange");

    QTest::newRow("set 1") << QSizeF(100, 50) << QSizeF(400, 200) << QSizeF(300, 150);
    QTest::newRow("set 2") << QSizeF(400, 50) << QSizeF(100, 200) << QSizeF(0, 150);
    QTest::newRow("set 3") << QSizeF(100, 200) << QSizeF(400, 50) << QSizeF(300, 0);
}

void Ut_DuiPannableViewport::setGeometry()
{
    QFETCH(QSizeF, viewportSize);
    QFETCH(QSizeF, pannedSize);
    QFETCH(QSizeF, physicsRange);

    QGraphicsWidget *widget = new QGraphicsWidget();
    widget->setMinimumSize(pannedSize);
    widget->setMaximumSize(pannedSize);

    subject->setWidget(widget);
    subject->setGeometry(QRectF(QPointF(), viewportSize));

    QCOMPARE(subject->physics()->range().size(), physicsRange);

    /* There is a bug in duipannableviewport that causes this testcase to crash
     * randomly, deleting widget and setting it to 0 seems to make this
     * testcase to work but it might hide the actual bug from test! So, this
     * code is here just for a reference - Jani Mikkonen
     *
     * delete widget;
     * widget = 0;
     * */
}

void Ut_DuiPannableViewport::event_data()
{
    QTest::addColumn< QSizeF >("viewportSize");
    QTest::addColumn< QSizeF >("pannedSize");
    QTest::addColumn< QSizeF >("physicsRange");

    QTest::newRow("set 1") << QSizeF(100, 50) << QSizeF(400, 200) << QSizeF(300, 150);
    QTest::newRow("set 2") << QSizeF(400, 50) << QSizeF(100, 200) << QSizeF(0, 150);
    QTest::newRow("set 3") << QSizeF(100, 200) << QSizeF(400, 50) << QSizeF(300, 0);
}

void Ut_DuiPannableViewport::event()
{
    QEvent *event = new QEvent(QEvent::LayoutRequest);

    QFETCH(QSizeF, viewportSize);
    QFETCH(QSizeF, pannedSize);
    QFETCH(QSizeF, physicsRange);

    QGraphicsWidget *widget = new QGraphicsWidget();
    // Forcing the size of panned widget
    widget->setMinimumSize(pannedSize);
    widget->setPreferredSize(pannedSize);
    widget->setMaximumSize(pannedSize);

    // Forcing the size of subject
    subject->setMinimumSize(viewportSize);
    subject->setPreferredSize(viewportSize);
    subject->setMaximumSize(viewportSize);

    subject->setWidget(widget);

    subject->event(event);

    QCOMPARE(subject->physics()->range().size(), physicsRange);
}

void Ut_DuiPannableViewport::updatePosition()
{

    QGraphicsWidget *widget = new QGraphicsWidget();
    subject->setWidget(widget);

    // Forcing the size of subject to some value
    subject->setMinimumSize(QSizeF(500, 300));
    subject->setMaximumSize(QSizeF(500, 300));

    QSignalSpy spy(subject, SIGNAL(positionChanged(QPointF)));

    subject->updatePosition(QPointF(-50, 75));

    QCOMPARE(static_cast<DuiPannableViewportPrivate *>(subject->d_ptr)->pannedWidget->pos(), -QPointF(-50, 75));

    QCOMPARE(spy.count(), 1);

    /* There is a bug in duipannableviewport that causes this testcase to crash
     * randomly, deleting widget and setting it to 0 seems to make this
     * testcase to work but it might hide the actual bug from test! So, this
     * code is here just for a reference - Jani Mikkonen
     *
     * delete widget;
     * widget = 0;
     * */
}

void Ut_DuiPannableViewport::updateSamePosition()
{
    QGraphicsWidget *mainWidget = new QGraphicsWidget();
    QPointF point(0.0, 0.0);

    subject->setMinimumSize(100, 480);
    subject->setPreferredSize(100, 480);
    subject->setMaximumSize(100, 480);

    mainWidget->setMinimumSize(100, 1000);
    mainWidget->setPreferredSize(100, 1000);
    mainWidget->setMaximumSize(100, 1000);

    subject->setWidget(mainWidget);

    subject->updatePosition(point);

    QSignalSpy spy(subject,
                   SIGNAL(positionChanged(QPointF)));

    subject->updatePosition(point);

    // Should not have emitted anything since nothing changed
    QCOMPARE(spy.count(), 0);
}

/*
 * While the pannedWidget is populated, sizePosChanged() signal should be
 * emitted only once for each actual change in pannedWidget's size.
 *
 * See NB#143428
 */
void Ut_DuiPannableViewport::sizePosChangedAfterPopulatingPannedWidget()
{
    QGraphicsWidget *mainWidget = new QGraphicsWidget();
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    QGraphicsWidget *childWidget;

    //Settle initial layout position
    subject->adjustSize();

    mainWidget->setLayout(layout);

    subject->setWidget(mainWidget);

    QSignalSpy spyRange(subject,
                   SIGNAL(rangeChanged(QRectF)));
    QSignalSpy spyViewportSize(subject,
                   SIGNAL(viewportSizeChanged(QSizeF)));
    QSignalSpy spyPosition(subject,
                   SIGNAL(positionChanged(QPointF)));


    for (int i = 0; i < 30; i++) {
        childWidget = new QGraphicsWidget;

        childWidget->setMinimumSize(100, 200);
        childWidget->setPreferredSize(100, 200);

        layout->addItem(childWidget);
    }

    // Force layout to work.
    subject->adjustSize();

    // Check consecutive signals (if any), are different from each other.
    // We don't want to send out the very same event twice.
    for (int i = 1; i < spyRange.size(); i++) {
        QVERIFY(spyRange.at(i) != spyRange.at(i - 1));
    }
    for (int i = 1; i < spyPosition.size(); i++) {
        QVERIFY(spyPosition.at(i) != spyPosition.at(i - 1));
    }
    for (int i = 1; i < spyViewportSize.size(); i++) {
        QVERIFY(spyViewportSize.at(i) != spyViewportSize.at(i - 1));
    }
}

void Ut_DuiPannableViewport::settingNewPositionIndicator()
{
    DuiPositionIndicator* newPositionIndicator = new DuiPositionIndicator();
    subject->setPositionIndicator(newPositionIndicator);

    QVERIFY(subject->positionIndicator() == newPositionIndicator);
}

void Ut_DuiPannableViewport::settingNULLPositionIndicatorShouldNotBeAccepted()
{
    subject->setPositionIndicator(NULL);

    QVERIFY(subject->positionIndicator() != NULL);
}

QTEST_APPLESS_MAIN(Ut_DuiPannableViewport)
