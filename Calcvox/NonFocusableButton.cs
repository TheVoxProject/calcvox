using Eto.Forms;

namespace Calcvox;

internal class NonFocusableButton : Button {
	public NonFocusableButton() {
		TabIndex = -1;
	}
}
