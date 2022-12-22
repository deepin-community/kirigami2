/*
 *  SPDX-FileCopyrightText: 2018 Aleix Pol Gonzalez <aleixpol@blue-systems.com>
 *
 *  SPDX-License-Identifier: LGPL-2.0-or-later
 */

import QtQuick 2.1
import QtQuick.Controls 2.4 as QQC2
import QtQuick.Window 2.15
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.19

/**
 * An about page that is ready to integrate in a kirigami app.
 *
 * Allows to have a page that will show the copyright notice of the application
 * together with the contributors and some information of which platform it's
 * running on.
 *
 * @since 5.52
 * @since org.kde.kirigami 2.6
 */
ScrollablePage
{
    id: page
    /**
     * This property holds an object with the same shape as KAboutData.
     *
     * For example:
     * @code{json}
     * aboutData: {
          "displayName" : "KirigamiApp",
          "productName" : "kirigami/app",
          "componentName" : "kirigamiapp",
          "shortDescription" : "A Kirigami example",
          "homepage" : "",
          "bugAddress" : "submit@bugs.kde.org",
          "version" : "5.14.80",
          "otherText" : "",
          "authors" : [
              {
                  "name" : "...",
                  "task" : "",
                  "emailAddress" : "somebody@kde.org",
                  "webAddress" : "",
                  "ocsUsername" : ""
              }
          ],
          "credits" : [],
          "translators" : [],
          "licenses" : [
              {
                  "name" : "GPL v2",
                  "text" : "long, boring, license text",
                  "spdx" : "GPL-2.0"
              }
          ],
          "copyrightStatement" : "© 2010-2018 Plasma Development Team",
          "desktopFileName" : "org.kde.kirigamiapp"
       }
       @endcode
     *
     * @see KAboutData
     */
    property alias aboutData: aboutItem.aboutData

    /**
     * This property holds a link to a "Get Involved" page. By default link to
     * "https://community.kde.org/Get_Involved" when your application application
     * id starts with "org.kde.", otherwise is empty.
     */
    property alias getInvolvedUrl: aboutItem.getInvolvedUrl

    /** @internal */
    default property alias _content: aboutItem._content

    title: qsTr("About %1").arg(page.aboutData.displayName)

    actions.main: Action {
        text: qsTr("Report Bug…")
        icon.name: "tools-report-bug"
        onTriggered: {
            if (page.aboutData.bugAddress === "submit@bugs.kde.org") {
                const elements = page.aboutData.productName.split('/');
                let url = `https://bugs.kde.org/enter_bug.cgi?format=guided&product=${elements[0]}&version=${page.aboutData.version}`;
                if (elements.length === 2) {
                    url += "&component=" + elements[1]
                }
                Qt.openUrlExternally(url)
            } else {
                Qt.openUrlExternally(page.aboutData.bugAddress)
            }
        }
    }

    AboutItem {
        id: aboutItem

        _usePageStack: applicationWindow().pageStack ? true : false
    }
}
