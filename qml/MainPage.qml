/****************************************************************************
**
** Copyright (C) 2017-2020 Elros https://github.com/elros34
**               2020 Rinigus https://github.com/rinigus
**               2012 Digia Plc and/or its subsidiary(-ies).
**
** This file is part of Flatpak Runner.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**
****************************************************************************/

import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: root
    objectName: "mainPage"

    property int nwindows: 0

    Label {
        id: hintLabel
        anchors.centerIn: parent
        font.pixelSize: Theme.fontSizeLarge
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: {
            if (runner.program && hintLabel.status)
                return qsTr("Flatpak: %1\n%2").arg(runner.program).arg(hintLabel.status);
            if (runner.program)
                return qsTr("Flatpak: %1").arg(runner.program);
            return qsTr("Flatpak Runner");
        }
        visible: nwindows <= 0
        width: root.width - 2*Theme.horizontalPageMargin
        wrapMode: Text.WordWrap

        property string status
        Connections {
            target: runner
            onExit: hintLabel.status = qsTr("Application finished")
        }
    }

    function windowAdded(window) {
        var windowContainerComponent = Qt.createComponent("WindowContainer.qml");
        if (windowContainerComponent.status !== Component.Ready) {
            console.warn("Error loading WindowContainer.qml: " +  windowContainerComponent.errorString());
            return;
        }

        var windowContainer = windowContainerComponent.createObject(root, {
                                                                    "child": compositor.item(window)
                                                                    });
        windowContainer.objectName = "windowContainer"
        windowContainer.child.resizeSurfaceToItem = true
        windowContainer.child.parent = windowContainer;
        windowContainer.child.touchEventsEnabled = true;

        nwindows += 1

        windowContainer.child.takeFocus()
    }

    function windowResized(window) {
        window.width = window.surface.size.width;
        window.height = window.surface.size.height;
    }

    function removeWindow(window) {
        window.destroy();
        nwindows -= 1
    }
}

