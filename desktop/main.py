import wx

class CalculatorFrame(wx.Frame):
	def __init__(self):
		super().__init__(None, title="Calcvox")
		panel = wx.Panel(self)
		self.output = wx.TextCtrl(panel, style=wx.TE_RIGHT | wx.TE_READONLY)
		self.grid = wx.GridSizer(4, 4, 5, 5)
		self.buttons = []
		self.labels = [
			"7", "8", "9", "/",
			"4", "5", "6", "*",
			"1", "2", "3", "-",
			"0", "C", "=", "+"
		]
		for label in self.labels:
			btn = wx.Button(panel, label=label)
			btn.Bind(wx.EVT_BUTTON, self.on_button)
			btn.Bind(wx.EVT_KEY_DOWN, self.on_key)
			btn.SetWindowStyleFlag(wx.WANTS_CHARS)  # allow arrow keys to reach handler
			self.grid.Add(btn, 0, wx.EXPAND)
			self.buttons.append(btn)
		self.focus_index = 0
		self.buttons[self.focus_index].SetFocus()
		vbox = wx.BoxSizer(wx.VERTICAL)
		vbox.Add(self.output, 0, wx.EXPAND | wx.ALL, 5)
		vbox.Add(self.grid, 1, wx.EXPAND | wx.ALL, 5)
		panel.SetSizer(vbox)
		self.input = ""

	def on_button(self, event):
		label = event.GetEventObject().GetLabel()
		if label == "C":
			self.input = ""
		elif label == "=":
			try:
				self.input = str(eval(self.input))
			except Exception:
				self.input = "Error"
		else:
			self.input += label
		self.output.SetValue(self.input)

	def on_key(self, event):
		key = event.GetKeyCode()
		row = self.focus_index // 4
		col = self.focus_index % 4
		new_row, new_col = row, col
		if key == wx.WXK_LEFT:
			if col > 0:
				new_col = col - 1
		elif key == wx.WXK_RIGHT:
			if col < 3:
				new_col = col + 1
		elif key == wx.WXK_UP:
			if row > 0:
				new_row = row - 1
		elif key == wx.WXK_DOWN:
			if row < 3:
				new_row = row + 1
		elif key == wx.WXK_TAB:
			return
		elif key == wx.WXK_RETURN or key == wx.WXK_NUMPAD_ENTER:
			self.buttons[self.focus_index].Command(wx.CommandEvent(wx.wxEVT_COMMAND_BUTTON_CLICKED))
			return
		else:
			event.Skip()
			return
		new_index = (new_row * 4) + new_col
		if new_index != self.focus_index:
			self.focus_index = new_index
			self.buttons[self.focus_index].SetFocus()

if __name__ == "__main__":
	app = wx.App(False)
	frame = CalculatorFrame()
	frame.Show()
	app.MainLoop()
