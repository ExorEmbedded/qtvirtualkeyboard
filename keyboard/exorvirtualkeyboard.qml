/* EXOR keyboard QQuickView */

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import QtQuick.VirtualKeyboard 2.4
import QtQuick.VirtualKeyboard.Settings 2.0

Item {
    id: window
    visible: true

    Loader {
        id: ime
        source: "qrc:/exorim.qml"
    }

    /* Temporary settings - will follow configuration */
    Binding {
        target: VirtualKeyboardSettings
        property: "locale"
        value: "en_US"
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
            inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
            height: 30
            width: 100
            visible: true
        }

        anchors.top: inputPanel.top
        //anchors.bottom: inputPanel.top
    }

    InputPanel {
        id: inputPanel
        objectName: "inputPanel"
        active: false
        z: 99
        x: 0
        y: window.height - inputPanel.height
        width: window.width
        visible: true
    }

    Item {
        id: exorIMHandler
        objectName: "exorIMHandler"

        // The method will set the Exor InputMethod as active.
        // The inputPanel will be activated on success.
        function setCustomInputMethod() {

            //console.log("Try to set: " + ime.item)
            InputContext.inputEngine.inputMethod = ime.item
            //console.log("Active IM: " + InputContext.inputEngine.inputMethod)

            // Then activate input panel
            inputPanel.active = true

            tfh.forceActiveFocus()
        }
    }

}

