from typing import Any

import wx


class ButtonAccessible(wx.Accessible):
	"""Provides a custom accessible name for a button."""

	def __init__(self, button: wx.Button, accessible_name: str) -> None:
		super().__init__(button)
		self.accessible_name = accessible_name

	def GetName(self, childId: int) -> tuple[int, str]:
		"""Return the accessible name."""
		_ = childId  # Make pyright happy.
		return wx.ACC_OK, self.accessible_name


class CalcButton(wx.Button):
	"""A button with optional accessibility and key handling."""

	def __init__(self, parent: wx.Window, label: str, accessible_label: str | None = None, **kwargs: Any) -> None:
		super().__init__(parent, label=label, style=wx.WANTS_CHARS, **kwargs)
		self.accessible_name = accessible_label
		if accessible_label is not None:
			accessible = ButtonAccessible(self, accessible_label)
			self.SetAccessible(accessible)
		self.Bind(wx.EVT_KEY_DOWN, self.on_key_down)

	def on_key_down(self, event: wx.KeyEvent) -> None:
		key = event.GetKeyCode()
		if key in (wx.WXK_LEFT, wx.WXK_RIGHT, wx.WXK_UP, wx.WXK_DOWN):
			event.Skip(False)
		else:
			event.Skip()
