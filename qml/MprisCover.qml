/****************************************************************************
**
** Copyright (C) 2026 qt-runner contributors
**
** This file is part of Qt Runner, under the BSD license (see main.qml).
**
****************************************************************************/

// The cover of an app that plays music (it offers MPRIS), as the media player's: the song
// over its art, with play/pause and next; while nothing plays, the album arts the app
// lists under qtrunner:recentArtUrls (its last albums; else the last song's art), with
// play. Apps without MPRIS keep their live window as the cover (main.qml).

import QtQuick 2.0
import Sailfish.Silica 1.0
import Amber.Mpris 1.0

CoverBackground {
    id: cover

    property var player // an Amber MprisController on the app's service

    readonly property var meta: player.metaData
    readonly property bool hasSong: meta.title !== undefined && meta.title !== ""
    readonly property bool playing: player.playbackStatus === Mpris.Playing
    readonly property bool idle: !hasSong || player.playbackStatus === Mpris.Stopped
    readonly property var idleArt: {
        var l = meta.extraFields ? meta.extraFields["qtrunner:recentArtUrls"] : undefined;
        l = l ? Array.prototype.slice.call(l) : [];
        if (l.length === 0 && meta.artUrl)
            l = [meta.artUrl];
        return l;
    }
    readonly property string artist: {
        var a = meta.contributingArtist;
        return a === undefined ? "" : (Array.isArray(a) ? a.join(", ") : String(a));
    }

    // Nothing to show yet (as the media player with no music): the app's icon and name.
    CoverPlaceholder {
        text: player.identity
        icon.source: player.desktopEntry ? "/usr/share/icons/hicolor/172x172/apps/" + player.desktopEntry + ".png" : ""
        visible: cover.idle && cover.idleArt.length === 0
    }

    // Idle: one large art, two small ones under it (dimmed), as the media player's cover.
    Item {
        anchors.fill: parent
        visible: cover.idle
        Image {
            id: large
            width: parent.width
            height: cover.idleArt.length > 1 ? width : parent.height
            source: cover.idleArt.length > 0 ? cover.idleArt[0] : ""
            sourceSize.width: width
            sourceSize.height: height
            fillMode: Image.PreserveAspectCrop
            asynchronous: true
        }
        Image {
            id: left
            anchors.top: large.bottom
            width: cover.idleArt.length > 2 ? parent.width / 2 : parent.width
            height: parent.height - large.height
            visible: cover.idleArt.length > 1
            source: visible ? cover.idleArt[1] : ""
            sourceSize.width: width
            sourceSize.height: height
            fillMode: Image.PreserveAspectCrop
            opacity: Theme.opacityLow
            asynchronous: true
        }
        Image {
            anchors { top: left.top; left: left.right }
            width: parent.width / 2
            height: left.height
            visible: cover.idleArt.length > 2
            source: visible ? cover.idleArt[2] : ""
            sourceSize.width: width
            sourceSize.height: height
            fillMode: Image.PreserveAspectCrop
            opacity: Theme.opacityLow
            asynchronous: true
        }
    }

    Image {
        id: art
        anchors.fill: parent
        visible: !cover.idle && status === Image.Ready
        source: cover.hasSong && cover.meta.artUrl ? cover.meta.artUrl : ""
        sourceSize.width: width
        sourceSize.height: height
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
    }

    Rectangle {
        width: parent.width
        height: column.y + column.height + 2 * Theme.paddingLarge
        visible: art.visible
        gradient: Gradient {
            GradientStop { position: 0.0; color: Qt.rgba(0, 0, 0, Theme.opacityHigh) }
            GradientStop { position: 0.6; color: Qt.rgba(0, 0, 0, Theme.opacityLow) }
            GradientStop { position: 1.0; color: "transparent" }
        }
    }

    Column {
        id: column
        x: Theme.paddingMedium
        y: Theme.paddingMedium
        width: parent.width - 2 * Theme.paddingMedium
        spacing: Theme.paddingSmall
        visible: !cover.idle
        opacity: cover.playing ? 1.0 : Theme.opacityHigh

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(implicitWidth, parent.width)
            visible: text !== ""
            text: cover.artist
            color: art.visible && Theme.colorScheme == Theme.DarkOnLight
                   ? Theme.highlightFromColor(Theme.highlightColor, Theme.LightOnDark)
                   : Theme.highlightColor
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeLarge
            lineHeightMode: Text.FixedHeight
            lineHeight: Theme.itemSizeMedium / 2 // aligned with the clock cover's text
            maximumLineCount: 1
        }

        Label {
            anchors.horizontalCenter: parent.horizontalCenter
            width: Math.min(implicitWidth, parent.width)
            text: cover.hasSong ? cover.meta.title : ""
            color: art.visible && Theme.colorScheme == Theme.DarkOnLight ? Theme.lightPrimaryColor : Theme.primaryColor
            wrapMode: Text.WordWrap
            maximumLineCount: 3
            truncationMode: TruncationMode.Fade
            font.pixelSize: Theme.fontSizeLarge
            lineHeightMode: Text.FixedHeight
            lineHeight: Theme.itemSizeMedium / 2
        }
    }

    CoverActionList {
        enabled: !cover.idle
        iconBackground: art.visible

        CoverAction {
            iconSource: cover.playing ? "image://theme/icon-cover-pause" : "image://theme/icon-cover-play"
            onTriggered: cover.player.playPause()
        }

        CoverAction {
            iconSource: "image://theme/icon-cover-next-song"
            onTriggered: cover.player.next()
        }
    }

    CoverActionList {
        enabled: cover.idle && cover.player.canPlay
        iconBackground: cover.idleArt.length > 0

        CoverAction {
            iconSource: "image://theme/icon-cover-play"
            onTriggered: cover.player.play()
        }
    }
}
