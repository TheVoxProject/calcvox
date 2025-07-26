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
				btn.Bind(wx.EVT_KEY_DOWN, self.on_key)
				grid.Add(btn, 0, wx.EXPAND)
				button_row.append(btn)
			self.buttons.append(button_row)
		self.panel.SetSizer(grid)
		self.Fit()
		self.Show()

	def on_key(self, event: wx.KeyEvent):
		key = event.GetKeyCode()
		ctrl = event.GetEventObject()
		row, col = self.find_button(ctrl)
		if key == wx.WXK_LEFT:
			self.set_focus(row, col - 1)
		elif key == wx.WXK_RIGHT:
			self.set_focus(row, col + 1)
		elif key == wx.WXK_UP:
			self.set_focus(row - 1, col)
		elif key == wx.WXK_DOWN:
			self.set_focus(row + 1, col)
		elif key in (wx.WXK_TAB, wx.WXK_SHIFT, wx.WXK_PAGEUP, wx.WXK_PAGEDOWN):
			return  # Suppress tab-like keys
		else:
			event.Skip()

	def find_button(self, btn: wx.Button) -> tuple[int, int]:
		for i, row in enumerate(self.buttons):
			for j, b in enumerate(row):
				if b is btn:
					return i, j
		return -1, -1

	def set_focus(self, row: int, col: int):
		if 0 <= row < len(self.buttons) and 0 <= col < len(self.buttons[row]):
			self.buttons[row][col].SetFocus()
