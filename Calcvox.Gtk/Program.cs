using System;
using Eto.Forms;

namespace Calcvox.Gtk {
	class Program {
		[STAThread]
		public static void Main(string[] args) {
			new Application(Eto.Platforms.Gtk).Run(new MainForm());
		}
	}
}
