import wx

from calcvox.calc_button import CalcButton  # Assuming this extends wx.Button


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
		for _, row in enumerate(labels):
			button_row = []
			for _, label in enumerate(row):
				acc_label = accessible_names.get(label)
				btn = CalcButton(self.panel, label=label, accessible_label=acc_label)
				btn.Bind(wx.EVT_KEY_DOWN, self.on_key_down)
				grid.Add(btn, 0, wx.EXPAND)
				button_row.append(btn)
			self.buttons.append(button_row)
		self.panel.SetSizer(grid)
		self.Fit()
		self.Show()

	def on_key_down(self, event: wx.KeyEvent):
		key = event.GetKeyCode()
		current = self.FindFocus()
		for r, row in enumerate(self.buttons):
			for c, btn in enumerate(row):
				if btn == current:
					new_r, new_c = r, c
					if key == wx.WXK_UP:
						new_r = max(r - 1, 0)
					elif key == wx.WXK_DOWN:
						new_r = min(r + 1, len(self.buttons) - 1)
					elif key == wx.WXK_LEFT:
						new_c = max(c - 1, 0)
					elif key == wx.WXK_RIGHT:
						new_c = min(c + 1, len(row) - 1)
					else:
						event.Skip()
						return
					self.buttons[new_r][new_c].SetFocus()
					return
		event.Skip()
