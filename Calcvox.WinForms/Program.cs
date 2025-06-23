using System;
using Eto.Forms;

namespace Calcvox.WinForms {
	class Program {
		[STAThread]
		public static void Main(string[] args) {
			new Application(Eto.Platforms.WinForms).Run(new MainForm());
		}
	}
}
