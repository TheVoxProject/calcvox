XIncludeFile "Eval.pbi"
EnableExplicit

Enumeration Windows
	#WndMain
EndEnumeration

Enumeration Gadgets
	#PnlMain
	
	#TxtEquation
	#StrEquation
	#TxtResult
	#EdtResult
	
	#TxtInBinFormat
	#CmbInBinFormat
	#TxtOutBinFormat
	#CmbOutBinFormat
	
	#TxtInCurrency
	#CmbInCurrency
	#TxtOutCurrency
	#CmbOutCurrency
	
	#TxtInUnit
	#CmbInUnit
	#TxtOutUnit
	#CmbOutUnit
	
	#BtnSolve
EndEnumeration

Enumeration Shortcuts
	#ShrtEscape
EndEnumeration

Procedure MenuEvents()
	Select EventMenu()
		Case #ShrtEscape
			PostEvent(#PB_Event_CloseWindow)
	EndSelect
EndProcedure

OpenWindow(#WndMain, #PB_Ignore, #PB_Ignore, 600, 480, "Calcvox")
PanelGadget(#PnlMain, 0, 0, 300, 200)
AddGadgetItem(#PnlMain, -1, "Scientific math")
TextGadget(#TxtEquation, 15, 15, 20, 5, "Equation")
StringGadget(#StrEquation, 15, 25, 80, 20, "")
TextGadget(#TxtResult, 150, 15, 20, 5, "Result")
EditorGadget(#EdtResult, 150, 25, 200, 100, #PB_Editor_ReadOnly)
AddGadgetItem(#PnlMain, -1, "Binary conversion")
AddGadgetItem(#PnlMain, -1, "Currency conversion")
AddGadgetItem(#PnlMain, -1, "Unit conversion")
CloseGadgetList()
ButtonGadget(#BtnSolve, 15, 430, 30, 30, "Solve")
AddKeyboardShortcut(#WndMain, #PB_Shortcut_Escape, #ShrtEscape)
BindEvent(#PB_Event_Menu, @MenuEvents())
SetActiveGadget(#PnlMain)

Repeat : Until WaitWindowEvent(1) = #PB_Event_CloseWindow
