import wx

from calcvox import speech
from calcvox.calc_button import CalcButton
from calcvox.calculator import Calculator


class MainWindow(wx.Frame):
	def __init__(self) -> None:
		super().__init__(None, title="Calcvox")
		self.calc = Calculator()
		self.panel = wx.Panel(self)
		self.buttons: list[list[CalcButton]] = []
		grid = wx.GridSizer(4, 4, 5, 5)
		labels = [
			["7", "8", "9", "/"],
			["4", "5", "6", "*"],
			["1", "2", "3", "-"],
			["0", ".", "=", "+"],
		]
		self.label_to_button = {}
		accessible_names = {
			"+": "Plus", "-": "Minus", "*": "Times",
			"/": "Divided by", "=": "Equals", ".": "Point"
		}
		for row in labels:
			button_row = []
			for label in row:
				acc_label = accessible_names.get(label, label)
				btn = CalcButton(self.panel, label=label, accessible_label=acc_label)
				btn.Bind(wx.EVT_KEY_DOWN, self.on_key_down)
				btn.Bind(wx.EVT_BUTTON, self.on_btn)
				grid.Add(btn, 0, wx.EXPAND)
				button_row.append(btn)
				self.label_to_button[label] = btn
			self.buttons.append(button_row)
		self.panel.SetSizer(grid)
		self.Fit()
		self.Show()

	def on_key_down(self, event: wx.KeyEvent) -> None:
		key = event.GetKeyCode()
		current = self.FindFocus()
		char = chr(key) if 32 <= key < 127 else ""
		print(f"Char: {char}")
		if char in self.label_to_button:
			btn = self.label_to_button[char]
			wx.PostEvent(btn, wx.CommandEvent(wx.EVT_BUTTON.typeId, btn.GetId()))
			return
		if key == wx.WXK_BACK:
			removed = self.calc.backspace()
			speech.speak(removed or "blank")
			return
		directions = {
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
					new_r = max(0, min(r + dr, len(self.buttons) - 1))
					new_c = max(0, min(c + dc, len(row) - 1))
					self.buttons[new_r][new_c].SetFocus()
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
			try:
				if self.calc.equation == "":
					return
				result = str(eval(self.calc.equation))
				speech.speak(f"Equals {result}")
				self.calc.equation = result
			except Exception:
				speech.speak("Error")
				self.calc.equation = ""
		else:
			self.calc.equation += label
			speech.speak(self.get_speakable_label(btn))

	def get_speakable_label(self, btn: CalcButton) -> str:
		return btn.accessible_name or btn.Label
