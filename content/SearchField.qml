import Qt 4.6

Item {
	id: searchField
	height: 33;

	property bool isActive: searchIn.focus;

	Rectangle {
		anchors.fill: parent; radius: 4; smooth: true;
		color: "#111111";
	}

	Item {
		width: parent.width-6; height: 26;
		anchors { top: parent.top; topMargin: 3; right: parent.right; rightMargin: 3; }
		BorderImage { source: "../images/lineedit.sci"; anchors.fill: parent; }

		Image {
			source:	"../images/search.png"; width: 16; height: 16; anchors { top:parent.top; topMargin: 4; left: parent.left; leftMargin: 4; }
		}

		MouseRegion { id: mouse; hoverEnabled: true; anchors.fill: parent; onClicked: { searchField.isActive = true; } }

		TextInput {
			id: searchIn;
			width: parent.width-8; anchors { left: parent.left; leftMargin: 25; verticalCenter: parent.verticalCenter; }
			font.pixelSize: 13;
			color: "#151515";

			Keys.forwardTo: [(returnKey)]
		}



		Item {
			id: returnKey;
			Keys.onPressed: { canvas.searchTerm = searchIn.text; canvas.searching(); }
		}

	}
}
