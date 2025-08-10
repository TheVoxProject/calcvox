from dataclasses import dataclass, field

from speechlight import speech
from sympy import sympify

from calcvox.history import History, HistoryEntry


@dataclass
class Calculator:
	equation: str = ""
	history: History = field(default_factory=History)

	def backspace(self) -> None:
		if self.equation == "":
			speech.say("Blank")
			return
		removed = self.equation[-1]
		self.equation = self.equation[:-1]
		speech.say(removed)

	def clear(self) -> None:
		if self.equation == "":
			speech.say("Blank")
			return
		self.equation = ""
		speech.say("Cleared")

	def evaluate(self) -> None:
		try:
			if self.equation == "":
				return
			result = sympify(self.equation).evalf()
			speech.say(f"Equals {result}")
			self.equation = str(result)
			self.history.add(HistoryEntry(self.equation, str(result)))
		except Exception as e:
			speech.say(f"Error: {e}")
			self.equation = ""
