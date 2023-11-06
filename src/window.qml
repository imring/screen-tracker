import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600

    Item {
        anchors.fill: parent
        anchors.margins: 20
        anchors.rightMargin: 0

        GridView {
            anchors.fill: parent
            width: parent.width
            height: parent.height

            cellWidth: 150 + parent.anchors.margins

            model: elements.length()
            delegate: Item {
                ColumnLayout {
                    Image {
                        source: "file:" + elements.path(index)
                        sourceSize.width: 150
                    }
                    Text {
                        Layout.alignment: Qt.AlignHCenter
                        text: "Similarity: " + elements.similarity(index).toFixed(2) + "%"
                    }
                }
            }
        }
    }
}