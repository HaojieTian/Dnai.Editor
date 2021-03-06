import QtQuick 2.0
import Dnai.Theme 1.0


TextField {

    property var model: null
    property int index: 0
    padding: 3
    leftPadding: 4
    horizontalAlignment: Text.AlignLeft
    font.capitalization: Font.Capitalize
    font.pointSize: 8

    onAccepted: {
        model.setProp(index, text)
    }
}
