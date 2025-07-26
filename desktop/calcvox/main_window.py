import wx

from calcvox.calc_button import CalcButton


class MainWindow(wx.Frame):
	def __init__(self) -> None:
		super().__init__(None, title="Calcvox")
		self.panel = wx.Panel(self)
		grid = wx.GridSizer(4, 4, 5, 5)
		self.buttons: list[list[CalcButton]] = []
		labels = [
			["7", "8", "9", "/"],
			["4", "5", "6", "*"],
			["1", "2", "3", "-"],
			["0", ".", "=", "+"],
		]
		accessible_names = {
			"+": "Plus",
			"-": "Minus",
			"*": "Times",
			"/": "Divided by",
			"=": "Equals",
			".": "Point",
		}
		for row in labels:
			button_row = []
			for label in row:
				acc_label = accessible_names.get(label)
				btn = CalcButton(self.panel, label=label, accessible_label=acc_label)
				grid.Add(btn, 0, wx.EXPAND)
				button_row.append(btn)
			self.buttons.append(button_row)
		self.panel.SetSizer(grid)
		self.Fit()
		self.Show()
