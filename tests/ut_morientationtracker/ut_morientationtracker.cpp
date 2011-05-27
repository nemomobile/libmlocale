/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
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

#include "ut_morientationtracker.h"
#include "mservicelistener_stub.h"
#include "contextproperty_stub.h"
#include <mdeviceprofile.h>

#include <MComponentData>
#include <MWindow>
#include <MApplicationWindow>
#include <MOnDisplayChangeEvent>

#include <QWindowStateChangeEvent>

namespace {
    enum { KeyboardOpen = 0, KeyboardClosed };
    QList<M::OrientationAngle> supportedAnglesStubLists[2];
}

bool MDeviceProfile::orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const
{
    if (isKeyboardOpen) {
        return supportedAnglesStubLists[KeyboardOpen].contains(angle);
    } else {
        return supportedAnglesStubLists[KeyboardClosed].contains(angle);
    }
}

void Ut_MOrientationTracker::init()
{
    // Give at least one valid angle so that
    // MOrientationTracker::orientationAngle() can
    // work correctly.
    // MOrientationTracker::orientationAngle() is called
    // in MWindow construction
    supportedAnglesStubLists[KeyboardOpen].clear();
    supportedAnglesStubLists[KeyboardClosed].clear();
    supportedAnglesStubLists[KeyboardOpen] << M::Angle0;
    supportedAnglesStubLists[KeyboardClosed] << M::Angle0;

    window1 = new MWindow;
    window2 = new MWindow;
}

void Ut_MOrientationTracker::cleanup()
{
    delete window1;
    delete window2;
}

void Ut_MOrientationTracker::initTestCase()
{
    supportedAnglesStubLists[KeyboardOpen] << M::Angle0;
    supportedAnglesStubLists[KeyboardClosed] << M::Angle0;
    static int argc = 1;
    static char *argv[ 1 ];
    argv[ 0 ] = (char*)"./ut_mwindow";

    gContextPropertyStubMap->createStub("/maemo/InternalKeyboard/Present")->stubSetReturnValue("value", QVariant("true"));
    gContextPropertyStubMap->createStub("com.nokia.policy.video_route")->stubSetReturnValue("value", QVariant(""));
    gContextPropertyStubMap->createStub("Screen.IsCovered")->stubSetReturnValue("value", QVariant("false"));
    gContextPropertyStubMap->createStub("/Screen/Desktop/OrientationAngle")->stubSetReturnValue("value", QVariant(0));
    gContextPropertyStubMap->createStub("/Screen/CurrentWindow/OrientationAngle")->stubSetReturnValue("value", QVariant(0));

    m_componentData = new MComponentData(argc, argv);
    mTracker = MOrientationTracker::instance();
}

void Ut_MOrientationTracker::cleanupTestCase()
{
    delete m_componentData;
}

void Ut_MOrientationTracker::testOrientationPolicyWithoutConstraints_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<bool>("newKeyboardState");
    QTest::newRow("Angle0, keyboard open") << M::Angle0 << true;
    QTest::newRow("Angle90, keyboard open") << M::Angle90 << true;
    QTest::newRow("Angle180, keyboard open") << M::Angle180 << true;
    QTest::newRow("Angle270, keyboard open") << M::Angle270 << true;
    QTest::newRow("Angle0, keyboard closed") << M::Angle0 << false;
    QTest::newRow("Angle90, keyboard closed") << M::Angle90 << false;
    QTest::newRow("Angle180, keyboard closed") << M::Angle180 << false;
    QTest::newRow("Angle270, keyboard closed") << M::Angle270 << false;
}

void Ut_MOrientationTracker::testOrientationPolicyWithoutConstraints()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(bool, newKeyboardState);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    mTracker->doUpdateOrientationAngle(newAngle, newKeyboardState, false, false);

    QCOMPARE(window1->orientationAngle(), newAngle);
    QCOMPARE(window2->orientationAngle(), newAngle);
}

