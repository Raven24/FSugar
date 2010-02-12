import Qt 4.6

Item {
	id: propertyEdit;
	property alias text: newProperty.text;
	//property alias focus: newProperty.focus;

	Rectangle {
		radius: 3; smooth: true; height: newProperty.height + 4;
		anchors { top: parent.top; left: parent.left; right: parent.right; }
		gradient: Gradient {
			GradientStop { position: 1.0; color: "#999999" }
			GradientStop { position: 0.3; color: "#777777" }
		}
		border { width: 1; color: "#BBBBBB"; }
	}

	//MouseRegion { id: mouseRegion; anchors.fill: parent; onClicked: { newProperty.focus = true; } }

	TextInput { id: newProperty; color: "#FFFFFF";
		anchors { top: parent.top; topMargin: 2; left: parent.left; leftMargin: 2; }
	}

}
