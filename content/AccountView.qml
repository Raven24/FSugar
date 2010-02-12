import Qt 4.6

Component {
	Item {
		id: accountItem; x: 10;
		width: parent.width-20; height: 80;

		property bool containsMouse: mouse.containsMouse;
		property bool detail: false;
		signal clicked()

		function showDetails() {
			if(accountItem.detail) {
				accountItem.detail = false;
				//console.log(model.address);
				model.address = newAddress.text;
				//model.setValue(model.index, {"address": newAddress.text });
				console.log(model.address);
			} else {
				accountItem.detail = true;
				//accountItem.focus = true;
			}

			accountItem.clicked();
		}

		Rectangle {
			id: background; radius: 7; smooth: true
			opacity: 0.3;
			anchors.fill: parent;
			gradient: Gradient {
				GradientStop { position: containsMouse?0.3:1.0; color: "#111111" }
				GradientStop { position: containsMouse?1.0:0.3; color: "#000000" }
			}
		}

		MouseRegion { id: mouse; hoverEnabled: true; anchors.fill: parent; onClicked: accountItem.showDetails(); }

		Text { id: accountName; x: 10; text: name;
			font.pixelSize: 14; font.bold: true; color: "#EEEEEE";
			style: Text.Raised; styleColor: "#999999";
			anchors.left: parent.left; anchors.leftMargin: 15; anchors.top: parent.top; anchors.topMargin:10;
		}

		Text { id: accountPhone; text: telephone;
			anchors { top: accountName.top; right: parent.right; rightMargin: 15; }
			color: "#EFEFEF";
		}
		// Address information
		Text { id: accountAddress; text: address; font.bold: false;
			anchors.top: accountName.bottom; anchors.topMargin:10; anchors.left: accountName.left;
			color: "#EFEFEF";
		}
		Text { id: accountAddress2; text: address2; font.bold: false;
			anchors.top: accountAddress.bottom; anchors.topMargin:5; anchors.left: accountAddress.left;
			color: "#EFEFEF";
		}

		PropertyEdit {
			id: newAddress; opacity: 0;
			anchors { top: accountAddress.top; left: accountAddress.left; }
			width: parent.width / 3; text: address;
		}
		PropertyEdit {
			id: newAddress2; opacity: 0;
			anchors { top: accountAddress2.top; left: accountAddress2.left; }
			width: parent.width / 3; text: address2;
		}
		PropertyEdit {
			id: newPhone; opacity: 0;
			anchors { top: accountPhone.top; right: accountPhone.right; }
			width: parent.width / 3; text: telephone;
		}
	/*	TextInput { id: accountAddress2Edit; text: address2;
			anchors.fill: accountAddress2; opacity: 0; }
		TextInput { id: accountPhoneEdit; text: telephone;
			anchors.fill: accountPhone; opacity: 0; }
	*/
		states: [
			State {
				name: "Detail";
				when: accountItem.detail
				PropertyChanges { target: accountItem; height: 200; }

				/*PropertyChanges { target: accountName; opacity: 0; }*/
				PropertyChanges { target: accountPhone; opacity: 0; }
				PropertyChanges { target: accountAddress; opacity: 0; }
				PropertyChanges { target: accountAddress2; opacity: 0; }

				/*PropertyChanges { target: accountNameEdit; opacity: 1; }
				PropertyChanges { target: accountPhoneEdit; opacity: 1; } */
				PropertyChanges { target: newAddress; opacity: 1; }
				PropertyChanges { target: newAddress2; opacity: 1; }
				PropertyChanges { target: newPhone; opacity: 1; }

				PropertyChanges { target: accountItem; focus: true; }
			},
			State {
				name: "Default"
				when: !accountItem.detail
			}
		]
		transitions: [
			Transition {
				NumberAnimation {
					matchProperties: "height, opacity"
					easing: "easeInOutQuad"
				}
			}
		]

	}
}
