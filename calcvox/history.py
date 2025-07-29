from dataclasses import dataclass, field


@dataclass(frozen=True)
class HistoryEntry:
	"""Represents a single item in the calculation history."""

	equation: str
	result: str

	def __str__(self) -> str:
		return f"{self.equation} = {self.result}"


@dataclass
class History:
	"""Stores a list of calculation history entries and manages a cursor for navigation."""

	_entries: list[HistoryEntry] = field(default_factory=list, init=False, repr=False)
	_cursor: int = field(default=-1, init=False, repr=False)

	def add(self, entry: HistoryEntry) -> None:
		"""Add a new entry to the history and reset cursor to the end."""
		self._entries.append(entry)
		self._cursor = len(self._entries) - 1

	def clear(self) -> None:
		"""Clear the history and reset the cursor."""
		self._entries.clear()
		self._cursor = -1

	def current(self) -> HistoryEntry | None:
		"""Returns the currently selected history entry, if any."""
		if 0 <= self._cursor < len(self._entries):
			return self._entries[self._cursor]
		return None

	def current_equation(self) -> str | None:
		entry = self.current()
		return entry.equation if entry else None

	def current_result(self) -> str | None:
		entry = self.current()
		return entry.result if entry else None

	def move_back(self) -> bool:
		"""Move the cursor one step back in history. Returns True if successful."""
		if self._cursor > 0:
			self._cursor -= 1
			return True
		return False

	def move_forward(self) -> bool:
		"""Move the cursor one step forward in history. Returns True if successful."""
		if self._cursor < len(self._entries) - 1:
			self._cursor += 1
			return True
		return False

	def move_to_top(self) -> bool:
		"""Move the cursor to the first history entry."""
		if self._entries:
			self._cursor = 0
			return True
		return False

	def move_to_bottom(self) -> bool:
		"""Move the cursor to the last history entry."""
		if self._entries:
			self._cursor = len(self._entries) - 1
			return True
		return False
