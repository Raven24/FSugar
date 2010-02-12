import Qt 4.6

Item {
	id: wrapper

	Column {
		anchors.centerIn: parent; spacing: 20;

		Column {
			Text {
				text: "Username:"
				color: "white"; font.pixelSize: 10; font.bold: true; horizontalAlignment: Qt.AlignRight;
			}
			Item {
				width: 220; height: 30;
				BorderImage { source: "../images/lineedit.sci"; anchors.fill: parent; }
				TextInput {
					id: nameIn;
					width: parent.width-8; anchors.centerIn: parent;
					font.pixelSize: 16; font.bold: true;
					color: "#151515"; focus: true;
				}
			}
		}

		Column {
			id: passWrap
			Text {
				text: "Password:"
				color: "white"; font.pixelSize: 10; font.bold: true; horizontalAlignment: Qt.AlignRight;
			}
			Item {
				width: 220; height: 30;
				BorderImage { source: "../images/lineedit.sci"; anchors.fill: parent; }
				TextInput {
					id: passIn;
					width: parent.width-8; anchors.centerIn: parent;
					font.pixelSize: 16; font.bold: true;
					color: "#151515";
					echoMode: TextInput.Password
				}
			}
		}

		Button {
			width: 100; height: 32;
			anchors.horizontalCenter: passWrap.horizontalCenter;
			id: login

			function doLogin() {
				canvas.state = "progress";
				canvas.username = nameIn.text;
				canvas.password = passIn.text;

				canvas.loggingIn();
			}

			text: "Login"
			onClicked: login.doLogin();

		}
		Button {
			width: 100; height: 32;
			anchors.horizontalCenter: login.horizontalCenter;
			id: test

			function doTest() {
				canvas.state = "testing";
				canvas.username = nameIn.text;
				canvas.password = passIn.text;

				//canvas.loggingIn();
			}

			text: "Test"
			onClicked: test.doTest();

		}
	}
}
