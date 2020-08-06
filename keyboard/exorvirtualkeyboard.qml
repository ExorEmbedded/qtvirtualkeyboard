/* EXOR keyboard QQuickView */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQuick.VirtualKeyboard 2.3
import QtQuick.VirtualKeyboard.Settings 2.0

import com.exor.Keyboard 1.0 as ExorKeyboard

Item {
    id: window
    visible: true

    Loader {
        id: ime
        source: "qrc:/exorim.qml"
    }

    /* Temporary initialization */
    Binding {
        target: VirtualKeyboardSettings
        property: "locale"
        value: "it_IT"
    }
    Binding {
        target: VirtualKeyboardSettings
        property: "activeLocales"
        value: ["it_IT"]
    }

    /* Helper (hidden) elements */
    RowLayout {
        id : rl
        height:100

        // This helper TextField is used to trigger the activation
        // of the Qt InputContext obj, in order to initialize correctly
        // the QtVirtualKeyboard
        TextField {
            id: tfh
            objectName: "textFieldHelper"
            focus: false
            inputMethodHints: Qt.ImhDigitsOnly //Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            height: 30
            width: 100
            visible: false
        }

        anchors.top: inputPanel.top
        //anchors.bottom: inputPanel.top
        anchors.left: inputPanel.left
    }

    InputPanel {
        property string locale: InputContext.locale

        onLocaleChanged: ExorKeyboard.Settings.updateLocale(locale)

        id: inputPanel
        objectName: "inputPanel"
        active: false
        z: 99
        x: window.width > window.height ? window.width/10 : 0
        y: window.height - inputPanel.height
        width: window.width > window.height ? (window.width*4)/5 : window.width
        visible: true
    }


    Item {
        id: exorIMHandler
        objectName: "exorIMHandler"

        // The method will set the Exor InputMethod as active.
        // The inputPanel will be activated on success.
        function setCustomInputMethod() {

            // Read settings from DBUS and configure VirtualKeyboard Settings
            VirtualKeyboardSettings.activeLocales = []
            VirtualKeyboardSettings.activeLocales = ExorKeyboard.Settings.activeLocales
            VirtualKeyboardSettings.locale = null
            VirtualKeyboardSettings.locale = ExorKeyboard.Settings.locale

            //console.log("Try to set: " + ime.item)
            InputContext.inputEngine.inputMethod = ime.item
            //console.log("Active IM: " + InputContext.inputEngine.inputMethod)

            // Then activate input panel
            inputPanel.active = true

            tfh.forceActiveFocus()
        }

        function updateInputPanel() {
            // Require settings update, then configure VirtualKeyboardSettings
            ExorKeyboard.Settings.update()

            VirtualKeyboardSettings.activeLocales = []
            VirtualKeyboardSettings.activeLocales = ExorKeyboard.Settings.activeLocales
            VirtualKeyboardSettings.locale = null
            VirtualKeyboardSettings.locale = ExorKeyboard.Settings.locale
        }
    }
}
