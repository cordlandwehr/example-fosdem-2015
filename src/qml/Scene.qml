/*
 *  Copyright 2015  Andreas Cord-Landwehr <cordlandwehr@kde.org>
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 *  IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 *  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 *  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
import QtQuick 2.1
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import org.kde.cordlandwehr.fosdemexample 1.0

RowLayout {
    id: root
    width: 800
    height: 600

    // element create/remove actions
    signal createBox(real x, real y);

    Item {
        id: scene
        Layout.fillWidth: true
        Layout.preferredHeight: root.height

        focus: true

        Rectangle {
            anchors.fill: parent
            color: "lightblue"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.createBox(mouse.x, mouse.y)
                }
            }
        }

        Repeater {
            model: BoxModel {
                boxManager: globalBoxManager
            }
            Rectangle {
                width: 20; height: 20
                color: "yellow"
                border.width: 2
                property Box box: model.dataRole
                x: box.position.x
                y: box.position.y
            }
        }
    }
    ListView{
        Layout.preferredHeight: root.height
        Layout.minimumWidth: 100
        model: BoxModel {
            boxManager: globalBoxManager
        }
        delegate: Text {
            text: model.dataRole.position.x + " / " + model.dataRole.position.y
        }
    }
}
