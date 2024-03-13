#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <EditConstants.au3>
#include <Constants.au3>
#Include <File.au3>
#include <ListViewConstants.au3>
#include <GuiListView.au3>
#include <GuiTreeView.au3>
#include <Excel.au3>
#include <Date.au3>

#include "wd_core.au3"
#include "wd_helper.au3"

$_WD_DEBUG = $_WD_DEBUG_None ; You could also use $_WD_DEBUG_Error

_WD_Option('Driver', @ScriptDir & '\msedgedriver.exe')
_WD_Option('Port', 9515)
_WD_Option('DriverParams', '--log-path="' & @ScriptDir & '\edge.log"')

$sDesiredCapabilities = '{"capabilities": {"alwaysMatch": {"ms:edgeOptions": {"binary": "' & StringReplace (@ProgramFilesDir, "\", "/") & '/Microsoft/Edge/Application/msedge.exe", "excludeSwitches": [ "enable-automation"], "useAutomationExtension": false}}}}'

_WD_Startup()

If @error <> $_WD_ERROR_Success Then
	MsgBox($MB_ICONWARNING, "ERRO", "Falha ao abrir o Edge")
	Exit -1
EndIf

$sSession = _WD_CreateSession($sDesiredCapabilities)

If @error <> $_WD_ERROR_Success Then
	MsgBox($MB_ICONWARNING, "ERRO", "Falha ao abrir o Edge")
	Exit -1
EndIf

_WD_Window($sSession, 'rect', '{"x":375,"y":5,"width":1000,"height":650}')

Global $tab_principal = '{"handle":"' & _WD_Window($sSession, "window") & '"}'
;_WD_Window($sSession, "switch", $tab_principal)
_WD_Navigate($sSession, "http://gedex/Inbox/Pessoal")

Global $acoes[][2] = [["Pesquisa", True], _
			   ["Revisa", False], _
			   ["Carrega", True], _
			   ["Submete", False], _
			   ["Aprova", True]]

Enum $AC_PESQ, _
	  $AC_REV, _
	  $AC_CARR, _
	  $AC_SUBM, _
	  $AC_APROV

Enum $SUB_ABRE_FICHA, _
	  $SUB_FECHA_FICHA, _
	  $SUB_PEGA_FICHA, _
	  $SUB_ESPERA_FICHA, _
	  $SUB_ABRE_ARQ, _
	  $SUB_CARR_ARQ, _
	  $SUB_ESPERA_ARQ, _
	  $SUB_ESPERA_ARQ_F, _
	  $SUB_ABRE_APROV, _
	  $SUB_ESPERA_APROV, _
	  $SUB_ESPERA_APROV_F, _
	  $SUB_ABRE_SUBM, _
	  $SUB_ESPERA_SUBM, _
	  $SUB_ABRE_ASSUME, _
	  $SUB_ESPERA_ASSUME, _
	  $SUB_PESQUISA, _
	  $SUB_ESPERA_PESQ, _
	  $SUB_VER_FICHA_REV, _
	  $SUB_VER_FICHA_APROV, _
	  $SUB_REVISA_FICHA, _
	  $SUB_ESPERA_REV_I, _
	  $SUB_PREENCHE_REV, _
	  $SUB_ESPERA_REV_F, _
	  $SUB_REL_PESQ, _
	  $SUB_REL_REV, _
	  $SUB_REL_CARR, _
	  $SUB_REL_SUB, _
	  $SUB_REL_APROV, _
	  $SUB_ABRE_WORKFLOW, _
	  $SUB_PREENCHE_WORKFLOW, _
	  $SUB_ESPERA_WORKFLOW, _
	  $SUB_NADA

Global $subs[] = ["sub_abre_ficha", _
	  "sub_fecha_ficha", _
	  "sub_pega_ficha", _
	  "sub_espera_ficha", _
	  "sub_abre_arq", _
	  "sub_carr_arq", _
	  "sub_espera_arq", _
	  "sub_espera_arq_f", _
	  "sub_abre_aprov", _
	  "sub_espera_aprov", _
	  "sub_espera_aprov_f", _
	  "sub_abre_subm", _
	  "sub_espera_subm", _
	  "sub_abre_assume", _
	  "sub_espera_assume", _
	  "sub_pesquisa", _
	  "sub_espera_pesq", _
	  "sub_ver_ficha_rev", _
	  "sub_ver_ficha_aprov", _
	  "sub_revisa_ficha", _
	  "sub_espera_rev_i", _
	  "sub_preenche_rev", _
	  "sub_espera_rev_f", _
	  "sub_rel_pesq", _
	  "sub_rel_rev", _
	  "sub_rel_carr", _
	  "sub_rel_sub", _
	  "sub_rel_aprov", _
	  "sub_abre_workflow", _
	  "sub_preenche_workflow", _
	  "sub_espera_workflow", _
	  "sub_nada"]



Global $etp_pesq[] = [$SUB_PESQUISA, $SUB_ESPERA_PESQ, $SUB_REL_PESQ]
Global $etp_rev[] = [$SUB_ABRE_FICHA, $SUB_ESPERA_FICHA, $SUB_VER_FICHA_REV, $SUB_REVISA_FICHA, $SUB_ESPERA_REV_I, $SUB_PREENCHE_REV, $SUB_ESPERA_REV_F, $SUB_FECHA_FICHA, $SUB_NADA, $SUB_PESQUISA, $SUB_ESPERA_PESQ, $SUB_REL_REV]
Global $etp_rev_ext[] = [$SUB_ABRE_FICHA, $SUB_ESPERA_FICHA, $SUB_VER_FICHA_REV, $SUB_REVISA_FICHA, _
						$SUB_ESPERA_REV_I, $SUB_PREENCHE_REV, $SUB_ESPERA_REV_F, $SUB_FECHA_FICHA, $SUB_NADA, _
						$SUB_PESQUISA, $SUB_ESPERA_PESQ, $SUB_ABRE_FICHA, $SUB_ESPERA_FICHA, _
						$SUB_ABRE_WORKFLOW, $SUB_PREENCHE_WORKFLOW, $SUB_ESPERA_WORKFLOW, $SUB_ESPERA_FICHA, $SUB_FECHA_FICHA, $SUB_NADA, _
						$SUB_PESQUISA, $SUB_ESPERA_PESQ, $SUB_REL_REV]
