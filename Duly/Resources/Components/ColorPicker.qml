import QtQuick 2.9
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import "../Forms"

ColorPickerForm {
    button.onClicked: colorDialogRef.open()
}