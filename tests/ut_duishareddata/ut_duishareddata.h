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

#ifndef UT_DUISHAREDDATA_H
#define UT_DUISHAREDDATA_H

#include <QtTest/QtTest>
#include <QObject>

#include <corelib/core/duishareddata.h>

Q_DECLARE_METATYPE(DuiSharedData *);

class Ut_DuiSharedData : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void testOpenClose();
    void testReadWrite_1();
    void testReadWrite_1000();
    void testReadWrite_2000();
    void testReadWrite_5000();
    void testReadWrite_10000();
    void testReadWrite_20000();

private:
    void testReadWrite_N(DuiSharedData *shm, QStringList &data, QList<qint64> &offsets);
    void testReadWrite_N_verify(DuiSharedData *shm, QStringList &data, QList<qint64> &offsets);

};

#endif

