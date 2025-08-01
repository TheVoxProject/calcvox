import wx

from calcvox import speech
from calcvox.calc_button import CalcButton
from calcvox.calculator import Calculator
from calcvox.history_dialog import HistoryDialog


class MainWindow(wx.Frame):
	def __init__(self) -> None:
		super().__init__(None, title="Calcvox")
		self.calc = Calculator()
		self.panel = wx.Panel(self)
		self.buttons: list[list[CalcButton]] = []
		grid = wx.GridSizer(5, 4, 5, 5)
		labels = [
			["", "", "B", "C"],
			["7", "8", "9", "/"],
			["4", "5", "6", "*"],
			["1", "2", "3", "-"],
			["0", ".", "=", "+"],
		]
		accessible_names = {"+": "Plus", "-": "Minus", "*": "Times", "/": "Divided by", "=": "Equals", ".": "Point", "B": "Backspace", "C": "Clear"}
		self.label_to_button = {}
		for row in labels:
			button_row = []
			for label in row:
				if label == "":
					grid.AddSpacer(1)
					button_row.append(None)
					continue
				acc_label = accessible_names.get(label, label)
				btn = CalcButton(self.panel, label=label, accessible_label=acc_label)
				btn.Bind(wx.EVT_KEY_DOWN, self.on_key_down)
				btn.Bind(wx.EVT_BUTTON, self.on_btn)
				grid.Add(btn, 0, wx.EXPAND)
				button_row.append(btn)
				self.label_to_button[label] = btn
			self.buttons.append(button_row)
		self.buttons[4][0].SetFocus()
		self.panel.SetSizer(grid)
		self.Fit()
		self.Show()

		accel_tbl = wx.AcceleratorTable([(wx.ACCEL_CTRL, ord("H"), wx.ID_ANY)])
		self.SetAcceleratorTable(accel_tbl)
		self.Bind(wx.EVT_MENU, self.on_show_history)

	def on_show_history(self, _event):
		dialog = HistoryDialog(self, self.calc.history)
		result = dialog.ShowModal()
		if result == 0:
			return
		entry_index = abs(result) - 1
		entry = self.history._entries[entry_index]
		if result > 0:
			self.calc.equation = entry.result
		else:
			self.calc.equation = entry.equation
		dialog.Destroy()

	def on_key_down(self, event: wx.KeyEvent) -> None:
		key = event.GetKeyCode()
		shift = event.ShiftDown()
		current = self.FindFocus()
		char = chr(key) if 32 <= key < 127 else ""
		if key == wx.WXK_BACK:
			if shift:
				self.calc.clear()
			else:
				self.calc.backspace()
		if char in self.label_to_button:
			btn = self.label_to_button[char]
			wx.PostEvent(btn, wx.CommandEvent(wx.EVT_BUTTON.typeId, btn.GetId()))
			return
		directions: dict[int, tuple[int, int]] = {
			wx.WXK_UP: (-1, 0),
			wx.WXK_DOWN: (1, 0),
			wx.WXK_LEFT: (0, -1),
			wx.WXK_RIGHT: (0, 1),
		}
		for r, row in enumerate(self.buttons):
			for c, btn in enumerate(row):
				if btn != current:
					continue
				if key in directions:
					dr, dc = directions[key]
					new_r = r + dr
					new_c = c + dc
					while 0 <= new_r < len(self.buttons) and 0 <= new_c < len(self.buttons[0]):
						next_btn = self.buttons[new_r][new_c]
						if next_btn is not None:
							next_btn.SetFocus()
							return
						new_r += dr
						new_c += dc
					return
				event.Skip()
				return
		event.Skip()

	def on_btn(self, event):
		btn = event.GetEventObject()
		if btn is None:
			btn = self.FindWindowById(event.GetId())
		label = btn.Label
		if label == "=":
			self.calc.evaluate()
		elif label == "C":
			self.calc.clear()
		elif label == "B":
			self.calc.backspace()
		else:
			self.calc.equation += label
			if isinstance(btn, CalcButton):
				speech.speak(self.get_speakable_label(btn))

	def get_speakable_label(self, btn: CalcButton) -> str:
		return btn.accessible_name or btn.Label
