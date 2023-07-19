/*
 * SPDX-FileCopyrightText: 2021 Arjen Hiemstra <ahiemstra@heimr.nl>
 *
 * SPDX-License-Identifier: LGPL-2.0-or-later
 */

#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <memory>

#include <QObject>

/**
 * This exposes information about the current used input method.
 */
class InputMethod : public QObject
{
    Q_OBJECT

public:
    InputMethod(QObject *parent = nullptr);
    ~InputMethod() override;

    /**
     * Is an input method available?
     *
     * This will be true if there is an input method available. When it is
     * false it means there's no special input method configured and input
     * happens directly through keyboard events.
     */
    Q_PROPERTY(bool available READ available NOTIFY availableChanged)
    bool available() const;
    Q_SIGNAL void availableChanged();

    /**
     * Is the current input method enabled?
     *
     * If this is false, that means the input method is available but not in use.
     */
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    bool enabled() const;
    void setEnabled(bool newEnabled);
    Q_SIGNAL void enabledChanged();

    /**
     * Is the current input method active?
     *
     * What active means depends on the type of input method. In case of a
     * virtual keyboard for example, it would mean the virtual keyboard is
     * visible.
     */
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    bool active() const;
    void setActive(bool newActive);
    Q_SIGNAL void activeChanged();

    /**
     * Is the current input method visible?
     *
     * For some input methods this will match \ref active however for others this
     * may differ.
     */
    Q_PROPERTY(bool visible READ visible NOTIFY visibleChanged)
    bool visible() const;
    Q_SIGNAL void visibleChanged();

    /**
     * Will the input method be shown when a text input field gains focus?
     *
     * This is intended to be used to decide whether to give an input field
     * default focus. For certain input methods, like virtual keyboards, it may
     * not be desirable to show it by default. For example, having a search
     * field focused on application startup may cause the virtual keyboard to
     * show, greatly reducing the available application space.
     */
    Q_PROPERTY(bool willShowOnActive READ willShowOnActive NOTIFY willShowOnActiveChanged)
    bool willShowOnActive() const;
    Q_SIGNAL void willShowOnActiveChanged();

private:
    class Private;
    const std::unique_ptr<Private> d;
};

#endif // INPUTMETHOD_H
