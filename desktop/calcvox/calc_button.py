import wx

class ButtonAccessible(wx.Accessible):
	def __init__(self, button: wx.Button, accessible_name: str) -> None:
		super().__init__(button)
		self.accessible_name = accessible_name

	def GetName(self, child_id):
		return wx.ACC_OK, self.accessible_name

	def GetRole(self, childId):
		return wx.ACC_OK, wx.ROLE_SYSTEM_PUSHBUTTON

class CalcButton(wx.Button):
	def __init__(self, parent: wx.Window, label: str, accessible_label: str | None=None, **kwargs) -> None:
		super().__init__(parent, label=label, **kwargs)
		if accessible_label is not None:
			accessible = ButtonAccessible(self, accessible_label)
			self.SetAccessible(accessible)