Global $etp_car[] = [$SUB_ABRE_FICHA]
;Global $etp_car[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_FECHA_FICHA] ; <<<<<<<<<< teste
;Global $etp_car_arq[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_ABRE_ARQ, $SUB_ESPERA_ARQ, $SUB_CARR_ARQ, $SUB_REL_CARR, $SUB_ESPERA_FICHA]
Global $etp_car_arq[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_ABRE_ARQ, $SUB_ESPERA_ARQ, $SUB_ESPERA_ARQ_F, $SUB_REL_CARR]
;Global $etp_car_arq[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_NADA] ; <<<<<<<<<< teste
Global $etp_subm[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_ABRE_SUBM, $SUB_ESPERA_SUBM, $SUB_ESPERA_APROV_F, $SUB_ESPERA_FICHA, $SUB_ABRE_ASSUME, $SUB_ESPERA_ASSUME, $SUB_REL_SUB]
Global $etp_aprov[] = [$SUB_ESPERA_FICHA, $SUB_VER_FICHA_APROV, $SUB_ABRE_APROV, $SUB_ESPERA_APROV, $SUB_ESPERA_APROV_F, $SUB_REL_APROV, $SUB_ESPERA_FICHA, $SUB_FECHA_FICHA]
Global $etp_nada[] = [$SUB_NADA]


Global $etapas[] = [$etp_pesq, $etp_rev, $etp_car, $etp_subm, $etp_aprov]
Global $etapas_ficha[] = [$etp_pesq, $etp_rev, $etp_car, $etp_subm, $etp_aprov]
Global $etapas_arq[] = [$etp_nada, $etp_nada, $etp_car_arq, $etp_nada, $etp_nada]
Global $et = $etp_pesq

Enum $NADA, _
	  $PROC_FICHA, _
	  $PROC_ARQ, _
	  $PROX_FICHA, _
	  $PROX_ARQ, _
	  $FIN_FICHA

Enum  $INI_ETAPA, _
	  $PROC_ETAPA, _
	  $PROC_SUB, _
	  $PROX_ETAPA, _
	  $PROX_SUB

Global $passo = $NADA, $prox_passo = $NADA, $conta = 0, $limite = 100, $tempo = 500, $etapa_atual, $etapa_final, $sub_etapa, $estado_etapa, $estado_sub
Global $init = 0

Global $gaDropFiles[1]
$dic_cam = ObjCreate("Scripting.Dictionary")
Global $h_relatorio, $msg_erro = ""

Global $ficha_atual, $arq_atual

Const $gui_x = 365, $gui_y =600
Dim $arq_drive, $arq_dir, $arquivo, $arq_ext

$g_princ = GUICreate("Trago Amor", $gui_x, $gui_y,10,10,-1,$WS_EX_ACCEPTFILES)  ; Cria a caixa de dialogo

$b_remove = GUICtrlCreateButton("Remove", 5, 5, 100)
$b_limpa = GUICtrlCreateButton("Limpa", 5, 35, 100)

Global $acoes_ch[UBound($acoes)]
For $i = 0 To UBound($acoes) - 1
   $acoes_ch[$i] = GUICtrlCreateCheckbox($acoes[$i][0], 5, 70 + (20 * $i), 100, 17)
   If $acoes[$i][1] Then GUICtrlSetState($acoes_ch[$i], $GUI_CHECKED)
Next

GUICtrlCreateLabel ("Revisão:", 5, 170)
$c_rev = GUICtrlCreateCombo("Menor", 5, 190, 80)
GUICtrlSetData($c_rev, "Maior")

GUICtrlCreateLabel ("Comentarios:", 5, 220)
$e_comentarios = GUICtrlCreateEdit("", 5, 240, 100, 110, 0)

$b_exec = GUICtrlCreateButton("Executa", 5, 370, 100)
$b_para = GUICtrlCreateButton("Para", 5, 400, 100)
GUICtrlSetBkColor ($b_para, $COLOR_GREEN)
$status = GUICtrlCreateLabel ("Parado", 5, 430,100)

GUICtrlCreateLabel ("Relatorio:", 5, 460)
$e_destino = GUICtrlCreateEdit("", 5, 480, 100, 110, 0)

Local $iStyle = BitOR($TVS_EDITLABELS, $TVS_HASBUTTONS, $TVS_HASLINES, $TVS_LINESATROOT, $TVS_SHOWSELALWAYS, $TVS_CHECKBOXES)
$tv_lista_arq = GUICtrlCreateTreeView(110, 5, 250, $gui_y - 10, $iStyle, $WS_EX_CLIENTEDGE)
GUICtrlSetState(-1, $GUI_DROPACCEPTED)


GUISetState(@SW_SHOW,$g_princ)

GUIRegisterMsg($WM_DROPFILES, "WM_DROPFILES_FUNC")
GUIRegisterMsg($WM_NOTIFY, "WM_NOTIFY")

$hTimer = TimerInit() ; Begin the timer and store the handle in a variable.

