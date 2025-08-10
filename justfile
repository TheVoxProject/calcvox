build:
  uv run nuitka --standalone --windows-console-mode=disable --user-package-configuration-file=speechlight.nuitka-package.config.yml --output-dir=dist --output-filename=calcvox main.py
