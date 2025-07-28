import wx

from calcvox import speech
from calcvox.calc_button import CalcButton
from calcvox.calculator import Calculator


class MainWindow(wx.Frame):
	def __init__(self) -> None:
		super().__init__(None, title="Calcvox")
		self.calc = Calculator()
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
				acc_label = accessible_names.get(label, label)
				btn = CalcButton(self.panel, label=label, accessible_label=acc_label)
				btn.Bind(wx.EVT_KEY_DOWN, self.on_key_down)
				btn.Bind(wx.EVT_BUTTON, self.on_btn)
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
					elif key == wx.WXK_BACK:
						removed = self.calc.backspace()
						if removed:
							speech.speak(removed)
						else:
							speech.speak("blank")
						return
					else:
						event.Skip()
						return
					self.buttons[new_r][new_c].SetFocus()
					return
		event.Skip()

	def on_btn(self, event):
		btn = event.GetEventObject()
		label = btn.GetLabel()
		if label == "=":
			try:
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