void Ut_MOrientationTracker::testOrientationPolicyWithTVConnectedConstraint_data()
{
    QTest::addColumn<M::OrientationAngle>("newAngle");
    QTest::addColumn<bool>("newKeyboardState");
    QTest::newRow("Angle0, keyboard open") << M::Angle0 << true;
    QTest::newRow("Angle90, keyboard open") << M::Angle90 << true;
    QTest::newRow("Angle180, keyboard open") << M::Angle180 << true;
    QTest::newRow("Angle270, keyboard open") << M::Angle270 << true;
    QTest::newRow("Angle0, keyboard closed") << M::Angle0 << false;
    QTest::newRow("Angle90, keyboard closed") << M::Angle90 << false;
    QTest::newRow("Angle180, keyboard closed") << M::Angle180 << false;
    QTest::newRow("Angle270, keyboard closed") << M::Angle270 << false;
}

void Ut_MOrientationTracker::testOrientationPolicyWithTVConnectedConstraint()
{
    QFETCH(M::OrientationAngle, newAngle);
    QFETCH(bool, newKeyboardState);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    mTracker->doUpdateOrientationAngle(newAngle, newKeyboardState, false, true);

    QCOMPARE(window1->orientationAngle(), M::Angle0);
    QCOMPARE(window2->orientationAngle(), M::Angle0);
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceFlatAndKeyboardJustClosed_data()
{
    QTest::addColumn<M::OrientationAngle>("firstAngle");
    QTest::addColumn<M::OrientationAngle>("secondAngle");

    QTest::newRow("Angle0 -> 90") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle90 -> 180") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle180 -> 270") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle270 -> 0") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceFlatAndKeyboardJustClosed()
{
    QFETCH(M::OrientationAngle, firstAngle);
    QFETCH(M::OrientationAngle, secondAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    //set in normal way
    mTracker->doUpdateOrientationAngle(firstAngle, true, false, false);

    //emulate state - keyboard just closed while device flat - no rotation
    mTracker->doUpdateOrientationAngle(secondAngle, false, true, false);

    QCOMPARE(window1->orientationAngle(), firstAngle);
    QCOMPARE(window2->orientationAngle(), firstAngle);
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceProfileConstraints_data()
{
    QTest::addColumn<M::OrientationAngle>("allowedAngle");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, allowed 0") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle180, allowed 90") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle270, allowed 180") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle0, allowed 270") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testOrientationPolicyDeviceProfileConstraints()
{
    QFETCH(M::OrientationAngle, allowedAngle);
    QFETCH(M::OrientationAngle, sensorAngle);

    supportedAnglesStubLists[KeyboardOpen].clear();
    supportedAnglesStubLists[KeyboardOpen] << allowedAngle;

    supportedAnglesStubLists[KeyboardClosed].clear();
    supportedAnglesStubLists[KeyboardClosed] << allowedAngle;

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientationAngle(), allowedAngle);
    QCOMPARE(window2->orientationAngle(), allowedAngle);
}

void Ut_MOrientationTracker::testWindowOrientationAngleLock_data()
{
    QTest::addColumn<M::OrientationAngle>("lockedAngle1");
    QTest::addColumn<M::OrientationAngle>("lockedAngle2");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, locked 0, 270") << M::Angle0 << M::Angle270 << M::Angle90;
    QTest::newRow("Angle180, locked 90, 0") << M::Angle90 << M::Angle0 << M::Angle180;
    QTest::newRow("Angle270, locked 180, 90") << M::Angle180 << M::Angle90 << M::Angle270;
    QTest::newRow("Angle0, locked 270, 180") << M::Angle270 << M::Angle180 << M::Angle0;
}

void Ut_MOrientationTracker::testWindowOrientationAngleLock()
{
    QFETCH(M::OrientationAngle, lockedAngle1);
    QFETCH(M::OrientationAngle, lockedAngle2);
    QFETCH(M::OrientationAngle, sensorAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    window1->setOrientationAngle(lockedAngle1);
    window1->lockOrientationAngle();

    window2->setOrientationAngle(lockedAngle2);
    window2->lockOrientationAngle();

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientationAngle(), lockedAngle1);
    QCOMPARE(window2->orientationAngle(), lockedAngle2);
}

void Ut_MOrientationTracker::testWindowOrientationLock_data()
{
    QTest::addColumn<M::OrientationAngle>("lockedAngle1");
    QTest::addColumn<M::OrientationAngle>("lockedAngle2");
    QTest::addColumn<M::OrientationAngle>("sensorAngle");

    QTest::newRow("Angle90, locked 0, 270") << M::Angle0 << M::Angle270 << M::Angle90;
    QTest::newRow("Angle180, locked 90, 0") << M::Angle90 << M::Angle0 << M::Angle180;
    QTest::newRow("Angle270, locked 180, 90") << M::Angle180 << M::Angle90 << M::Angle270;
    QTest::newRow("Angle0, locked 270, 180") << M::Angle270 << M::Angle180 << M::Angle0;
}

void Ut_MOrientationTracker::testWindowOrientationLock()
{
    QFETCH(M::OrientationAngle, lockedAngle1);
    QFETCH(M::OrientationAngle, lockedAngle2);
    QFETCH(M::OrientationAngle, sensorAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    window1->setOrientationAngle(lockedAngle1);
    window1->lockOrientation();
    M::Orientation orientation1 = (lockedAngle1 == M::Angle90 || lockedAngle1 == M::Angle270)?
                                  M::Portrait : M::Landscape;

    window2->setOrientationAngle(lockedAngle2);
    window2->lockOrientation();
    M::Orientation orientation2 = (lockedAngle2 == M::Angle90 || lockedAngle2 == M::Angle270)?
                                  M::Portrait : M::Landscape;

    mTracker->doUpdateOrientationAngle(sensorAngle, true, false, false);

    QCOMPARE(window1->orientation(), orientation1);
    QCOMPARE(window2->orientation(), orientation2);
}

void Ut_MOrientationTracker::testWindowRemoteOrientationLock_data()
{
    QTest::addColumn<MServiceListener::ServicePresence>("remoteTopEdgePropertyPresence");
    QTest::addColumn<QString>("sensorTopEdge");
    QTest::addColumn<QString>("remoteTopEdge");
    QTest::addColumn<QString>("keyboardOpen");
    QTest::addColumn<QString>("tvOutState");
    QTest::addColumn<M::OrientationAngle>("expectedAngle");

    QTest::newRow("NotPresent, right, bottom, false, <empty> -> Angle90")
            << MServiceListener::NotPresent << "right" << "bottom" << "false" << "" << M::Angle90;
    QTest::newRow("NotPresent, right, bottom, true, <empty> -> Angle0")
            << MServiceListener::NotPresent << "right" << "bottom" << "true" << "" << M::Angle0;
    QTest::newRow("NotPresent, right, bottom, false, tvout -> Angle0")
            << MServiceListener::NotPresent << "right" << "bottom" << "false" << "tvout" << M::Angle0;
    QTest::newRow("NotPresent, right, bottom, true, tvout -> Angle0")
            << MServiceListener::NotPresent << "right" << "bottom" << "true" << "tvout" << M::Angle0;
    QTest::newRow("Present, right, <empty>, false, <empty> -> Angle90")
            << MServiceListener::Present << "right" << "" << "false" << "" << M::Angle90;
    QTest::newRow("Present, right, <empty>, true, <empty> -> Angle0")
            << MServiceListener::Present << "right" << "" << "true" << "" << M::Angle0;
    QTest::newRow("Present, right, <empty>, false, tvout -> Angle0")
            << MServiceListener::Present << "right" << "" << "false" << "tvout" << M::Angle0;
    QTest::newRow("Present, right, <empty>, true, tvout -> Angle0")
            << MServiceListener::Present << "right" << "" << "true" << "tvout" << M::Angle0;
    QTest::newRow("Present, right, bottom, false, <empty> -> Angle180")
            << MServiceListener::Present << "right" << "bottom" << "false" << "" << M::Angle180;
    QTest::newRow("Present, right, bottom, true, <empty> -> Angle180")
            << MServiceListener::Present << "right" << "bottom" << "true" << "" << M::Angle180;
    QTest::newRow("Present, right, bottom, false, tvout -> Angle180")
            << MServiceListener::Present << "right" << "bottom" << "false" << "tvout" << M::Angle180;
    QTest::newRow("Present, right, bottom, true, tvout -> Angle180")
            << MServiceListener::Present << "right" << "bottom" << "true" << "tvout" << M::Angle180;
}

void Ut_MOrientationTracker::testWindowRemoteOrientationLock()
{
    QFETCH(MServiceListener::ServicePresence, remoteTopEdgePropertyPresence);
    QFETCH(QString, sensorTopEdge);
    QFETCH(QString, remoteTopEdge);
    QFETCH(QString, keyboardOpen);
    QFETCH(QString, tvOutState);
    QFETCH(M::OrientationAngle, expectedAngle);

    supportedAnglesStubLists[KeyboardOpen].clear();
    supportedAnglesStubLists[KeyboardOpen] <<  M::Angle0;

    supportedAnglesStubLists[KeyboardClosed].clear();
    supportedAnglesStubLists[KeyboardClosed] << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;

    QString dbusName = gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->getProxy()->info()->providerDBusName();

    gMServiceListenerStubMap->findStub(dbusName)->stubReset();
    gContextPropertyStubMap->findStub("Screen.TopEdge")->stubReset();
    gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->stubReset();
    gContextPropertyStubMap->findStub("/maemo/InternalKeyboard/Open")->stubReset();
    gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->stubReset();

    gMServiceListenerStubMap->findStub(dbusName)->stubSetReturnValue("isServicePresent", remoteTopEdgePropertyPresence);
    gContextPropertyStubMap->findStub("Screen.TopEdge")->stubSetReturnValue("value", QVariant(sensorTopEdge));
    gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->stubSetReturnValue("value", QVariant(remoteTopEdge));
    gContextPropertyStubMap->findStub("/maemo/InternalKeyboard/Open")->stubSetReturnValue("value", QVariant(keyboardOpen));
    gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->stubSetReturnValue("value", QVariant(tvOutState));

    window1->setVisible(true);
    window2->setVisible(true);
    MOnDisplayChangeEvent displayEvent1(true, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    MOnDisplayChangeEvent displayEvent2(true, QRectF(QPointF(0,0), window2->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent1);
    qApp->sendEvent(window2, &displayEvent2);

    // Use this signal so that also the video_route gets updated when eventually entering the updateOrientationAndle()
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->getProxy(), "valueChanged");

    QCoreApplication::processEvents();

    QCOMPARE(static_cast<int>(window1->orientationAngle()), static_cast<int>(expectedAngle));
    QCOMPARE(static_cast<int>(window2->orientationAngle()), static_cast<int>(expectedAngle));
}

void Ut_MOrientationTracker::testUpdatesPostponedUntilRotationsAreEnabled()
{
    supportedAnglesStubLists[KeyboardOpen].clear();
    supportedAnglesStubLists[KeyboardOpen] <<  M::Angle0;

    supportedAnglesStubLists[KeyboardClosed].clear();
    supportedAnglesStubLists[KeyboardClosed] << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;

    QString dbusName = gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->getProxy()->info()->providerDBusName();

    gMServiceListenerStubMap->findStub(dbusName)->stubReset();
    gContextPropertyStubMap->findStub("Screen.TopEdge")->stubReset();
    gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->stubReset();
    gContextPropertyStubMap->findStub("/maemo/InternalKeyboard/Open")->stubReset();
    gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->stubReset();

    gMServiceListenerStubMap->findStub(dbusName)->stubSetReturnValue("isServicePresent", MServiceListener::NotPresent);
    gContextPropertyStubMap->findStub("Screen.TopEdge")->stubSetReturnValue("value", QVariant(QString("top")));
    gContextPropertyStubMap->findStub("RemoteScreen.TopEdge")->stubSetReturnValue("value", QVariant(""));
    gContextPropertyStubMap->findStub("/maemo/InternalKeyboard/Open")->stubSetReturnValue("value", QVariant(false));
    gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->stubSetReturnValue("value", QVariant(""));

    // Use this signal so that also the video_route gets updated when eventually entering the updateOrientationAndle()
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("com.nokia.policy.video_route")->getProxy(), "valueChanged");

    window1->setVisible(true);
    MOnDisplayChangeEvent displayEvent(true, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent);

    // Cause MOrientationTrackerPrivate::updateOrienationAngle() slot to get called.
    // Will update the orientation of our window1
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("Screen.TopEdge")->getProxy(), "valueChanged");

    // Should follow Screen.TopEdge accordingly
    QCOMPARE(static_cast<int>(window1->orientationAngle()), static_cast<int>(M::Angle0));

    mTracker->disableRotations();

    gContextPropertyStubMap->findStub("Screen.TopEdge")->stubSetReturnValue("value", QVariant(QString("left")));

    // Cause MOrientationTrackerPrivate::updateOrienationAngle() slot to get called
    // Will update the orientation of our window1
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("Screen.TopEdge")->getProxy(), "valueChanged");

    // Don't follow Screen.TopEdge since rotations are disabled
    QCOMPARE(static_cast<int>(window1->orientationAngle()), static_cast<int>(M::Angle0));

    mTracker->enableRotations();

    // Now window1 orientation should have been updated properly to match current state of Screen.TopEdge
    QCOMPARE(static_cast<int>(window1->orientationAngle()), static_cast<int>(M::Angle270));
}

