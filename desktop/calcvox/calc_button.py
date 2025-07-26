from typing import Any

import wx


class ButtonAccessible(wx.Accessible):
	def __init__(self, button: wx.Button, accessible_name: str) -> None:
		super().__init__(button)
		self.accessible_name = accessible_name

	def GetName(self, childId: int) -> tuple[int, str]:
		_ = childId  # Make pyright happy.
		return wx.ACC_OK, self.accessible_name


class CalcButton(wx.Button):
	def __init__(self, parent: wx.Window, label: str, accessible_label: str | None = None, **kwargs: Any) -> None:
		super().__init__(parent, label=label, **kwargs)
		if accessible_label is not None:
			accessible = ButtonAccessible(self, accessible_label)
			self.SetAccessible(accessible)