While 1
   $msg = GUIGetMsg()
   Switch $msg
   Case $GUI_EVENT_CLOSE
	  FileClose($h_relatorio)
	  ExitLoop

	  Case $b_exec
		 GUICtrlSetData ($status, "Executando...")
		 GUICtrlSetBkColor ($b_para, $COLOR_RED)
		 $item_sel = _GUICtrlTreeView_GetSelection ($tv_lista_arq)
		 If $item_sel Then
			$pai = _GUICtrlTreeView_GetParentHandle ($tv_lista_arq, $item_sel)
			If $pai Then
			   $ficha_atual = $pai
			   $arq_atual = $item_sel
			   $passo = $PROC_ARQ
			   $conta = 0;
			   $init = 0
			Else
			   $ficha_atual = $item_sel
			   $passo = $PROC_FICHA
			   $conta = 0;
			   $init = 0
			EndIf

			$hTimer = TimerInit() ; Begin the timer and store the handle in a variable.
			_WD_Window($sSession, "switch", $tab_principal)

			$h_relatorio = FileOpen(GUICtrlRead ($e_destino), $FO_APPEND)
			FileWrite( $h_relatorio, @CRLF & "----------- " & _Now() & " -------------" & @CRLF)
			FileWrite( $h_relatorio, ">>>>> Comentário:  " & GUICtrlRead ($e_comentarios) & @CRLF)
			FileWrite( $h_relatorio, "----------------------------------------------------------" & @CRLF)
		 Else
			MsgBox($MB_SYSTEMMODAL, "Nada a fazer", "Selecione um item da lista para começar")
			GUICtrlSetData ($status, "Parado")
			GUICtrlSetBkColor ($b_para, $COLOR_GREEN)
		 EndIf

	  Case $b_remove
		 $item_sel = _GUICtrlTreeView_GetSelection ($tv_lista_arq)
		 If $item_sel Then
			$pai = _GUICtrlTreeView_GetParentHandle ($tv_lista_arq, $item_sel)
			_GUICtrlTreeView_Delete ($tv_lista_arq, $item_sel)
			If $pai Then
			   If _GUICtrlTreeView_GetChildCount ( $tv_lista_arq, $pai ) <= 0 Then
				  _GUICtrlTreeView_Delete ($tv_lista_arq, $pai)
			   EndIf
			EndIf
		 EndIf
	  Case $b_limpa
		   _GUICtrlTreeView_DeleteAll($tv_lista_arq)

	  Case $b_para
		 $passo = $NADA
		 $conta = 0
		 GUICtrlSetData ($status, "Parado") ; --------------------------
		 GUICtrlSetBkColor ($b_para, $COLOR_GREEN)
		 FileClose($h_relatorio)

	  Case $GUI_EVENT_DROPPED
		 For $i = 0 To UBound($gaDropFiles) - 1
			_PathSplit($gaDropFiles[$i], $arq_drive, $arq_dir, $arquivo, $arq_ext)
			GUICtrlSetData ($e_destino, $arq_drive & $arq_dir & "relatorio.txt")

			Local $aArray = StringRegExp(StringLower($arquivo), '^(\w{2})(\w{2}\d?)_(\d+)', $STR_REGEXPARRAYMATCH)
			If UBound($aArray) > 0 Then
			   $s_subpasta = $aArray[0] & "/" & $aArray[1] & "-" & $aArray[2]
			Else
			   $a_pedaco_arq = StringSplit($arquivo,"_")
			   $num_pedacos = $a_pedaco_arq[0]
			   $s_subpasta = StringLower($a_pedaco_arq[1])
			   If $num_pedacos > 2 Then
				  for $j = 2 to ($num_pedacos - 1)
					 $s_subpasta = $s_subpasta & "_" & StringLower($a_pedaco_arq[$j])
				  Next
			   EndIf
			EndIf

			$ficha_item = _GUICtrlTreeView_FindItem($tv_lista_arq, $s_subpasta)
			If $ficha_item Then

			   $arq_item = _GUICtrlTreeView_FindItem($tv_lista_arq, $arquivo & $arq_ext)
			   ;$arq_item = _GUICtrlTreeView_FindItemEx($tv_lista_arq, $s_subpasta & "\" & $arquivo & $arq_ext)

			   If $arq_item Then
				  If $dic_cam.Exists(String($arq_item)) Then
					 $dic_cam.Item(String($arq_item)) = $gaDropFiles[$i]
				  Else
					 $dic_cam.Add(String($arq_item), $gaDropFiles[$i])
				  EndIf
			   Else
				  $arq_item = _GUICtrlTreeView_AddChild ($tv_lista_arq, $ficha_item, $arquivo & $arq_ext)
				  _GUICtrlTreeView_SetStateImageIndex ($tv_lista_arq, $arq_item, 0)
				  If $dic_cam.Exists(String($arq_item)) Then
					 $dic_cam.Item(String($arq_item)) = $gaDropFiles[$i]
				  Else
					 $dic_cam.Add(String($arq_item), $gaDropFiles[$i])
				  EndIf
			   EndIf
			Else
			   $ficha_item = _GUICtrlTreeView_Add ($tv_lista_arq, 0, $s_subpasta)
			   _GUICtrlTreeView_SetChecked($tv_lista_arq, $ficha_item)
			   _GUICtrlTreeView_SetBold($tv_lista_arq, $ficha_item)

			   $arq_item = _GUICtrlTreeView_AddChild ($tv_lista_arq, $ficha_item, $arquivo & $arq_ext)
			   _GUICtrlTreeView_SetStateImageIndex ($tv_lista_arq, $arq_item, 0)

			   _GUICtrlTreeView_Expand($tv_lista_arq, $ficha_item)
			   If $dic_cam.Exists(String($arq_item)) Then
				  $dic_cam.Item(String($arq_item)) = $gaDropFiles[$i]
			   Else
				  $dic_cam.Add(String($arq_item), $gaDropFiles[$i])
			   EndIf
			EndIf
		 Next
   EndSwitch

   If $conta = 0 Then
	  Switch $passo
		 Case $PROC_FICHA
			_GUICtrlTreeView_SelectItem ($tv_lista_arq, $ficha_atual)
			$arq_atual = _GUICtrlTreeView_GetFirstChild ($tv_lista_arq, $ficha_atual)
			;MsgBox($MB_SYSTEMMODAL, "Information", "Ficha: " & _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual))
			;FileWrite( $h_relatorio, "Ficha: " & _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual))
			$conta = 1;
			$estado_etapa = $INI_ETAPA
			$etapa_final = $AC_CARR
			$prox_passo = $PROC_ARQ
		 Case $PROC_ARQ
			_GUICtrlTreeView_SelectItem ( $tv_lista_arq, $arq_atual )
			;MsgBox($MB_SYSTEMMODAL, "Information", "Arq: " & $dic_cam.Item(String($arq_atual)))
			;FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual))
			$conta = 1;
			$etapa_atual = $AC_REV
			$etapa_final = $AC_CARR
			$estado_etapa = $PROX_ETAPA
			$prox_passo = $PROX_ARQ
		 Case $PROX_ARQ
			$arq_atual = _GUICtrlTreeView_GetNextChild ($tv_lista_arq, $arq_atual)
			If $arq_atual Then
			   $passo = $PROC_ARQ
			Else
			   $passo = $FIN_FICHA
			EndIf
			$conta = 0;
		 Case $FIN_FICHA
			_GUICtrlTreeView_SelectItem ($tv_lista_arq, $ficha_atual)
			;MsgBox($MB_SYSTEMMODAL, "Information", "Ficha: " & _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual))
			;FileWrite( $h_relatorio, @TAB & @TAB & "Aprov - " & _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual))
			$conta = 1;
			$etapa_atual = $AC_CARR
			$etapa_final = $AC_APROV
			$estado_etapa = $PROX_ETAPA
			$prox_passo = $PROX_FICHA
		  Case $PROX_FICHA
			sub_fecha_ficha()
			$init = 0
			$ficha_atual = _GUICtrlTreeView_GetNextSibling($tv_lista_arq, $ficha_atual)
			If $ficha_atual Then
			   $passo = $PROC_FICHA
			   $conta = 0;
			Else
			   $passo = $NADA
			   $conta = 0
			   GUICtrlSetData ($status, "Parado") ; --------------------------
			   GUICtrlSetBkColor ($b_para, $COLOR_GREEN)
			   FileClose($h_relatorio)
			EndIf

		 Case Else
			$conta = 0
			$passo = $NADA

	  EndSwitch
	ElseIf $conta > $limite Then
		If $passo = $PROC_ARQ Then
			FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "ERRO-"& $msg_erro & @CRLF)
		Else
			FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "ERRO-"& $msg_erro & @CRLF)
		EndIf

		_WD_Window($sSession, "switch", $tab_principal)
		$passo = $PROX_FICHA
		 $conta = 0
	  ; ===============erro ===============
   Else
	  If (TimerDiff($hTimer) > $tempo) Then
		 $hTimer = TimerInit() ; restart Timer
		 ;=======================================================
		 If $estado_etapa = $INI_ETAPA Then
			$etapa_atual = 0
			$conta = 1
			If Not _IsChecked($acoes_ch[$etapa_atual]) Then
				$estado_etapa = $PROX_ETAPA
			Else
				$sub_etapa = 0
				$estado_etapa = $PROC_SUB
				$et = $etapas_ficha[$etapa_atual]
				If $passo = $PROC_ARQ Then $et = $etapas_arq[$etapa_atual]
			EndIf
		 EndIf
		 If $estado_etapa = $PROC_SUB Then
			;$et = $etapas_ficha[$etapa_atual]
			;If $passo = $PROC_ARQ Then $et = $etapas_arq[$etapa_atual]
			$ret = Call($subs[$et[$sub_etapa]])
			If $ret > 0 Then
			   ;ConsoleWrite($acoes[$etapa_atual][0] & ": " & $subs[$et[$sub_etapa]] & @CRLF)
			   $conta = 1
			   $init = 0
			   If $sub_etapa < UBound($et) - 1 Then
				  $sub_etapa += 1
				  $estado_etapa = $PROC_SUB
			   Else
				  $sub_etapa = 0
				  $estado_etapa = $PROX_ETAPA
			   EndIf
			ElseIf $ret = 0 Then
			   $conta += 1
			Else
			   $conta = $limite + 1 ; <<<<<<<<<<<<<<<<< erro
			EndIf
		 EndIf
		 If $estado_etapa = $PROX_ETAPA Then
			If $etapa_atual < $etapa_final Then
			   $etapa_atual += 1
			   $conta = 1
			   If Not _IsChecked($acoes_ch[$etapa_atual]) Then
					$estado_etapa = $PROX_ETAPA
			   Else
					$sub_etapa = 0
					$estado_etapa = $PROC_SUB
					$et = $etapas_ficha[$etapa_atual]
					If $passo = $PROC_ARQ Then $et = $etapas_arq[$etapa_atual]
			   EndIf
			Else
			   $conta = 0
			   $passo = $prox_passo
			   ;FileWrite( $h_relatorio, @TAB & "OK" & @CRLF)
			EndIf
		 EndIf

		 ;=======================================================

	  EndIf
   EndIf

