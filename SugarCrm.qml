import Qt 4.6
import "content"

Item {
	id: canvas;
	width: windowWidth; height: windowHeight;

	signal loggingIn()
	signal searching()
	property string username;
	property string password;
	property string searchTerm;

	function displayAccounts() {
		canvas.state = "accounts";
		console.log("ready to display");
		accountsListView.model = accountModel
	}
	//property string state: "*";

	Rectangle {
		color: "#333333";
		anchors.fill: parent;
		Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.3; }
	}

	Item {
		id: failedMsg;
		anchors.horizontalCenter: parent.horizontalCenter; anchors.top: parent.top; anchors.topMargin: windowHeight/5;
		width: 300; height: 20; opacity: 0;
		Text {
			text: "Login fehlgeschlagen!"; color: "#FFFFFF"; anchors.horizontalCenter: parent.horizontalCenter;
			font.pixelSize: 14; font.bold: true; horizontalAlignment: AlignHCenter;
		}
		Timer { id: failTimer; interval: 5000; onTriggered: canvas.state = "*"; }
	}
	Item {
		id: auth
		anchors.fill: parent;
		AuthView {
			id: authView;
			anchors.verticalCenter: parent.verticalCenter; anchors.top: parent.top;
			width: parent.width; height: parent.height-60;
		}
	}

	Item {
		id: loader
		anchors.fill: parent;
		opacity: 0;
		Loading { anchors.centerIn: parent; }
	}

	Item {
		id: accounts
		anchors.fill: parent;
		opacity: 0;

		AccountView {
			id: accountDelegate
		}

		TestListModel {
			id: testModel
		}

		ListView {
			id: accountsListView
			delegate: accountDelegate
			anchors { fill: parent; top: parent.top; topMargin: 10; }
			spacing: 10;
		}

		SearchField {
			id: search
			anchors.right: parent.right; anchors.bottom: parent.bottom;
			width: parent.width/3; opacity: 0.7;
		}
	}

	Connection {
		sender: search; signal: "searching(search)"; script: { canvas.searchTerm = search; canvas.searching(); console.log("search!"); }
	}

	states: [
		State {
			name: "*"
		},
		State {
			name: "loginFailed"; extend: "*";
			PropertyChanges { target: failedMsg; opacity: 1; }
			PropertyChanges { target: failTimer; running: true; }
		},
		State {
			name: "progress"
			PropertyChanges { target: auth; opacity: 0; }
			PropertyChanges { target: loader; opacity: 1; }
		},
		State {
			name: "accounts"
			PropertyChanges { target: auth; opacity: 0; }
			PropertyChanges { target: accounts; opacity: 1; }
		},
		State {
			name: "testing"
			PropertyChanges { target: auth; opacity: 0; }
			PropertyChanges { target: accounts; opacity: 1; }
			PropertyChanges { target: accountsListView; model: testModel; }
		},
		State {
			name: "searching"
			when: search.isActive;
			extend: "accounts"
			PropertyChanges { target: search; opacity: 1; }
		}
	]
	transitions: [
		Transition {
			NumberAnimation {
				matchProperties: "opacity"
				easing: "easeInOutQuad"
			}
		}
	]
}
