using System;
using Eto.Forms;
using Eto.Drawing;

namespace Calcvox;

public class MainForm : Form {
	public MainForm() {
		Title = "Calcvox";
		MinimumSize = new Size(600, 400);
		string[,] labels = {
			{"7", "8", "9", "/"},
			{"4", "5", "6", "*"},
			{"1", "2", "3", "-"},
			{"0", ".", "=", "+"}
		};
		var buttons = new NonFocusableButton[4, 4];
		var rows = new TableRow[4];
		for (int row = 0; row < 4; row++) {
			var cells = new TableCell[4];
			for (int col = 0; col < 4; col++) {
				var btn = new NonFocusableButton {Text = labels[row, col]};
				buttons[row, col] = btn;
				int r = row;
				int c = col;
				cells[col] = new TableCell(btn, true);
			}
			rows[row] = new TableRow(cells);
		}
		var layout = new TableLayout {
			Spacing = new Size(5, 5),
			Padding = 10
		};
		foreach (var row in rows)
			layout.Rows.Add(row);
		Content = layout;
	}
}