WEnd

_WD_DeleteSession($sSession)
_WD_Shutdown()

GUIDelete($g_princ)

Func _IsChecked($idControlID)
    Return BitAND(GUICtrlRead($idControlID), $GUI_CHECKED) = $GUI_CHECKED
EndFunc   ;==>_IsChecked

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

Func WM_NOTIFY($hWnd, $iMsg, $wParam, $lParam)
   #forceref $hWnd, $iMsg, $wParam
   Local $hWndFrom, $iIDFrom, $iCode, $tNMHDR, $hWndTreeview
   $hWndTreeview = $tv_lista_arq
   If Not IsHWnd($tv_lista_arq) Then $hWndTreeview = GUICtrlGetHandle($tv_lista_arq)

   $tNMHDR = DllStructCreate($tagNMHDR, $lParam)
   $hWndFrom = HWnd(DllStructGetData($tNMHDR, "hWndFrom"))
   $iIDFrom = DllStructGetData($tNMHDR, "IDFrom")
   $iCode = DllStructGetData($tNMHDR, "Code")


   Switch $HwndFrom
	  Case $hWndTreeview
		 Switch $iCode
			Case $TVN_BEGINLABELEDITA, $TVN_BEGINLABELEDITW
			  Return False
			Case $TVN_ENDLABELEDITA, $TVN_ENDLABELEDITW
			  Local $tInfo = DllStructCreate($tagNMTVDISPINFO, $lParam), $sBuffer = "wchar Text[" & DllStructGetData($tInfo, "TextMax") & "]", $tBuffer, $sText
			  If Not _GUICtrlTreeView_GetUnicodeFormat($HwndFrom) Then $sBuffer = StringTrimLeft($sBuffer, 1)
			  $tBuffer = DllStructCreate($sBuffer, DllStructGetData($tInfo, "Text"))
			  $sText = DllStructGetData($tBuffer, "Text")
			   ;ConsoleWrite($sText & @CRLF)
			  If StringLen($sText) Then Return 1
		 EndSwitch
   EndSwitch

   Return $GUI_RUNDEFMSG
EndFunc

Func sub_abre_ficha()
	$msg_erro = "Ficha não localizada"

	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)
	Local $posicoes[10]
	Local $num_pos = 0

	If $init = 0 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//span[@id='widgetBoxInboxLoadingTexto']")
		If @error = $_WD_ERROR_Success Then
			Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
			If @error Then
				$IsVisible = False
			EndIf

			If $IsVisible Then
				Return 0
			EndIf
		EndIf

		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableInbox']/thead/tr[2]/th[3]/input")
		If @error = $_WD_ERROR_Success Then
			_WD_ElementAction($sSession, $sElement, 'clear')
			_WD_ElementAction($sSession, $sElement, 'value', $nome_ficha & " ")
			$init = 1
		Else
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/thead/tr[2]/th[3]/input")
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'clear')
				_WD_ElementAction($sSession, $sElement, 'value', $nome_ficha & "  ")
				$init = 1
			Else
				$init = 0
				Return -1
			EndIf
		EndIf

   Else
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableInbox']")
		If @error = $_WD_ERROR_Success Then
			$tab_id = "tableInbox"
		Else
			$tab_id = "tableResultadoPesquisa"
		EndIf

		$retorno = _WD_ExecuteScript($sSession, 'if (document.getElementById("' & $tab_id & '").rows[2].cells.length > 10) { return document.getElementById("' & $tab_id & '").rows.length - 2} else { return 0 }')
		If @error <> $_WD_ERROR_Success Then Return 0
		$data = Json_Decode ($retorno)
		$num_fichas = Json_Get ($data, '["value"]')

		If $num_fichas = 1 Then
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='" & $tab_id & "']/tbody/tr[" & $num_fichas & "]/td[3]/a/span")
			If @error = $_WD_ERROR_Success Then
				$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
				If StringCompare($ficha_texto, $nome_ficha) = 0 Then
					_WD_ElementAction($sSession, $sElement, 'click')
					$init = 0
					Return 1
				EndIf

			Else
				$init = 0
				Return -1
			EndIf


		ElseIf $num_fichas > 1 Then

			For $i = 1 to $num_fichas
				$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='" & $tab_id & "']/tbody/tr[" & $i & "]/td[3]/a/span")
				If @error = $_WD_ERROR_Success Then
					$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
					If StringCompare($ficha_texto, $nome_ficha) = 0 Then
						$num_pos += 1
						If UBound($posicoes) < $num_pos Then ReDim $posicoes[$num_pos]
						$posicoes[$num_pos-1] = $i
					EndIf
				Else
					$init = 0
					Return -1
				EndIf

			Next

			If $num_pos < 1 Then
				$init = 0
				Return -1
			EndIf

			Local $rev1, $rev2, $pos = $posicoes[0]
			If $num_pos > 1 then
				For $i = 1 to $num_pos - 1
					$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='" & $tab_id & "']/tbody/tr[" & $posicoes[$i-1] & "]/td[4]/span")
					If @error = $_WD_ERROR_Success Then
						$rev1 = _WD_ElementAction($sSession, $sElement, 'text')
					Else
						$init = 0
						Return -1
					EndIf


					$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='" & $tab_id & "']/tbody/tr[" & $posicoes[$i] & "]/td[4]/span")
					If @error = $_WD_ERROR_Success Then
						$rev2 = _WD_ElementAction($sSession, $sElement, 'text')
						If compara_rev($rev2, $rev1) = 1 Then
							$pos = $posicoes[$i]
						Else
							$pos = $posicoes[$i-1]
						EndIf
					Else
						$init = 0
						Return -1
					EndIf
				Next
			EndIf


			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='" & $tab_id & "']/tbody/tr[" & $pos & "]/td[3]/a/span")
			If @error = $_WD_ERROR_Success Then
				$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
				If StringCompare($ficha_texto, $nome_ficha) = 0 Then
					_WD_ElementAction($sSession, $sElement, 'click')
					$init = 0
					Return 1
				EndIf

			Else
				$init = 0
				Return -1
			EndIf
		Else
			$init = 0
			Return -1
		EndIf

	EndIf
	Return 0
