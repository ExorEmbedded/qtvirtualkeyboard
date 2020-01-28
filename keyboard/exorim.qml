/* EXOR custom input method */

import QtQuick 2.0
import QtQuick.VirtualKeyboard 1.0

import com.exor.Keyboard 1.0 as ExorKeyboard

InputMethod {

    function selectionLists() {
        var selectionList = []
        //console.log("EXOR selectionLists:" + selectionList)
        return selectionList;
    }

    function inputModes(locale) {
        var modes = [InputEngine.Latin, InputEngine.InputMode.Numeric]
        //console.log("EXOR inputModes(" +locale+ "):" + modes)
        return modes;
    }

    function setInputMode(locale, inputMode) {
        //console.log("EXOR setInputMode(locale:" + locale + ", mode:" + inputMode + ")")
        return true
    }

    function setTextCase(textCase) {
        //console.log("EXOR setTextCase(" + textCase + ")")
        return true
    }

    function reset() {
        //console.log("EXOR reset()")
    }

    function update() {
        //console.log("EXOR update()")
    }

    // Handle key and pass it back to ExorVirtualKeyboard backend
    function keyEvent(key, text, modifiers) {
        var accept = false
        //console.log("EXOR keyEvent key:" + key + ",text:" + text + ",mod:" + modifiers)
        accept = ExorKeyboard.Wayland.keyEvent(key, text, modifiers)
        //console.log("EXOR keyEvent accept:" + accept)
        return accept;
    }
}