void Ut_MOrientationTracker::testFollowsCurrentAppWindowWhenDynamicPropertySet_data()
{
    QTest::addColumn<M::OrientationAngle>("firstAngle");
    QTest::addColumn<M::OrientationAngle>("secondAngle");

    QTest::newRow("Angle0 -> 90") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle90 -> 180") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle180 -> 270") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle270 -> 0") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testFollowsCurrentAppWindowWhenDynamicPropertySet()
{
    QFETCH(M::OrientationAngle, firstAngle);
    QFETCH(M::OrientationAngle, secondAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubReset();
    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubSetReturnValue("value", QVariant(firstAngle));

    window1->setProperty("followsCurrentApplicationWindowOrientation", true);

    QCOMPARE(window1->orientationAngle(), firstAngle);

    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubSetReturnValue("value", QVariant(secondAngle));
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->getProxy(), "valueChanged");

    QCOMPARE(window1->orientationAngle(), secondAngle);
}

void Ut_MOrientationTracker::testFollowsCurrentAppWindowWhenOnStackButNotTopmost_data()
{
    QTest::addColumn<M::OrientationAngle>("firstAngle");
    QTest::addColumn<M::OrientationAngle>("secondAngle");

    QTest::newRow("Angle0 -> 90") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle90 -> 180") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle180 -> 270") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle270 -> 0") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testFollowsCurrentAppWindowWhenOnStackButNotTopmost()
{
    QFETCH(M::OrientationAngle, firstAngle);
    QFETCH(M::OrientationAngle, secondAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubReset();
    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubSetReturnValue("value", QVariant(firstAngle));

    window1->setVisible(true);
    MOnDisplayChangeEvent displayEvent(false, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent);

    QCOMPARE(window1->orientationAngle(), firstAngle);

    gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->stubSetReturnValue("value", QVariant(secondAngle));
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle")->getProxy(), "valueChanged");

    QCOMPARE(window1->orientationAngle(), secondAngle);
}

/*
  A window that has the property "followsCurrentApplicationWindowOrientation" set to true
  will rotate freely if the context property "/Screen/CurrentWindow/OrientationAngle"
  is null or invalid.
 */
void Ut_MOrientationTracker::testRotatesFreelyIfCurrentAppWindowContextPorpertyNotSet()
{
    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    StubMap<QString, ContextProperty>::StubType *currentWindowOrientationPropStub =
            gContextPropertyStubMap->findStub("/Screen/CurrentWindow/OrientationAngle");
    StubMap<QString, ContextProperty>::StubType *topEdgePropStub =
            gContextPropertyStubMap->findStub("Screen.TopEdge");

    window1->setProperty("followsCurrentApplicationWindowOrientation", true);
    window1->setVisible(true);

    // Set a NULL variant to "/Screen/CurrentWindow/OrientationAngle"
    currentWindowOrientationPropStub->stubReset();
    currentWindowOrientationPropStub->stubSetReturnValue("value", QVariant(QVariant::Int));

    topEdgePropStub->stubReset();
    topEdgePropStub->stubSetReturnValue("value", QVariant(QString("top")));
    QMetaObject::invokeMethod(topEdgePropStub->getProxy(), "valueChanged");

    MOnDisplayChangeEvent displayEvent(true, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent);

    // Screen.TopEdge "top" == M::Angle0
    QCOMPARE(static_cast<int>(window1->orientationAngle()),
             static_cast<int>(M::Angle0));

    topEdgePropStub->stubSetReturnValue("value", QVariant(QString("left")));
    QMetaObject::invokeMethod(topEdgePropStub->getProxy(), "valueChanged");

    // Screen.TopEdge "left" == M::Angle270
    QCOMPARE(static_cast<int>(window1->orientationAngle()),
             static_cast<int>(M::Angle270));
}

void Ut_MOrientationTracker::testFollowingDesktopOrientation_data()
{
    QTest::addColumn<M::OrientationAngle>("firstAngle");
    QTest::addColumn<M::OrientationAngle>("secondAngle");

    QTest::newRow("Angle0 -> 90") << M::Angle0 << M::Angle90;
    QTest::newRow("Angle90 -> 180") << M::Angle90 << M::Angle180;
    QTest::newRow("Angle180 -> 270") << M::Angle180 << M::Angle270;
    QTest::newRow("Angle270 -> 0") << M::Angle270 << M::Angle0;
}

void Ut_MOrientationTracker::testFollowingDesktopOrientation()
{
    QFETCH(M::OrientationAngle, firstAngle);
    QFETCH(M::OrientationAngle, secondAngle);

    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->stubReset();

    gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->stubSetReturnValue("value", QVariant(firstAngle));

    //convince the window that it is in switcher
    window1->setWindowState(Qt::WindowMinimized);
    MOnDisplayChangeEvent displayChangeEvent(true, QRectF(QPointF(0,0),window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayChangeEvent);

    //test if window is rotated after entering switcher
    QCOMPARE(window1->orientationAngle(), firstAngle);

    //simulate desktop rotation
    gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->stubSetReturnValue("value", QVariant(secondAngle));
    QMetaObject::invokeMethod(gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->getProxy(), "valueChanged");

    //test if desktop rotation was handled properly
    QCOMPARE(window1->orientationAngle(), secondAngle);
}

void Ut_MOrientationTracker::testFollowingDesktopOrientationWhenPropertyIsNotPresent()
{
    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->stubReset();

    gContextPropertyStubMap->findStub("/Screen/Desktop/OrientationAngle")->stubSetReturnValue("value", QVariant());

    window1->setLandscapeOrientation();

    //convince the window that it is in switcher
    window1->setWindowState(Qt::WindowMinimized);
    MOnDisplayChangeEvent displayChangeEvent(true, QRectF(QPointF(0,0),window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayChangeEvent);

    //if property is not present (value().isNull()) defaults to portrait
    QCOMPARE(window1->orientation(), M::Portrait);
}


void Ut_MOrientationTracker::testSensorPropertiesSubscription()
{
    setAllAngles(&supportedAnglesStubLists[KeyboardOpen]);
    setAllAngles(&supportedAnglesStubLists[KeyboardClosed]);

    QVERIFY(!mTracker->isSubscribedToSensorProperties());

    M::Orientation angle = window1->orientation();
    Q_UNUSED(angle);

    QVERIFY(!mTracker->isSubscribedToSensorProperties());

    window1->setVisible(true);
    MOnDisplayChangeEvent displayEvent1(true, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent1);

    QVERIFY(mTracker->isSubscribedToSensorProperties());

    window1->setVisible(false);
    MOnDisplayChangeEvent displayEvent2(false, QRectF(QPointF(0,0), window1->visibleSceneSize()));
    qApp->sendEvent(window1, &displayEvent2);

    QVERIFY(!mTracker->isSubscribedToSensorProperties());
}

///////////////////////////////////////////////////////
//////////////////HELPER FUNCTIONS/////////////////////
///////////////////////////////////////////////////////

void Ut_MOrientationTracker::setAllAngles(QList<M::OrientationAngle> *list)
{
    *list << M::Angle0 << M::Angle90 << M::Angle180 << M::Angle270;
}

QTEST_MAIN(Ut_MOrientationTracker);