EndFunc
Func sub_fecha_ficha()
	If WinExists ("Abrir") Then
		WinActivate("Abrir")
		Sleep (500)
		Send("{ESCAPE}")
		Sleep (100)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalArquivoX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Sleep (500)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalAprovarX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Sleep (500)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalRejeitarX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Sleep (500)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalReassignX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Sleep (500)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalWorkflowX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Sleep (500)
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalDetalhesFichaX']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'click')
		Return 1
	Else
		Return 0
	EndIf
EndFunc
Func sub_pega_ficha()
   If $conta < 2 Then
	  ConsoleWrite("Pega ficha")
	  Return 0
   Else
	  ConsoleWrite(" OK" & @CRLF)
	  Return 1
   EndIf

EndFunc
Func sub_espera_ficha()
	$msg_erro = "Visualizar ficha"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='modalDetalhesFichaCorpo']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//span[@id='modalDetalhesFichaTituloName']")
			If @error <> $_WD_ERROR_Success Then Return -1
			$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
			Local $aArray = StringRegExp($ficha_texto, '\[(.*)\]\[(.*)\]', $STR_REGEXPARRAYMATCH)
			;ConsoleWrite("ficha: " & UBound($aArray) &"  " & $aArray[0] & @CRLF)
			If StringCompare($aArray[0], $nome_ficha) = 0 Then
				Return 1
			Else
				Return -1
			EndIf
		ElseIf sub_abre_ficha() < 0 Then
			Return -1
		EndIf
	ElseIf sub_abre_ficha() < 0 Then
		Return -1

	EndIf
	Return 0

EndFunc

Func sub_abre_arq()
	$msg_erro = "Falha de carregamento"
	Local $arq = _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual)
	Static Local $pos = 0
	If $init = 0 Then
		$pos = 0
		$retorno = _WD_ExecuteScript($sSession, 'var arq="' & $arq & '"; var lista = document.getElementById("detalhesFicha").children[1].children[0].children[1].children[0].children[0].children[0].children[0].children; var pos = 0; for(i = 0, len = lista.length; i < len; i++){ if (lista[i].children[1].children[0].children[0].children[0].text.trim().toLowerCase() == arq.toLowerCase()){ pos=i+1; break;}}; return pos;')
		If @error = $_WD_ERROR_Success Then
			$data = Json_Decode ($retorno)
			$pos = Json_Get ($data, '["value"]')
		EndIf
		;//*[@id="detalhesFicha"]/div[2]/div/div[2]/div/div/div/div/div[2]/div/i
		;//*[@id="detalhesFicha"]/div[2]/div/div[2]/div/div/div/div/div[6]/div/i
		;//*[@id="detalhesFicha"]/div[2]/div/div[2]/div/div/div/div/div[' & $pos & ']/div/i
		If $pos > 0 Then
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="detalhesFicha"]/div[2]/div/div[2]/div/div/div/div/div[' & $pos & ']/div/i')
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'click')
			EndIf
		EndIf
	ElseIf $init > 1 Then
		If $pos > 0 Then
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="detalhesFicha"]/div[2]/div/div[2]/div/div/div/div/div[' & $pos & ']/div[2]/div/span/a/i')
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'click')
				Return 1
			Else
				Return -1
			EndIf
		Else
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//div[@id="detalhesFicha"]/div[2]/div/div/div/a[3]/i')
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'click')
				Return 1
			Else
				Return -1
			EndIf
		EndIf
	EndIf
	$init += 1
	Return 0

EndFunc

Func sub_carr_arq()
	$msg_erro = "Falha de carregamento"
	Local $caminho = $dic_cam.Item(String($arq_atual))

	If WinExists ("Abrir") Then
		WinActivate("Abrir")
		Sleep (500)
		ClipPut($caminho)
		Sleep (500)
		Send("{CTRLDOWN}v{CTRLUP}")
		Sleep (100)
		Send("{ENTER}")
		Return 1
	EndIf

	Return 0

EndFunc

Func sub_espera_arq2()
	$msg_erro = "Erro de carregamento"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="formUpload"]/div/span/i')
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_arq()
	$msg_erro = "Erro de carregamento"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="formUpload"]/div/span/i')
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			Local $sInput = "var x = document.createElement('INPUT'); x.id = 'arquivo_trago_amor'; x.type = 'file'; x.style.display = 'none'; document.body.appendChild(x);"
			Local $com = "var files=document.getElementById('arquivo_trago_amor').files;" & _
						"var eve=document.createEvent('HTMLEvents');" & _
						"eve.initEvent('drop', true, true);" & _
						"eve.dataTransfer = {files:files};" & _
						"eve.type = 'drop'; document.getElementById('formUpload').dispatchEvent(eve);"

			Local $caminho = $dic_cam.Item(String($arq_atual))
			_WD_SelectFiles($sSession, $_WD_LOCATOR_ByXPath, '//input[@id="arquivo_trago_amor"]', $caminho)
			If @error = $_WD_ERROR_Success Then
				_WD_ExecuteScript($sSession, $com)
				If @error = $_WD_ERROR_Success Then return 1
			Else
				_WD_ExecuteScript($sSession, $sInput)
				If @error = $_WD_ERROR_Success Then
					_WD_SelectFiles($sSession, $_WD_LOCATOR_ByXPath, '//input[@id="arquivo_trago_amor"]', $caminho)
					If @error = $_WD_ERROR_Success Then
						;ConsoleWrite ("  Input virtual OK  " & @CRLF)
						_WD_ExecuteScript($sSession, $com)
						If @error = $_WD_ERROR_Success Then return 1
					EndIf
				EndIf
			EndIf
			;ConsoleWrite ("  Erro no carregamento novo  " & @CRLF)
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_arq_f()
	$msg_erro = "Erro de carregamento"
	If $init > 1 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//*[@id='modalArquivo']")
		If @error = $_WD_ERROR_Success Then
			Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
			If @error Then
				$IsVisible = False
			EndIf

			If $IsVisible Then Return 0
		EndIf
		;ConsoleWrite (" carrega novo - espera final  ")
		$init = 0
		Return 1
	Else
		$init += 1
		Return 0
	EndIf

