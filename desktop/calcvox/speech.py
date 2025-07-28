from accessible_output2 import outputs
from accessible_output2.outputs.base import Output

_output: Output | None = None


def speak(text: str, interrupt: bool = True, prefer_tts: bool = False) -> bool:
	global _output
	if _output is None:
		_output = outputs.auto.Auto()
	if _output:
		_output.output(text, interrupt=interrupt)
		return True
	return False
