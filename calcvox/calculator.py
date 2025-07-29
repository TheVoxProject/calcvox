from dataclasses import dataclass

from calcvox import speech


@dataclass
class Calculator:
	equation: str = ""

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
