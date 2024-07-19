; German forum: http://www.purebasic.fr/german/archive/viewtopic.php?t=2073&highlight=
; Author: freedimension
; Date: 23. August 2003
; OS: Windows
; Demo: Yes

#EVAL_NUMS = "0123456789"  ;Die Zahlen von 0 bis 9 
#EVAL_GENAUGK = 65         ;Genauigkeit für StrF() - 65 sollte eigentlich genügen, mehr bringt sowieso nichts 
#EVAL_DWARF = 0.00001      ;Alles was kleiner als dieser Wert ist wird wissenschaftlich notiert 
#EVAL_GIANT = 1000000      ;Alles was größer-gleich ist wird wissenschaftlich notiert 

Procedure.s MyEval(expr.s) ; wird nur intern von Funktion Eval() aufgerufen 
  Protected exprLeft.s, exprRight.s, exprMid.s, Result.s, exprLen.l, valLeft.f, valRight.f, i.l 
  EVAL_START: 
  exprLen = Len(expr) 
  For i = 1 To exprLen ; In dieser Schleife schauen wir nach der ersten öffnenden Klammer 
    exprMid = Mid(expr, i, 1) 
    If exprMid = "(" 
      BracketCount = 1 
      start = i 
      For i = i+1 To exprLen ; wurde eine öffnende Klammer gefunden sucht diese Schleife die dazu passende schließende 
        exprMid = Mid(expr, i, 1) 
        If exprMid = "(" ;Sch... , noch eine Klammerebene 
          BracketCount + 1 
        ElseIf exprMid = ")" 
          BracketCount - 1 
          If BracketCount = 0 ; gefunden 
            exprLeft = Left(expr, start-1) 
            exprRight = Right(expr, exprLen - i) 
            exprMid = Mid(expr, start+1, exprLen - Len(exprLeft + exprRight) - 2) 
            exprMid = MyEval(exprMid) ; berechnen des mittleren Abschnitts (der in der Klammer) 
            If exprMid="x" 
              ProcedureReturn "x" 
            EndIf 
            expr = exprLeft + exprMid + exprRight 
            Goto EVAL_START   ;des Goto musste mal sein, ich brauch's ja sonst nie 
          EndIf 
        EndIf 
      Next 
    EndIf 
  Next 
  
  For i = exprLen To 1 Step -1 
    exprMid = Mid(expr, i, 1) 
    Select exprMid 
      Case "=" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        If exprLeft = exprRight 
          ProcedureReturn "1" 
        Else 
          ProcedureReturn "0.0" 
        EndIf 
      Case "<" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        If ValF(exprLeft) < ValF(exprRight) 
          ProcedureReturn "1" 
        Else 
          ProcedureReturn "0.0" 
        EndIf 
      Case ">" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        If ValF(exprLeft) > ValF(exprRight) 
          ProcedureReturn "1" 
        Else 
          ProcedureReturn "0.0" 
        EndIf 
    EndSelect 
  Next 
  
  
  For i = exprLen To 1 Step -1 
    exprMid = Mid(expr, i, 1) 
    Select exprMid 
      Case "&" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        intL.l = Val(exprLeft) 
        intR.l = Val(exprRight) 
        intR = intR & intL 
        Result = StrF(intR, #EVAL_GENAUGK) 
        ProcedureReturn Result 
      Case "|" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        intL.l = Val(exprLeft) 
        intR.l = Val(exprRight) 
        intR = intR | intL 
        Result = StrF(intR, #EVAL_GENAUGK) 
        ProcedureReturn Result 
    EndSelect 
  Next 
  
  For i = exprLen To 1 Step -1 ; + und - Terme ausrechnen 
    exprMid = Mid(expr, i, 1) 
    Select exprMid 
      Case "+" 
        exprLeft  = Left(expr, i-1) 
        ; statt Addition könnte es auch einfach nur ein Vorzeichen sein. Das wollen wir natürlich nicht berechnen. 
        If exprLeft<>"" And FindString(#EVAL_NUMS ,Right(exprLeft, 1), 1) 
          exprLeft = MyEval(exprLeft) 
          exprRight = MyEval(Right(expr, exprLen - i)) 
          If exprLeft="x" Or exprRight="x" 
            ProcedureReturn "x" 
          EndIf 
          Result = StrF(ValF(exprLeft) + ValF(exprRight), #EVAL_GENAUGK) 
          ProcedureReturn Result 
        EndIf 
      Case "-" 
        exprLeft  = Left(expr, i-1) 
        ; ditto für Subtraktion 
        If exprLeft<>"" And FindString(#EVAL_NUMS ,Right(exprLeft, 1), 1) 
          exprLeft = MyEval(exprLeft) 
          exprRight = MyEval(Right(expr, exprLen - i)) 
          If exprLeft="x" Or exprRight="x" 
            ProcedureReturn "x" 
          EndIf 
          Result = StrF(ValF(exprLeft) - ValF(exprRight), #EVAL_GENAUGK) 
          ProcedureReturn Result 
        EndIf 
    EndSelect 
  Next 
  For i = exprLen To 1 Step -1 ; Malnehmen, Teilen und Modulo (Restwertbildung) 
    exprMid = Mid(expr, i, 1) 
    Select exprMid 
      Case "*" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        Result = StrF(ValF(exprLeft) * ValF(exprRight), #EVAL_GENAUGK) 
        ProcedureReturn Result 
      Case "/" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        Result = StrF(ValF(exprLeft) / ValF(exprRight), #EVAL_GENAUGK) 
        ProcedureReturn Result 
      Case "%" 
        exprLeft  = MyEval(Left(expr, i-1)) 
        exprRight = MyEval(Right(expr, exprLen - i)) 
        If exprLeft="x" Or exprRight="x" 
          ProcedureReturn "x" 
        EndIf 
        valLeft = ValF(exprLeft) 
        valRight = ValF(exprRight) 
        Result = StrF(valLeft-Int(valLeft/valRight)*valRight, #EVAL_GENAUGK) 
        ProcedureReturn Result 
    EndSelect 
  Next 
  
  For i = exprLen To 1 Step -1 ; Potentes Ding 
    exprMid = Mid(expr, i, 1) 
    If exprMid = "^" 
      exprLeft  = MyEval(Left(expr, i-1)) 
      exprRight = MyEval(Right(expr, exprLen - i)) 
      If exprLeft="x" Or exprRight="x" 
        ProcedureReturn "x" 
      EndIf 
      valLeft = ValF(exprLeft) 
      valRight = ValF(exprRight) 
      Result = StrF(Pow(valLeft, valRight), #EVAL_GENAUGK) 
      ProcedureReturn Result 
    EndIf 
  Next 
  
  exprLeft = Left(expr, 1) 
  If exprLeft = "+" Or exprLeft = "-" 
    vz.s = exprLeft 
    expr = Right(expr, exprLen-1) 
    exprLen = Len(expr) 
  Else 
    vz = "" 
  EndIf 
  
  ;und nun bis zum Schluss noch einige nützliche Funktionen 
  exprLeft = Left(expr, 4) 
  Select exprLeft 
    Case "ASIN" 
      exprRight = Right(expr, exprLen - 4) 
      valRight = ValF(exprRight) 
      Result = StrF(ASin(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "ACOS" 
      exprRight = Right(expr, exprLen - 4) 
      valRight = ValF(exprRight) 
      Result = StrF(ACos(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "ATAN" 
      exprRight = Right(expr, exprLen - 4) 
      valRight = ValF(exprRight) 
      Result = StrF(ATan(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
  EndSelect 
  
  exprLeft = Left(expr, 3) 
  Select exprLeft 
    ;  Select Left(expr, 3) ; Merkwürdiger Bug wenn es direkt so aufgerufen wird !!!!!!!!!!!!!!!!!!!!!! 
    Case "SQR" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      If valRight <0 
        ProcedureReturn "x" 
      EndIf 
      Result = StrF(Sqr(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "LOG" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      If valRight <0 
        ProcedureReturn "x" 
      EndIf 
      Result = StrF(Log10(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "SIN" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      Result = StrF(Sin(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "COS" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      Result = StrF(Cos(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "TAN" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      Result = StrF(Tan(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "ABS" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      Result = StrF(Abs(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
    Case "INV" 
      exprRight = Right(expr, exprLen - 3) 
      valRight = ValF(exprRight) 
      If valRight =0 
        ProcedureReturn "x" 
      EndIf 
      Result = StrF(1/valRight, #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
  EndSelect 
  
  exprLeft = Left(expr, 2) 
  Select exprLeft 
    Case "LN" 
      exprRight = Right(expr, exprLen - 2) 
      valRight = ValF(exprRight) 
      If valRight <0 
        ProcedureReturn "x" 
      EndIf 
      Result = StrF(Log(valRight), #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
  EndSelect 
  
  exprLeft = Left(expr, 1) 
  Select exprLeft 
    Case "!" 
      exprRight = Right(expr, exprLen - 1) 
      valRight = ValF(exprRight) 
      If Int(valRight)>=0 
        valLeft = 1 
        For i = 2 To Int(valRight) 
          valLeft * i 
        Next 
      Else 
        ProcedureReturn "x" 
      EndIf 
      Result = StrF(valLeft, #EVAL_GENAUGK) 
      ProcedureReturn vz + Result 
  EndSelect 
  
  ProcedureReturn vz + StrF(ValF(expr), #EVAL_GENAUGK) 
EndProcedure 
Procedure.s Eval(expr.s) ;Pre- und Postprocessor, eigentliches Rechnen in MyEval() 
  Shared Eval_LastResult.s 
  Shared Eval_Memory.s, Eval_Memory1.s, Eval_Memory2.s, Eval_Memory3.s, Eval_Memory4.s, Eval_Memory5.s, Eval_Memory6.s, Eval_Memory7.s, Eval_Memory8.s, Eval_Memory9.s 
  expr = UCase(expr) ; Alles Großbuchstaben, so haben wir's beim Parsen einfacher 
  expr = ReplaceString(expr, " ", "") ; Leerzeichen entfernen 
  If Left(expr, 1) = ":" 
    Select expr 
      Case ":MEM" 
        Eval_Memory = Eval_LastResult 
        expr = Eval_Memory 
        prependix.s = "M+ " 
      Case ":MEM1" 
        Eval_Memory1 = Eval_LastResult 
        expr = Eval_Memory1 
        prependix = "M1+" 
      Case ":MEM2" 
        Eval_Memory2 = Eval_LastResult 
        expr = Eval_Memory2 
        prependix = "M2+" 
      Case ":MEM3" 
        Eval_Memory3 = Eval_LastResult 
        expr = Eval_Memory3 
        prependix = "M3+" 
      Case ":MEM4" 
        Eval_Memory4 = Eval_LastResult 
        expr = Eval_Memory4 
        prependix = "M4+" 
      Case ":MEM5" 
        Eval_Memory5 = Eval_LastResult 
        expr = Eval_Memory5 
        prependix = "M5+" 
      Case ":MEM6" 
        Eval_Memory6 = Eval_LastResult 
        expr = Eval_Memory6 
        prependix = "M6+" 
      Case ":MEM7" 
        Eval_Memory7 = Eval_LastResult 
        expr = Eval_Memory7 
        prependix = "M7+" 
      Case ":MEM8" 
        Eval_Memory8 = Eval_LastResult 
        expr = Eval_Memory8 
        prependix = "M8+" 
      Case ":MEM9" 
        Eval_Memory9 = Eval_LastResult 
        expr = Eval_Memory9 
        prependix = "M9+" 
    EndSelect 
  EndIf 
  
  ; Mathematische Konstanten ersetzen 
  expr = ReplaceString(expr, "PI", "3.1415926535897932384626433832795") ; Kreiskonstante PI 
  expr = ReplaceString(expr, "EUL", "2.7182818284590452353602874713527") ; Eulersche Zahl 
  
  ; Ergebnisspeicher 
  expr = ReplaceString(expr, "MEM1", Eval_Memory1) 
  expr = ReplaceString(expr, "MEM2", Eval_Memory2) 
  expr = ReplaceString(expr, "MEM3", Eval_Memory3) 
  expr = ReplaceString(expr, "MEM4", Eval_Memory4) 
  expr = ReplaceString(expr, "MEM5", Eval_Memory5) 
  expr = ReplaceString(expr, "MEM6", Eval_Memory6) 
  expr = ReplaceString(expr, "MEM7", Eval_Memory7) 
  expr = ReplaceString(expr, "MEM8", Eval_Memory8) 
  expr = ReplaceString(expr, "MEM9", Eval_Memory9) 
  expr = ReplaceString(expr, "MEM", Eval_Memory) 
  
  expr = ReplaceString(expr, "LR", Eval_LastResult) ; Das letzte Ergebnis 
  
  expr = ReplaceString(expr, ",", ".") ; Dezimalpunkt statt Komma 
  expr = ReplaceString(expr, ":", "/") ; Beide Notationen für Teilen zulassen 
  
  ; Wissenschaftliche Zahlennotation zulassen 
  Repeat 
    posE = FindString(expr, "E", 1) 
    If posE > 0 And FindString("+-", Mid(expr, posE+1, 1), 1) 
      posMant = posE - 1 
      While posMant>0 And FindString(#EVAL_NUMS+".", Mid(expr, posMant, 1), 1) 
        posMant - 1 
      Wend 
      posExp = posE + 2 
      While posExp <= Len(expr) And FindString(#EVAL_NUMS+".", Mid(expr, posExp, 1), 1) 
        posExp + 1 
      Wend 
      prt1.s = Left(expr, posMant) 
      prt2.s = Mid(expr,posMant+1, posE - posMant -1) 
      prt3.s = Mid(expr, posE + 1, posExp - posE -1) 
      prt4.s = Right(expr, Len(expr)-posExp+1) 
      expr = prt1 + "(" + prt2 + "e" + prt3 + ")" + prt4 
    EndIf 
  Until posE = 0 
  expr = ReplaceString(expr, "e+", "*10^") 
  expr = ReplaceString(expr, "e-", "*10^-") 
  
  expr = MyEval(expr.s) ; Jetzt lassen wir richtig rechnen 
  If expr = "x" 
    expr = "ERROR" 
  Else 
    Eval_LastResult = expr ; und speichern das Ergebniss zur späteren Verwendung 
    ; Überschüssige Nullen (und evtl. auch Dezimalpunkt) am Ende entfernen 
    exprLen = Len(expr) 
    While Mid(expr, exprLen, 1) = "0" 
      exprLen - 1 
    Wend 
    expr = Left(expr, exprLen) 
    If Right(expr, 1) = "." 
      expr = Left(expr, exprLen-1) 
    EndIf 
    If expr = "" 
      expr = "0" 
    EndIf 
    
    ; Bei kleinen und großen Zahlen die Wissenschaftliche Notation verwenden 
    If expr <> "0" And Abs(ValF(expr)) < #EVAL_DWARF 
      ;  If Abs(ValF(expr)) < #EVAL_DWARF And expr <> "0" ; auch hier ein Bug durch leicht anderen Aufruf !!!!!!!!!!!!!! 
      exprLen = Len(expr) 
      If Left(expr, 1) = "-" 
        vz.s = "-" 
        expr = Right(expr, exprLen - 3) 
      Else 
        vz = "" 
        expr = Right(expr, exprLen - 2) 
      EndIf 
      exp = 1 
      While Left(expr, 1) = "0" 
        expr = Right(expr, Len(expr)-1) 
        exp + 1 
      Wend 
      expr = vz + Left(expr, 1) + "." + Right(expr, Len(expr)-1) + "E-" + Str(exp) 
    EndIf 
    If Abs(ValF(expr)) >= #EVAL_GIANT 
      exprLen = Len(expr) 
      If Left(expr, 1) = "-" 
        vz.s = "-" 
        expr = Right(expr, exprLen - 1) 
      Else 
        vz = "" 
      EndIf 
      expr = vz + Left(expr, 1) + "." + Right(expr, Len(expr)-1) + "E+" + Str(Len(expr)-1) 
    EndIf 
    expr = prependix + expr 
    prependix = "" 
  EndIf 
  ProcedureReturn expr 
EndProcedure 

; IDE Options = PureBasic v4.00 (Windows - x86)
; Folding = -
