import QtQuick 2.12
import QtQuick.Controls 2.12 as QQC2
import org.kde.kirigami 2.14 as Kirigami
import QtTest 1.0

Item {
    id: root

    width: 110
    height: 110 * 3

    TestCase {
        name: "AvatarTests"
        function test_latin_name() {
            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("Nate Martin"), false)
            compare(Kirigami.NameUtils.initialsFromString("Nate Martin"), "NM")

            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("Kalanoka"), false)
            compare(Kirigami.NameUtils.initialsFromString("Kalanoka"), "K")

            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("Why would anyone use such a long not name in the field of the Name"), false)
            compare(Kirigami.NameUtils.initialsFromString("Why would anyone use such a long not name in the field of the Name"), "WN")

            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("Live-CD User"), false)
            compare(Kirigami.NameUtils.initialsFromString("Live-CD User"), "LU")
        }
        // these are just randomly sampled names from internet pages in the
        // source languages of the name
        function test_jp_name() {
            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("北里 柴三郎"), false)
            compare(Kirigami.NameUtils.initialsFromString("北里 柴三郎"), "北")

            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("小野田 寛郎"), false)
            compare(Kirigami.NameUtils.initialsFromString("小野田 寛郎"), "小")
        }
        function test_cn_name() {
            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("蔣經國"), false)
            compare(Kirigami.NameUtils.initialsFromString("蔣經國"), "蔣")
        }
        function test_bad_names() {
            compare(Kirigami.NameUtils.isStringUnsuitableForInitials("151231023"), true)
        }
    }
    TestCase {
        name: "AvatarActions"

        width: 110
        height: 110 * 3
        visible: true
        when: windowShown

        Kirigami.Avatar {
            id: avatarWithNullAction

            x: 5
            y: 5
            width: 100
            height: 100

            actions.main: null
            activeFocusOnTab: true
        }

        Kirigami.Avatar {
            id: avatarWithKirigamiAction

            x: 5
            y: 110 + 5
            width: 100
            height: 100

            actions.main: Kirigami.Action {
                onTriggered: {
                    signalProxyForKirigamiAction.triggered();
                }
            }

            QtObject {
                id: signalProxyForKirigamiAction
                signal triggered()
            }

            SignalSpy {
                id: spyKirigamiAction
                target: signalProxyForKirigamiAction
                signalName: "triggered"
            }
        }

        Kirigami.Avatar {
            id: avatarWithImpostorAction

            x: 5
            y: 110 * 2 + 5
            width: 100
            height: 100

            // Ideally, custom objects should not be assignable here
            actions.main: QtObject {
                function trigger() {
                    signalProxyForImpostorAction.triggered();
                    return true;
                }
            }

            QtObject {
                id: signalProxyForImpostorAction
                signal triggered()
            }

            SignalSpy {
                id: spyImpostorAction
                target: signalProxyForImpostorAction
                signalName: "triggered"
            }
        }

        function test_null_action() {
            mouseClick(avatarWithNullAction);
            avatarWithNullAction.forceActiveFocus(Qt.TabFocusReason);
            compare(avatarWithNullAction.activeFocus, true);
            keyClick(Qt.Key_Space);
            keyClick(Qt.Key_Return);
            keyClick(Qt.Key_Enter);
            // Should not print any TypeError warnings, but there's no way to
            // test it, except that it should not abort execution of this
            // test script.
            // TODO KF6: Use failOnWarning available from Qt 6.3
        }

        function test_kirigami_action() {
            spyKirigamiAction.clear();
            mouseClick(avatarWithKirigamiAction);
            compare(spyKirigamiAction.count, 1);
            avatarWithKirigamiAction.forceActiveFocus(Qt.TabFocusReason);
            keyClick(Qt.Key_Space);
            compare(spyKirigamiAction.count, 2);
            keyClick(Qt.Key_Return);
            compare(spyKirigamiAction.count, 3);
            keyClick(Qt.Key_Enter);
            compare(spyKirigamiAction.count, 4);
        }

        function test_impostor_action() {
            spyImpostorAction.clear();
            mouseClick(avatarWithImpostorAction);
            compare(spyImpostorAction.count, 1);
            avatarWithImpostorAction.forceActiveFocus(Qt.TabFocusReason);
            keyClick(Qt.Key_Space);
            compare(spyImpostorAction.count, 2);
            keyClick(Qt.Key_Return);
            compare(spyImpostorAction.count, 3);
            keyClick(Qt.Key_Enter);
            compare(spyImpostorAction.count, 4);
        }
    }
}
