/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MDEVICEPROFILE_H
#define MDEVICEPROFILE_H

#include "mexport.h"
#include "mnamespace.h"
#include <QObject>

class QSize;
class MDeviceProfilePrivate;

/*! \brief The MDeviceProfile class holds physical and simulated properties of the target device

 */
class M_CORE_EXPORT MDeviceProfile : public QObject
{
    Q_OBJECT

public:
    //! Default constructor. Only used by MApplication class.
    //! Client classes should be using MDeviceProfile::instance() method.
    MDeviceProfile(QObject *parent = 0);

    //! Default destructor.
    virtual ~MDeviceProfile();

    //! Returns the global MDeviceProfile instance
    static MDeviceProfile *instance();

    //! Return the target device resolution
    QSize resolution() const;

    //! Return the target device pixels per inch
    QSize pixelsPerInch() const;

    //! Return whether the status bar should be shown on target device
    bool showStatusbar() const;

    //! Returns true if orientation angle is supported for given keyboard state.
    bool orientationAngleIsSupported(M::OrientationAngle angle, bool isKeyboardOpen) const;

    //! Returns the number of pixels required to display a length of \a mm millimeters on this device.
    int mmToPixels(qreal mm);

    //! Returns the number of pixels required to display a length of \a mm millimeters on this device using floating point precision.
    qreal mmToPixelsF(qreal mm);

protected:
    MDeviceProfilePrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MDeviceProfile)
    Q_DECLARE_PRIVATE(MDeviceProfile)
};

#endif
