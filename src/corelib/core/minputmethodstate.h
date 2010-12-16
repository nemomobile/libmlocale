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

#ifndef MINPUTMETHODSTATE_H
#define MINPUTMETHODSTATE_H

#include <QObject>
#include <QRect>

#include "mnamespace.h"
#include "mexport.h"

class QKeyEvent;

class MInputMethodStatePrivate;

/*!
 * \brief A mediator between MInputContext and applications/libmeegotouch that use it
 *
 * This class allows MInputContext (technically other input contexts too) to
 * communicate input method area on display to the application and application
 * to communicate its active window's orientation and custom toolbar to the input context.
 * The application can be a M application or a plain Qt application (which needs
 * to link against libmeegotouch to get access to this class).  The input method area
 * can be used by the application to avoid obstructing the input method.
 */
class M_CORE_EXPORT MInputMethodState : public QObject
{
    Q_OBJECT

public:
    //! \brief Get singleton instance
    //! \return singleton instance
    static MInputMethodState *instance();

    //! \brief Get current input method area
    //! \return current input method area
    QRect inputMethodArea() const;

    //! \brief Get the orientation of application's active window
    //! \return orientation of application's active window
    M::OrientationAngle activeWindowOrientationAngle() const;

    /*!
     * Sends a request to the application's input context to open a software
     * input panel (e.g. the virtual keyboard).
     * Warning: In order to gain more screen space, window decorations may be
     * temporarily hidden while the input panel is up.
     * \sa closeSoftwareInputPanel
     */
    static void requestSoftwareInputPanel();

    /*!
     * Sends a request to the application's input context to close a software
     * input panel.
     * \sa requestSoftwareInputPanel
     */
    static void closeSoftwareInputPanel();

    //! \internal
    void emitKeyPress(const QKeyEvent &event);
    void emitKeyRelease(const QKeyEvent &event);

    // returns the identifiers of registered toolbars
    QList<int> toolbarIds() const;

    // providing item name -> (attribute name -> attribute value)
    typedef QMap<QString, QVariant> AttributeMap;
    typedef QMap<QString, AttributeMap> ItemAttributeMap;

    // returns state set for a toolbar
    ItemAttributeMap toolbarState(int id) const;
    //! \internal_end

public Q_SLOTS:
    /*!
     * \brief Application's active window start to rotate to \a newOrientationAngle.
     *
     * \note This method should be called when active window start rotation animation.
     */
    void startActiveWindowOrientationAngleChange(M::OrientationAngle newOrientationAngle);

    /*!
     * \brief Set the orientation angle of application's active window to \a newOrientationAngle
     *
     * \note This method should be called when active window already finished the rotation animation.
     */
    void setActiveWindowOrientationAngle(M::OrientationAngle newOrientationAngle);

    //! \brief Set input method area to \a newRegion
    void setInputMethodArea(const QRect &newRegion);

    /*!
     * \brief Register an input method toolbar which is defined in \a fileName.
     *  \param \a fileName is the absolute file name of a custom toolbar, or the relative file under
     *      /usr/share/meegoimframework/imtoolbars/ (relative path deprecated).
     *  \return the registered unique identifier of this toolbar.
     */
    int registerToolbar(const QString &fileName);

    /*!
     * \brief Unregister an input method toolbar which unique id \a id.
     *  \param \a id is the unique identifier of the registered toolbar.
     */
    void unregisterToolbar(int id);

    //! \brief Set the \a attribute of the \a item in the input method toolbar which has the unique \a id to \a value.
    void setToolbarItemAttribute(int id, const QString &item,
                                 const QString &attribute, const QVariant &value);

    /*!
     * \brief Returns the input method toolbar file name which is registered with the unique identifier \id.
     *  \param id the unique identifier of a registered toolbar.
     *  \return the file name of toolbar, or empty QString if \a id is invalid.
     */
    QString toolbar(int id) const;

Q_SIGNALS:
    //! Emitted when input method area is changed
    //! \param region new input method area
    void inputMethodAreaChanged(const QRect &region);

    /*!
     * \brief This signal is emitted before the orientation angle of the application's active window is changed.
     * \param orientationAngle New orientation angle which is about to change.
     *
     * \note It is emitted at the start of the rotation animation.
     */
    void activeWindowOrientationAngleAboutToChange(M::OrientationAngle orientationAngle);

    /*!
     * \brief This signal is emitted when the orientation angle of application's active window is changed
     * \param orientationAngle new orientation angle
     *
     * \note It's emitted with every orientation angle change finished.
     */
    void activeWindowOrientationAngleChanged(M::OrientationAngle orientationAngle);

    //! Emitted when an input method toolbar which is defined in \a fileName with an unique identifier \a id is registered.
    void toolbarRegistered(int id, const QString &fileName);

    //! Emitted when an input method toolbar with an unique \a id is unregistered.
    void toolbarUnregistered(int id);

    //! Emitted when input method toolbar item attribute is changed.
    void toolbarItemAttributeChanged(int id, const QString &item, const QString &attribute,
                                     const QVariant &value);

    //! Can be emitted by input method to notify about raw key press event it receives.
    void keyPress(const QKeyEvent &);

    //! Can be emitted by input method to notify about raw key release event it receives.
    void keyRelease(const QKeyEvent &);

private:
    //! Disable default construction
    MInputMethodState();
    ~MInputMethodState();
    Q_DISABLE_COPY(MInputMethodState)

    MInputMethodStatePrivate *const d_ptr;

    Q_DECLARE_PRIVATE(MInputMethodState)
};

#endif
