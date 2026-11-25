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
**   * Neither the name of the copyright holder nor the names of its
**     contributors may be used to endorse or promote products derived
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
import Amber.Mpris 1.0
import Nemo.DBus 2.0
import "."

ApplicationWindow {
    id: app

    initialPage: MainPage {}
    // The app's live window, or, when it plays music (MPRIS), the media player's cover.
    cover: mprisService !== "" ? mprisCover : undefined
    allowedOrientations: defaultAllowedOrientations
    _defaultPageOrientations: allowedOrientations

    property bool darkStyle: Theme.colorScheme === Theme.LightOnDark
    property var  py

    // The app's MPRIS service: by convention named after its program
    // (org.mpris.MediaPlayer2.<name>, maybe with an instance suffix).
    readonly property string mprisService: {
        var name = (runner.program || "").split("/").pop().toLowerCase();
        var services = mpris.availableServices;
        for (var i = 0; name !== "" && i < services.length; i++)
            if (services[i].toLowerCase().indexOf("org.mpris.mediaplayer2." + name) === 0)
                return services[i];
        return "";
    }

    // The app's own player only: unlocked, Amber follows any player that starts playing
    // (a video in the browser), and the cover would show and control that one.
    MprisController {
        id: mpris
        singleService: true
        currentService: app.mprisService
    }

    Component {
        id: mprisCover
        MprisCover { player: mpris }
    }

    Connections {
        target: runner
        onExit: {
            console.log("Skipping quit as it will hang the window. Proper exit is needed");
            // Qt.quit();
        }
    }

    // org.sailfishos.qtrunner.p<program, as a D-Bus name>, method activate: brings this
    // window to the front. A program started again (Sailfish's single-instance launch can't
    // tell qt-runner's apps apart) or asked to show itself (MPRIS Raise) calls it.
    DBusAdaptor {
        service: modeSettings ? "" : "org.sailfishos.qtrunner.p" + (runner.program || "").replace(/[^A-Za-z0-9_]/g, "_")
        path: "/"
        iface: "org.sailfishos.qtrunner"
        xml: '<interface name="org.sailfishos.qtrunner"><method name="activate"/></interface>'
        function activate() { app.activate(); }
    }

    Component.onCompleted: {
        settings.dark = darkStyle;
        if (!modeSettings) {
            //settings.applyTheme(runner.program);
            runner.start();
        }
    }

    Component.onDestruction: {
        compositor.cleanup();
        runner.stop();
    }

    onDarkStyleChanged: settings.dark = darkStyle
}
