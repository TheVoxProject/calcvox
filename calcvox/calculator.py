from dataclasses import dataclass


@dataclass
class Calculator:
	equation: str = ""

	def backspace(self) -> str | None:
		if not self.equation:
			return None
		removed = self.equation[-1]
		self.equation = self.equation[:-1]
		return removed
