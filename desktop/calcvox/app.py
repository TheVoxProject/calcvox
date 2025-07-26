import wx
from calcvox.main_window import MainWindow


class App(wx.App):
	def OnInit(self) -> bool:
		main_window = MainWindow()
		return True
