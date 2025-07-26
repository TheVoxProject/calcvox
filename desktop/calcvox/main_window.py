import wx

from calcvox.calc_button import CalcButton


class MainWindow(wx.Frame):
	def __init__(self) -> None:
		super().__init__(None, title="Calcvox")
		self.panel = wx.Panel(self)
		self.btn_one = CalcButton(self.panel, "1")
		self.btn_two = CalcButton(self.panel, "2")
		self.btn_three = CalcButton(self.panel, "3")
		self.btn_plus = CalcButton(self.panel, "+", "Plus")
		sizer = wx.BoxSizer(wx.VERTICAL)
		sizer.Add(self.btn_one, 0, wx.ALL, 5)
		sizer.Add(self.btn_two, 0, wx.ALL, 5)
		sizer.Add(self.btn_three, 0, wx.ALL, 5)
		sizer.Add(self.btn_plus, 0, wx.ALL, 5)
		self.panel.SetSizer(sizer)
		self.Fit()
		self.Show()
