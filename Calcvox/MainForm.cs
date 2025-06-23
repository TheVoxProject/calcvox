using System;
using Eto.Forms;
using Eto.Drawing;

namespace Calcvox;

public partial class MainForm : Form {
	public MainForm() {
		Title = "Calcvox";
		MinimumSize = new Size(600, 400);
		Content = new StackLayout {
			Padding = 10,
			Items =
			{
				"Hello World!",
			}
		};
		var quitCommand = new Command { MenuText = "Quit", Shortcut = Application.Instance.CommonModifier | Keys.Q };
		quitCommand.Executed += (sender, e) => Application.Instance.Quit();
		Menu = new MenuBar {
			QuitItem = quitCommand
		};
	}
}
