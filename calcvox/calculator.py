from dataclasses import dataclass, field

from sympy import sympify

from calcvox import speech
from calcvox.history import History, HistoryEntry


@dataclass
class Calculator:
	equation: str = ""
	history: History = field(default_factory=History)

	def backspace(self) -> None:
		if self.equation == "":
			speech.speak("Blank")
			return
		removed = self.equation[-1]
		self.equation = self.equation[:-1]
		speech.speak(removed)

	def clear(self) -> None:
		if self.equation == "":
			speech.speak("Blank")
			return
		self.equation = ""
		speech.speak("Cleared")

	def evaluate(self) -> None:
		try:
			if self.equation == "":
				return
			result = sympify(self.equation).evalf()
			speech.speak(f"Equals {result}")
			self.equation = str(result)
			self.history.add(HistoryEntry(self.equation, str(result)))
		except Exception as e:
			speech.speak(f"Error: {e}")
			self.equation = ""
