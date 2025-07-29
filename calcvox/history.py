from dataclasses import dataclass, field


@dataclass(frozen=True)
class HistoryEntry:
	"""Represents a single item in the calculation history."""

	equation: str
	result: str

	def __str__(self) -> str:
		"""Returns the readable form of this history item (e.g. 2+2 = 4)."""
		return f"{self.equation} = {self.result}"


@dataclass
class History:
	"""Stores a list of calculation history entries."""

	_entries: list[HistoryEntry] = field(default_factory=list, init=False, repr=False)

	def add(self, entry: HistoryEntry) -> None:
		"""Add a new entry to the history."""
		self._entries.append(entry)

	def clear(self) -> None:
		"""Clear the history of all items."""
		self._entries.clear()

	def all(self) -> list[HistoryEntry]:
		"""Return a list of all history entries."""
		return self._entries.copy()
