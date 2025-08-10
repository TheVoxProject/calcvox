import math
from dataclasses import dataclass, field

from asteval import Interpreter
from speechlight import speech

from calcvox.history import History, HistoryEntry

# Grab all public names from math module
safe_math_names = {name: obj for name, obj in math.__dict__.items() if not name.startswith("__")}
aeval = Interpreter(symtable=safe_math_names, use_numpy=False)


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
			result = aeval(self.equation)
			if result is None:
				raise ValueError("No result")
			speech.say(f"Equals {result}")
			self.equation = str(result)
			self.history.add(HistoryEntry(self.equation, str(result)))
		except Exception as e:
			speech.say(f"Error: {e}")
			self.equation = ""
