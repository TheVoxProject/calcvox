import wx


class App(wx.App):
	def OnInit(self) -> bool:
		wx.MessageBox("Test", "It works")
		return True
