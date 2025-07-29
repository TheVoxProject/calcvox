import wx

from calcvox.history import History


class HistoryDialog(wx.Dialog):
	def __init__(self, parent: wx.Window, history: History) -> None:
		super().__init__(parent, title="History")
		self.history = history
		self.list_box = wx.ListBox(self, style=wx.LB_SINGLE)
		self.list_box.Bind(wx.EVT_KEY_DOWN, self.on_key_down)
		self.list_box.Bind(wx.EVT_LISTBOX_DCLICK, self.on_dclick)
		for entry in self.history._entries:
			self.list_box.Append(str(entry))
		sizer = wx.BoxSizer(wx.VERTICAL)
		sizer.Add(self.list_box, 1, wx.EXPAND | wx.ALL, 5)
		self.SetSizer(sizer)

	def on_key_down(self, event: wx.KeyEvent):
		key = event.GetKeyCode()
		if key == wx.WXK_RETURN:
			selection = self.list_box.GetSelection()
			if selection != wx.NOT_FOUND:
				if event.ShiftDown():
					self.EndModal(-(selection + 1))
				else:
					self.EndModal(selection + 1)
		else:
			event.Skip()

	def on_dclick(self, _event):
		selection = self.list_box.GetSelection()
		if selection != wx.NOT_FOUND:
			self.EndModal(selection + 1)
