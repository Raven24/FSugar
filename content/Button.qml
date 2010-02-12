import Qt 4.6

Item {
    id: container

    signal clicked

    property string text
    property bool keyUsing: false

	BorderImage {
		id: buttonImage
		source: "../images/toolbutton.sci"
		width: container.width; height: container.height
	}
	BorderImage {
		id: hover; opacity: 0
		source: "../images/toolbutton.sci"
		width: container.width; height: container.height
		effect: DropShadow { blurRadius: 5; color: "#ffffff"; xOffset: 0; yOffset: 0; }
    }
    BorderImage {
		id: pressed; opacity: 0
		source: "../images/toolbutton.sci"
		width: container.width; height: container.height
		effect: Colorize { color: "#000000"; strength: 0.8; }
    }

    MouseRegion {
        id: mouseRegion
		anchors.fill: buttonImage; hoverEnabled: true;
        onClicked: { container.clicked(); }
    }
    Text {
        id: btnText
        color: if(container.keyUsing){"#DDDDDD";} else {"#FFFFFF";}
        anchors.centerIn: buttonImage; font.bold: true
        text: container.text; style: Text.Raised; styleColor: "black"
        font.pixelSize: 12
    }
    states: [
        State {
            name: "Pressed"
            when: mouseRegion.pressed == true
            PropertyChanges { target: pressed; opacity: 1 }
        },
        State {
            name: "Focused"
            when: container.focus == true
            PropertyChanges { target: btnText; color: "#FFFFFF" }
		},
		State {
			name: "Hover";
			when: mouseRegion.containsMouse;
			PropertyChanges { target: hover; opacity: 1 }
		}
	]
    transitions: Transition {
        ColorAnimation { target: btnText; }
    }
}