EndFunc

Func sub_abre_aprov()
	Local $aprov = _GUICtrlTreeView_GetChecked ($tv_lista_arq, $ficha_atual)

	If $aprov Then
		$msg_erro = "Ficha não permite aprovação"
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
		If @error = $_WD_ERROR_Success Then
			Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
			If StringCompare($bot_texto, "Aprovar") = 0 Then
				_WD_ElementAction($sSession, $sElement, 'click')
				Return 1
			Else
				Return -1
			EndIf
		EndIf

	Else
		$msg_erro = "Ficha não permite reprovação"
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button[2]")
		If @error = $_WD_ERROR_Success Then
			Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
			If StringCompare($bot_texto, "Reprovar") = 0 Then
				_WD_ElementAction($sSession, $sElement, 'click')
				Return 1
			Else
				Return -1
			EndIf
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_aprov()
	Local $aprov = _GUICtrlTreeView_GetChecked ($tv_lista_arq, $ficha_atual)
	Local $comentarios = GUICtrlRead ($e_comentarios)
	$msg_erro = "Falha ao processar aprovação"

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//textarea[@id='WorkflowStep_Comentarios']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			_WD_ElementAction($sSession, $sElement, 'clear')
			_WD_ElementAction($sSession, $sElement, 'value', $comentarios)

			If $aprov Then
				$msg_erro = "Falha ao processar aprovação"
				$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalAprovarProsseguir']")
				If @error = $_WD_ERROR_Success Then
					_WD_ElementAction($sSession, $sElement, 'click')
					Return 1
				Else
					Return 0
				EndIf

			Else
				$msg_erro = "Falha ao processar reprovação"
				$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalRejeitarProsseguir']")
				If @error = $_WD_ERROR_Success Then
					_WD_ElementAction($sSession, $sElement, 'click')
					Return 1
				Else
					Return 0
				EndIf
			EndIf
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_aprov_f()
	Local $aprov = _GUICtrlTreeView_GetChecked ($tv_lista_arq, $ficha_atual)
	Local $comentarios = GUICtrlRead ($e_comentarios)
	$msg_erro = "Falha ao processar aprovação"

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//textarea[@id='WorkflowStep_Comentarios']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			return 0
		EndIf
	EndIf
	Return 1
EndFunc

Func sub_abre_subm()
	$msg_erro = "Ficha não permite submissão"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		If StringCompare($bot_texto, "Aprovar") = 0 Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		Else
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_subm()
	Local $comentarios = GUICtrlRead ($e_comentarios)
	$msg_erro = "Falha ao processar submissão"

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//textarea[@id='WorkflowStep_Comentarios']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			_WD_ElementAction($sSession, $sElement, 'clear')
			_WD_ElementAction($sSession, $sElement, 'value', $comentarios)
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalAprovarProsseguir']")
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'click')
				Return 1
			Else
				Return 0
			EndIf
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_abre_assume()
	$msg_erro = "Falha ao assumir"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		;ConsoleWrite ($bot_texto & @CRLF)
		If StringCompare($bot_texto, "Assumir") = 0 Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		Else
			Return -1
		EndIf
	EndIf

	Return 0

EndFunc

Func sub_espera_assume()
	$msg_erro = "Falha ao assumir"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalReassignProsseguir']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		EndIf
	EndIf

	Return 0

EndFunc

Func sub_pesquisa()
	$msg_erro = "Ficha não localizada"
	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//input[@id='inputPesquisaDireta']")
	If @error = $_WD_ERROR_Success Then
		_WD_ElementAction($sSession, $sElement, 'clear')
		_WD_ElementAction($sSession, $sElement, 'value', $nome_ficha)
		Sleep (500)
		_WD_ElementAction($sSession, $sElement, 'value', @CRLF)
		Return 1
	Else
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//input[@id='Keywords']")
		If @error = $_WD_ERROR_Success Then
			_WD_ElementAction($sSession, $sElement, 'clear')
			_WD_ElementAction($sSession, $sElement, 'value', $nome_ficha)
			Sleep (500)
			_WD_ElementAction($sSession, $sElement, 'value', @CRLF)
			Return 1
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_espera_pesq()
	$msg_erro = "Ficha não localizada"

	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="divPesquisaGoogleishResultado"]/div[2]/strong')
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			;$init = 0
			Return -1
		EndIf
	EndIf

	If $init = 0 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="loadingPesquisaGoogleish"]/div/div/div')
		If @error = $_WD_ERROR_Success Then
			Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
			If @error Then
				$IsVisible = False
			EndIf

			If $IsVisible Then
				Return 0
			EndIf
		EndIf

		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/thead/tr[2]/th[3]/input")
		If @error = $_WD_ERROR_Success Then
			Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
			If @error Then
				$IsVisible = False
			EndIf

			If Not $IsVisible Then

				Return 0
			EndIf

	;~ 		$retorno = _WD_ExecuteScript($sSession, "var arq = '" & $nome_ficha & "'; var lista = document.getElementById('tableResultadoPesquisa').rows;  if (lista[2].cells.length > 10) { var pos = 0; for(i = 2, len = lista.length; i < len; i++) { if (lista[i].children[2].children[0].textContent.trim().toLowerCase() == arq.toLowerCase()){ pos = i }}; return pos-1;} else { return 0; }")
	;~ 		If @error = $_WD_ERROR_Success Then
	;~ 			$data = Json_Decode ($retorno)
	;~ 			Local $pos = Json_Get ($data, '["value"]')
	;~ 			MsgBox($MB_SYSTEMMODAL, "Information", "Pos: " & $pos)
	;~ 		EndIf

			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/thead/tr[2]/th[3]/input")
			If @error = $_WD_ERROR_Success Then
				_WD_ElementAction($sSession, $sElement, 'clear')
				_WD_ElementAction($sSession, $sElement, 'value', $nome_ficha & " ")
				$init = 1
			EndIf
		EndIf
	ElseIf $init > 3 Then
		$retorno = _WD_ExecuteScript($sSession, 'if (document.getElementById("tableResultadoPesquisa").rows[2].cells.length > 10) { return document.getElementById("tableResultadoPesquisa").rows.length - 2} else { return 0 }')
		If @error <> $_WD_ERROR_Success Then Return 0
		$data = Json_Decode ($retorno)
		$num_fichas = Json_Get ($data, '["value"]')

		If $num_fichas > 0 Then
			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/tbody/tr[" & $num_fichas & "]/td[3]/a/span")
			If @error = $_WD_ERROR_Success Then
				$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
				If StringCompare($ficha_texto, $nome_ficha) = 0 Then
					$msg_erro = ""
					$msg_erro &= @TAB & $ficha_texto
					;ConsoleWrite("Ficha-" & $ficha_texto)
					$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/tbody/tr[" & $num_fichas & "]/td[4]/span")
					If @error = $_WD_ERROR_Success Then
						$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
						;ConsoleWrite("-rev-" & $ficha_texto)
						$msg_erro &= @TAB & $ficha_texto
					EndIf
					$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//table[@id='tableResultadoPesquisa']/tbody/tr[" & $num_fichas & "]/td[7]")
					If @error = $_WD_ERROR_Success Then
						$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
						;ConsoleWrite("-" & $ficha_texto)
						$msg_erro &= @TAB & $ficha_texto
					EndIf
					;ConsoleWrite(@CRLF)
					$init = 0
					Return 1
				Else
					$init = 0
					Return -1
				EndIf
			EndIf
		Else
			$init = 0
			Return -1
		EndIf
	Else
		$init += 1
	EndIf
	Return 0

