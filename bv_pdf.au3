#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <EditConstants.au3>
#include <Constants.au3>
#Include <File.au3>
#include <ListViewConstants.au3>
#include <GuiListView.au3>

$sDirINI = @ScriptDir & "\bv_pdf.ini"
$sDir_pdftk = IniRead($sDirINI, "Diretorios", "pdftk", @ScriptDir & "\pdftk\")
$sDir_tmp = @ScriptDir & "\_tmp\"
$sBVexe = "C:\Program Files\Bentley\Bentley View CONNECT Edition\BentleyView\"

Global $gaDropFiles[1]

Const $gui_x = 500, $gui_y =600
Dim $arq_drive, $arq_dir, $arquivo, $arq_ext

$g_princ = GUICreate("Cria PDFs com Bentley View", $gui_x, $gui_y,-1,-1,-1,$WS_EX_ACCEPTFILES)  ; Cria a caixa de dialogo

$b_exec = GUICtrlCreateButton("Executa", 10, 20, 100)
;$b_para = GUICtrlCreateButton("Para", 120, 20, 100)
$c_mono = GUICtrlCreateCheckbox("Colorido", 130, 22)

$b_remove = GUICtrlCreateButton("Remove", $gui_x - 120, 20, 100)

$lv_lista_arq = GUICtrlCreateListView("Ficha                                   |Caminho                             ", 10, 60, $gui_x - 20, $gui_y - 185, BitOR($LVS_SHOWSELALWAYS, $LVS_SINGLESEL, $LVS_EDITLABELS, $LVS_NOSORTHEADER))
GUICtrlSetState(-1, $GUI_DROPACCEPTED)

GUICtrlCreateLabel ("Destino:", 15, $gui_y - 110)

$status = GUICtrlCreateLabel ("Parado", 250, 20,100)

$e_status = GUICtrlCreateEdit("", 10, $gui_y - 95, $gui_x - 20, 85, 0)

GUISetState(@SW_SHOW,$g_princ)

GUIRegisterMsg($WM_DROPFILES, "WM_DROPFILES_FUNC")


 While 1
   $msg = GUIGetMsg(1)
   Select
	  Case $msg[0] = $GUI_EVENT_CLOSE And $msg[1] = $g_princ

		  ExitLoop

	  Case $msg[0] = $b_exec
		 GUICtrlSetData ($status, "Executando...")

		 ; Create dictionary object
		 $dic_pdf = ObjCreate("Scripting.Dictionary")

		 For $iIndex = 0 to _GUICtrlListView_GetItemCount($lv_lista_arq) - 1 ; Index starts at 0 so we need to reduce by 1
			 $arq_mae = _GUICtrlListView_GetItemText($lv_lista_arq, $iIndex, 0)
			 If $dic_pdf.Exists($arq_mae) Then
				 $lista = $dic_pdf.Item($arq_mae)
				 _ArrayAdd($lista, _GUICtrlListView_GetItemText($lv_lista_arq, $iIndex, 1))
				 $dic_pdf.Item($arq_mae) = $lista
			  Else
				 Local $lista[1]
				  $lista[0] = _GUICtrlListView_GetItemText($lv_lista_arq, $iIndex, 1)
				  $dic_pdf.Add($arq_mae, $lista)
			 EndIf
		  Next

		 ; Add keys into an array
		  $aKeys = $dic_pdf.Keys
		  ;_ArrayDisplay($aKeys, "arquivos PDF")

		  $hcom_bv = FileOpen($sDir_tmp & "com_bv.txt", 2)
		  $hcom_pdf = FileOpen($sDir_tmp & "com_pdf.bat", 2)
		  FileWrite($hcom_pdf, '@echo off' & @CRLF)


		  ; Display keys in the array
		  For $i = 0 To $dic_pdf.Count -1
			   $arq_mae = $aKeys[$i]
			  $lista = $dic_pdf.Item($arq_mae)
			  ;_ArrayDisplay($lista, "arquivos filhos de " & $aKeys[$i])
			  $s_lista = ""
			For $j = 0 To UBound($lista)-1
			   ;$s_lista = $s_lista & '"' & $lista[$j] & '" '
			   _PathSplit($lista[$j], $arq_drive, $arq_dir, $arquivo, $arq_ext)
			   FileWrite($hcom_bv,"RD=" & $lista[$j] & @CRLF)
			   FileWrite($hcom_bv,"PRINT DRIVER pdf.pltcfg" & @CRLF)
			   FileWrite($hcom_bv,"PRINT PAPERNAME ISO A1" & @CRLF)
			   FileWrite($hcom_bv,"PRINT ORIENTATION LANDSCAPE" & @CRLF)

			   If Not _IsChecked($c_mono) Then
				   FileWrite($hcom_bv,"PRINT COLORMODE MONOCHROME" & @CRLF)
			   Else
				   FileWrite($hcom_bv,"PRINT COLORMODE COLOR" & @CRLF)
				EndIf

			   FileWrite($hcom_bv,"PRINT AUTOCENTER ON" & @CRLF)
			   FileWrite($hcom_bv,"PRINT BOUNDARY FIT ALL" & @CRLF)
			   FileWrite($hcom_bv,"PRINT EXECUTE " & $sDir_tmp & $arquivo & ".pdf" & @CRLF)

			   $s_lista = $s_lista & '"' & $sDir_tmp & $arquivo & ".pdf" & '" '
			Next
			$sSaida = GUICtrlRead ($e_status) & $arq_mae & '.pdf'
			FileWrite($hcom_pdf, @ScriptDir & "\pdftk\pdftk " & $s_lista &'cat output ' &'"' & $sSaida & '" dont_ask' & @CRLF)

			  ;Local $iPID = Run(@ComSpec & ' /C ' & 'pdftk '& $s_lista &'cat output ' &'"' & $sSaida & '" dont_ask',$sDir_pdftk, @SW_HIDE, $STDERR_CHILD + $STDOUT_CHILD)
			   ;While ProcessExists($iPID)
			   ;WEnd

		  Next
		  FileWrite($hcom_bv,"EXIT" & @CRLF)
		  FileClose($hcom_bv)
		  FileClose($hcom_pdf)

		  Local $iPID = Run(@ComSpec & ' /C ' & 'BentleyView.exe -s"' & $sDir_tmp & 'com_bv.txt" "' & @ScriptDir & '\branco.dgn"', $sBVexe, @SW_HIDE, $STDERR_CHILD + $STDOUT_CHILD)
			While ProcessExists($iPID)
			WEnd

			$iPID = Run(@ComSpec & ' /C ' & $sDir_tmp & "com_pdf.bat", $sDir_tmp , @SW_HIDE, $STDERR_CHILD + $STDOUT_CHILD)
			While ProcessExists($iPID)
			WEnd


		  GUICtrlSetData ($status, "Parado")



	  Case $msg[0] = $b_remove
		  _GUICtrlListView_DeleteItemsSelected ($lv_lista_arq)


	  Case $msg[0] = $GUI_EVENT_DROPPED
		  For $i = 0 To UBound($gaDropFiles) - 1

			 _PathSplit($gaDropFiles[$i], $arq_drive, $arq_dir, $arquivo, $arq_ext)
			 ;If $arq_ext = ".dgn" Then

				$a_pedaco_arq = StringSplit($arquivo,"_")
				 $num_pedacos = $a_pedaco_arq[0]
				 $s_subpasta = $a_pedaco_arq[1]
				 if $num_pedacos > 2 Then
					 for $j = 2 to ($num_pedacos - 1)
						 $s_subpasta = $s_subpasta & "_" & $a_pedaco_arq[$j]
					 Next
				 EndIf

				GUICtrlCreateListViewItem($s_subpasta & "|"& $gaDropFiles[$i], $lv_lista_arq)

				GUICtrlSetData ($e_status, $arq_drive & $arq_dir)
			 ;EndIf

		  Next

	  EndSelect

	WEnd
GUIDelete($g_princ)

Func WM_DROPFILES_FUNC($hWnd, $msgID, $wParam, $lParam)
    Local $nSize, $pFileName
    Local $nAmt = DllCall("shell32.dll", "int", "DragQueryFile", "hwnd", $wParam, "int", 0xFFFFFFFF, "ptr", 0, "int", 255)
    For $i = 0 To $nAmt[0] - 1
        $nSize = DllCall("shell32.dll", "int", "DragQueryFile", "hwnd", $wParam, "int", $i, "ptr", 0, "int", 0)
        $nSize = $nSize[0] + 1
        $pFileName = DllStructCreate("char[" & $nSize & "]")
        DllCall("shell32.dll", "int", "DragQueryFile", "hwnd", $wParam, "int", $i, "ptr", DllStructGetPtr($pFileName), "int", $nSize)
        ReDim $gaDropFiles[$i + 1]
        $gaDropFiles[$i] = DllStructGetData($pFileName, 1)
        $pFileName = 0
    Next
EndFunc   ;==>WM_DROPFILES_FUNC

Func _IsChecked($idControlID)
    Return BitAND(GUICtrlRead($idControlID), $GUI_CHECKED) = $GUI_CHECKED
EndFunc   ;==>_IsChecked
