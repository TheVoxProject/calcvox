XIncludeFile "Eval.pbi"
EnableExplicit

Enumeration Modes
	#Mode_ScientificMath
	#Mode_UnitConversion
	#Mode_BinaryConversion
	#Mode_CurrencyConversion
EndEnumeration

Enumeration Gadgets
	#Txt_Modes
	#Cmb_Modes
EndEnumeration

Procedure$ ModeToString(Mode)
	Protected Res$
	Select Mode
		Case #Mode_BinaryConversion: Res$ = "Binary/hex conversion"
		Case #Mode_CurrencyConversion: Res$ = "Currency conversion"
		Case #Mode_ScientificMath: Res$ = "Scientific math"
		Case #Mode_UnitConversion: Res$ = "Unit conversion"
		Default: Res$ = "Unknown mode"
	EndSelect
	ProcedureReturn Res$
EndProcedure

Macro AddMode(_mode): AddGadgetItem(#Cmb_Modes, -1, ModeToString(_mode)): EndMacro

OpenWindow(0, 0, 0, 600, 480, "Calcvox", #PB_Window_SystemMenu | #PB_Window_MinimizeGadget | #PB_Window_MaximizeGadget)
TextGadget(#Txt_Modes, 5, 5, 20, 5, "Mode:")
ComboBoxGadget(#Cmb_Modes, 5, 15, 100, 20)
AddMode(#Mode_ScientificMath)
AddMode(#Mode_BinaryConversion)
AddMode(#Mode_CurrencyConversion)
AddMode(#Mode_UnitConversion)
SetGadgetState(#Cmb_Modes, 0)
SetActiveGadget(#Cmb_Modes)

Repeat: Until WaitWindowEvent(1) = #PB_Event_CloseWindow