EndFunc

Func sub_ver_ficha_rev()
	$msg_erro = "Ficha já está em revisão"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		If StringCompare($bot_texto, "Revisar") = 0 Then
			;ConsoleWrite("Verifica rev OK" & @CRLF)
			Return 1
		Else
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc
Func sub_ver_ficha_aprov()
	$msg_erro = "Não é possível carregar arquivos na ficha"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		If StringCompare($bot_texto, "Aprovar") = 0 Then
			;ConsoleWrite("Verifica aprov OK" & @CRLF)
			Return 1
		Else
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc
Func sub_revisa_ficha()
   $msg_erro = "Ficha já está em revisão"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		If StringCompare($bot_texto, "Revisar") = 0 Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		Else
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc
Func sub_espera_rev_i()
	$msg_erro = "Falha em revisar ficha"
	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)


	If $init = 0 Then
		Local $aHandles = _WD_Window($sSession, 'handles')
		For $sHandle In $aHandles
			_WD_Window($sSession, 'switch', '{"handle":"' & $sHandle & '"}')
			If StringInStr (_WD_Action($sSession, 'title'), "Revisar") Then
				$init = 1
				ExitLoop
			EndIf
		Next
	ElseIf $init > 10 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="main-container"]/div[2]/div/div[2]/div/div/div[4]/h1')
		If @error = $_WD_ERROR_Success Then
			Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
			If @error Then
				$IsVisible = False
			EndIf

			If Not $IsVisible Then
				Return 0
			EndIf
			Local $txt = _WD_ElementAction($sSession, $sElement, 'text')
			;ConsoleWrite($txt)
			If Not StringInStr ($txt, 'Revisão') Then
				_WD_Window($sSession, "close")
				_WD_Window($sSession, "switch", $tab_principal)
				$init = 0
				Return -1
			EndIf

			$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="main-container"]/div[2]/div/div[2]/div/div/div[4]/h1/span')
			If @error <> $_WD_ERROR_Success Then
				_WD_Window($sSession, "close")
				_WD_Window($sSession, "switch", $tab_principal)
				$init = 0
				Return -1
			EndIf
			$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
			Local $aArray = StringRegExp($ficha_texto, '\[(.*)\] \[(.*)\]', $STR_REGEXPARRAYMATCH)
			;ConsoleWrite("ficha: " & UBound($aArray) &"  " & $aArray[0] & @CRLF)
			If StringCompare($aArray[0], $nome_ficha) = 0 Then
				$init = 0
				Return 1
			Else
				_WD_Window($sSession, "close")
				_WD_Window($sSession, "switch", $tab_principal)
				$init = 0
				Return -1
			EndIf

		EndIf

	Else
		$init += 1
	EndIf
	Return 0

EndFunc
Func sub_preenche_rev()
	$msg_erro = "Falha em revisar ficha"

	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)

	If $init = 0 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="TipoRevisao_chosen"]/a/span')
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'click')
		;ConsoleWrite("1-")
		$init = 1
		Return 0
	ElseIf $init = 1 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="TipoRevisao_chosen"]/div/div/input')
		If @error <> $_WD_ERROR_Success Then Return 0
		Local $rev_tipo = GUICtrlRead ($c_rev)
		_WD_ElementAction($sSession, $sElement, 'clear')
		_WD_ElementAction($sSession, $sElement, 'value', $rev_tipo & @CRLF)
		;ConsoleWrite("2-")
		$init = 2
		Return 0
	ElseIf $init = 2 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="AttachTo_chosen"]/a/span')
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'click')
		;ConsoleWrite("3-")
		$init = 3
		Return 0
	ElseIf $init = 3 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="AttachTo_chosen"]/div/div/input')
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'clear')
		_WD_ElementAction($sSession, $sElement, 'value', "2" & @CRLF)
		$init = 4
		Return 0
	ElseIf $init = 4 Then
		Local $comentarios = GUICtrlRead ($e_comentarios)
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//textarea[@id='Observacoes']")
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'clear')
		_WD_ElementAction($sSession, $sElement, 'value', $comentarios)
		$init = 5
		Return 0
	ElseIf $init = 5 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='btnRevisarFicha']")
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'click')
		$init = 0
		Return 1
	EndIf
	Return 0

EndFunc

Func sub_espera_rev_f()
	$msg_erro = "Falha em revisar ficha"

	Local $nome_ficha = _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual)

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//img[@id='formRevisarFichaLoading']")
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			Return 0
		EndIf
	EndIf

;~ 	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="gritter-item-2"]/div[2]/div[2]/p')
;~ 	If @error = $_WD_ERROR_Success Then
;~ 		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
;~ 		If @error Then
;~ 			$IsVisible = False
;~ 		EndIf

;~ 		If $IsVisible Then
;~ 			Local $txt = _WD_ElementAction($sSession, $sElement, 'text')
;~ 			If StringInStr ($txt, 'Unable') Then
;~ 				$et = $etp_rev_ext
;~ 				_WD_Window($sSession, "close")
;~ 				_WD_Window($sSession, "switch", $tab_principal)
;~ 				$init = 0
;~ 				Return 1
;~ 			Else
;~ 				Return -1
;~ 			EndIf

;~ 			Return 0
;~ 		EndIf
;~ 	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="main-container"]/div[2]/div/div[2]/div/div/div/strong')
	If @error <> $_WD_ERROR_Success Then
		If $init > 5 Then
			$et = $etp_rev_ext
			_WD_Window($sSession, "close")
			_WD_Window($sSession, "switch", $tab_principal)
			$init = 0
			ConsoleWrite ("  Tenta submeter ao workflow   " & @CRLF)
			Return 1
		Else
			$init += 1
		EndIf

		Return 0
	EndIf

	Local $txt = _WD_ElementAction($sSession, $sElement, 'text')
	If StringCompare ($txt, 'Sucesso!') <> 0 Then
		_WD_Window($sSession, "close")
		_WD_Window($sSession, "switch", $tab_principal)
		$init = 0
		Return -1
	EndIf

	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="main-container"]/div[2]/div/div[2]/div/div/div[30]/h1/span[2]')
	If @error <> $_WD_ERROR_Success Then
		_WD_Window($sSession, "close")
		_WD_Window($sSession, "switch", $tab_principal)
		Return -1
	EndIf

	$ficha_texto = _WD_ElementAction($sSession, $sElement, 'text')
	Local $aArray = StringRegExp($ficha_texto, '\[(.*)\] \[(.*)\]', $STR_REGEXPARRAYMATCH)
	;ConsoleWrite("ficha: " & UBound($aArray) &"  " & $aArray[0] & @CRLF)
	If StringCompare($aArray[0], $nome_ficha) = 0 Then
		$init = 0
		_WD_Window($sSession, "close")
		_WD_Window($sSession, "switch", $tab_principal)
		Return 1
	Else
		$init = 0
		_WD_Window($sSession, "close")
		_WD_Window($sSession, "switch", $tab_principal)
		Return -1
	EndIf
	Return 0

EndFunc

Func sub_abre_workflow()
   $msg_erro = "Falha ao submeter no workflow"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//div[@id='detalhesFicha']/div/div/div/div/div/button")
	If @error = $_WD_ERROR_Success Then
		Local $bot_texto = _WD_ElementAction($sSession, $sElement, 'text')
		If StringInStr($bot_texto, "Workflow") Then
			_WD_ElementAction($sSession, $sElement, 'click')
			Return 1
		Else
			Return -1
		EndIf
	EndIf
	Return 0

EndFunc

Func sub_preenche_workflow()
	$msg_erro = "Falha ao submeter no workflow"
	If $init = 0 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="AttachTo_chosen"]/a/span')
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'click')
		;ConsoleWrite("3-")
		$init = 1
		Return 0
	ElseIf $init = 1 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="AttachTo_chosen"]/div/div/input')
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'clear')
		_WD_ElementAction($sSession, $sElement, 'value', "2" & @CRLF)
		$init = 2
		Return 0
	ElseIf $init = 2 Then
		$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, "//button[@id='modalWorkflowProsseguir']")
		If @error <> $_WD_ERROR_Success Then Return 0
		_WD_ElementAction($sSession, $sElement, 'click')
		$init = 0
		Return 1
	EndIf
	Return 0
EndFunc


Func sub_espera_workflow()
	$msg_erro = "Falha ao submeter no workflow"
	$sElement = _WD_FindElement($sSession, $_WD_LOCATOR_ByXPath, '//*[@id="modalWorkflowLoading"]')
	If @error = $_WD_ERROR_Success Then
		Local $IsVisible = _WD_ElementAction($sSession, $sElement, 'displayed')
		If @error Then
			$IsVisible = False
		EndIf

		If $IsVisible Then
			Return 0
		EndIf
	EndIf

	Return 1

EndFunc

Func sub_rel_pesq()
	FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Pesquisa OK - " & $msg_erro & @CRLF)
	;FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "ERRO-"& $msg_erro & @CRLF)
	Return 1
EndFunc

Func sub_rel_rev()
	FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Revisão OK - " & $msg_erro & @CRLF)
	;FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "ERRO-"& $msg_erro & @CRLF)
	Return 1
EndFunc

Func sub_rel_carr()
	;FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Upload OK" @CRLF)
	FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "Upload OK" & @CRLF)
	Return 1
EndFunc

Func sub_rel_sub()
	FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Submete OK" & @CRLF)
	;FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "ERRO-"& $msg_erro & @CRLF)
	Return 1
EndFunc

Func sub_rel_aprov()
	Local $aprov = _GUICtrlTreeView_GetChecked ($tv_lista_arq, $ficha_atual)
	If $aprov Then
		FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Aprovação OK" & @CRLF)
	Else
		FileWrite( $h_relatorio, _GUICtrlTreeView_GetText ($tv_lista_arq, $ficha_atual) & @TAB & @TAB & "Reprovação OK" & @CRLF)
	EndIf

	;FileWrite( $h_relatorio, @TAB & _GUICtrlTreeView_GetText ($tv_lista_arq, $arq_atual) & @TAB & "ERRO-"& $msg_erro & @CRLF)
	Return 1
EndFunc

Func sub_nada()

	If $conta < 2 Then
	  ;ConsoleWrite("Teste")
	  Return 0
   Else
	  ;ConsoleWrite(" OK" & @CRLF)
	  Return 1
   EndIf

EndFunc

Func compara_rev($rev1, $rev2)
	Local $a1 = StringRegExp(StringUpper($rev1), '(\w+)(\d+)', $STR_REGEXPARRAYMATCH)
	If UBound($a1) <= 0 Then Return 0
	Local $a2 = StringRegExp(StringUpper($rev2), '(\w+)(\d+)', $STR_REGEXPARRAYMATCH)
	If UBound($a2) <= 0 Then Return 0

	Local $m1 = letra_num($a1[0])
	Local $m2 = letra_num($a2[0])

	If $m1 < $m2 Then
		Return -1
	ElseIf $m1 > $m2 Then
		Return 1
	Else
		$m1 = Number($a1[1])
		$m2 = Number($a2[1])
		If $m1 < $m2 Then
			Return -1
		ElseIf $m1 > $m2 Then
			Return 1
		Else
			Return 0
		EndIf
	EndIf

EndFunc


Func letra_num ($letras)
	Local $n = StringLen($letras)
	Local $sum = 0
	For $i = $n To 1 Step -1
		$val = Asc(StringMid($letras, $i, 1)) - 64
		$sum += $val*Floor(26 ^ ($n - $i))
	Next
	Return $sum
EndFunc
