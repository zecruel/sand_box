#include <GUIConstantsEx.au3>
#include <WindowsConstants.au3>
#include <EditConstants.au3>
#include <Constants.au3>
#Include <File.au3>
#include <ListViewConstants.au3>
#include <GuiListView.au3>
#include <GuiRichEdit.au3>

$ini = 'function copia(texto) {var existsTextarea = document.getElementById("inputPesquisaDireta"); existsTextarea.value = texto; existsTextarea.select();document.execCommand("copy"); existsTextarea.blur();} function isvisible(element) { return (element.offsetWidth > 0 && element.offsetHeight > 0);} function ver_ficha(){ if(isvisible(document.getElementById("modalDetalhesFichaCorpo"))){ copia("ficha_ok");} else{copia("nao");}} function cont_aprov(){ if(isvisible(document.getElementById("modalAprovarCorpo"))){ copia("aprov_ini_ok");} else{copia("nao");}} function cont_reprov(){ if(isvisible(document.getElementById("modalRejeitarCorpo"))){ copia("reprov_ini_ok");} else{copia("nao");}} function limpa() {var existsTextarea = document.getElementById("inputPesquisaDireta"); existsTextarea.value = "";} function proc_ficha() { document.getElementById("tableInbox").tHead.rows[1].cells[2].children[0].select();} function abre_ficha() { if (document.getElementById("tableInbox").rows[2].cells.length == 13) { document.getElementById("tableInbox").tBodies[0].rows[0].cells[2].children[0].children[0].click();} else {copia("ficha_erro");}} function fecha_ficha() { document.getElementById("modalDetalhesFichaX").click();} function proc_arq(arq) {var lista = document.getElementById("detalhesFicha").children[1].children[0].children[1].children[0].children[0].children[0].children[0].children;var pos = 0; for(i = 0, len = lista.length; i < len; i++){ if (lista[i].children[1].children[0].children[0].children[0].text.trim().toLowerCase() == arq.toLowerCase()){pos=i+1;break;}};return pos;} function up_arq_e(pos) {document.getElementById("detalhesFicha").children[1].children[0].children[1].children[0].children[0].children[0].children[0].children[pos].children[1].children[0].children[1].children[0].click();} function up_arq_n() {document.getElementById("detalhesFicha").children[1].children[0].children[0].children[1].children[5].click();} function up_arq(arq) {var indice = proc_arq(arq); if (indice > 0){up_arq_e(indice - 1);} else { up_arq_n(); }; copia("up_ok");} function abre_arq() {document.getElementById("formUpload").click(); copia("arq_ok");} function aprova_ini() {document.getElementById("detalhesFicha").children[0].children[0].children[0].children[1].children[0].children[0].click();} function reprova_ini() {document.getElementById("detalhesFicha").children[0].children[0].children[0].children[1].children[0].children[1].click();} function aprova(coment) {document.getElementById("WorkflowStep_Comentarios").value = coment; document.getElementById("modalAprovarProsseguir").click();} function reprova(coment) {document.getElementById("WorkflowStep_Comentarios").value = coment; document.getElementById("modalRejeitarProsseguir").click();} function proc_ficha_pesq() {document.getElementById("tableResultadoPesquisa").tHead.rows[1].cells[2].children[0].select();} copia("ini_ok");'
$pesq1 = 'http://gedoc2/?sm=corp&q='
$pesq2 = '&t=5&m=true'

$ini2 = 'function copia(texto) {var existsTextarea = document.getElementById("Keywords"); existsTextarea.value = texto; existsTextarea.select();document.execCommand("copy"); existsTextarea.blur();} function proc_ficha_pesq() {document.getElementById("tableResultadoPesquisa").tHead.rows[1].cells[2].children[0].select();} function abre_ficha2() { if (document.getElementById("tableResultadoPesquisa").rows[2].cells.length == 13) { document.getElementById("tableResultadoPesquisa").tBodies[0].rows[0].cells[2].children[0].children[0].click();} else {copia("ficha_erro");}} copia("ini_ok");'

Global $gaDropFiles[1]

$atual = 0
$prox = 0
$passo = 0
$prox_passo = 1
$num_tentativas = 60
$cont_tent = 0
$comando = "proc_ficha()"
$desejado = "ini_ok"
$tempo = 2000

$ok = False
AutoItSetOption("SendKeyDelay", 0)

$hTimer = TimerInit() ; Begin the timer and store the handle in a variable.

Const $gui_x = 900, $gui_y = 600
Dim $arq_drive, $arq_dir, $arquivo, $arq_ext

$sDirINI = @ScriptDir & "\comando.ini"

$g_princ = GUICreate("Submete GEDEX", $gui_x, $gui_y,-1,-1,-1,$WS_EX_ACCEPTFILES)  ; Cria a caixa de dialogo

Local $idTab = GUICtrlCreateTab(10, 10, $gui_x - 20, $gui_y - 20)

GUICtrlCreateTabItem("Caixa de Entrada")

$b_exec = GUICtrlCreateButton("Executa", 20, 40, 100)
$b_para = GUICtrlCreateButton("Para", 130, 40, 100)

$b_remove = GUICtrlCreateButton("Remove", $gui_x - 120, 40, 100)

$lv_lista_arq = GUICtrlCreateListView("Ficha                                   |A|R|Caminho                             |OK", 20, 70, $gui_x - 40, $gui_y - 205, BitOR($LVS_SHOWSELALWAYS, $LVS_SINGLESEL, $LVS_EDITLABELS, $LVS_NOSORTHEADER))
GUICtrlSetState(-1, $GUI_DROPACCEPTED)

GUICtrlCreateLabel ("Comentários:", 25, $gui_y - 125)

$status = GUICtrlCreateLabel ("Parado", 350, 45,100)

$e_status = GUICtrlCreateEdit("", 20, $gui_y - 105, $gui_x - 40, 85, $ES_WANTRETURN+$ES_MULTILINE)

GUICtrlCreateTabItem("Baixar pesquisa")
	  $b_busca = GUICtrlCreateButton("Busca", 500, 40, 100)
    $e_busca = GUICtrlCreateEdit("", 20, 40, 400, 400, BitOR($ES_WANTRETURN, $ES_MULTILINE, $WS_VSCROLL, $ES_AUTOVSCROLL))
	;$e_busca = _GUICtrlRichEdit_Create($g_princ, "", 20, 40, 400, 400, BitOR($ES_WANTRETURN, $ES_MULTILINE, $WS_VSCROLL, $ES_AUTOVSCROLL))

GUICtrlCreateTabItem(""); end tabitem definition

GUISetState(@SW_SHOW,$g_princ)

GUIRegisterMsg($WM_DROPFILES, "WM_DROPFILES_FUNC")
GUIRegisterMsg($WM_NOTIFY, "WM_NOTIFY")



 While 1
   $msg = GUIGetMsg(1)
   Select
	  Case $msg[0] = $GUI_EVENT_CLOSE And $msg[1] = $g_princ

		  ExitLoop
	   Case $msg[0] = $b_para
		 $passo = 0
		 GUICtrlSetData ($status, "Parado")

	  Case $msg[0] = $b_exec
		 $passo = 2
		 GUICtrlSetData ($status, "Executando...")

		 $selecao = _GUICtrlListView_GetSelectedIndices($lv_lista_arq, True)
		 If ($selecao[0] > 0) Then
			$atual = $selecao[1]
			$prox = _GUICtrlListView_GetNextItem($lv_lista_arq, $atual,0,0)

		 Else
			$passo = 0
			GUICtrlSetData ($status, "Parado")
			MsgBox (0, "Submete GEDEX", "Nada a fazer?")
		 EndIf


	  Case $msg[0] = $b_remove
		  _GUICtrlListView_DeleteItemsSelected ($lv_lista_arq)

	  Case $msg[0] = $b_busca
		  busca()

	  Case $msg[0] = $GUI_EVENT_DROPPED
		  For $i = 0 To UBound($gaDropFiles) - 1

			 _PathSplit($gaDropFiles[$i], $arq_drive, $arq_dir, $arquivo, $arq_ext)
			 GUICtrlCreateListViewItem($arquivo & "|x||"& $gaDropFiles[$i] &"|", $lv_lista_arq)
			 $last_item = _GUICtrlListView_GetItemCount ($lv_lista_arq) - 1
		  Next

	  EndSelect

	  If (TimerDiff($hTimer) > $tempo) Then
		 $hTimer = TimerInit() ; restart Timer
		 Select
		 Case $passo = 0
			;nao faz nada


		 Case $passo = 1
			if (envia_comando($comando)) Then
			   Sleep (200)
			   SetError(0)
			   $clip = ClipGet()
			   If Not @error Then
				  If (StringCompare ($clip, $desejado) = 0) Then
					 $passo = $prox_passo
				  Else
					 $cont_tent = $cont_tent + 1
				  EndIf

				  If ($cont_tent >= $num_tentativas) Then
					 $passo = 0
					 GUICtrlSetData ($status, "Parado")
					 MsgBox (0, "Submete GEDEX", "ERRO!")
				  EndIf
			   Else
				  $cont_tent = $cont_tent + 1
			   EndIf
			Else
			   $passo = 0
			   GUICtrlSetData ($status, "Parado")
			   MsgBox (0, "Submete GEDEX", "ERRO!")
			EndIf

		 Case $passo = 2
			$comando = $ini
			$passo = 1
			$prox_passo = 3
			$desejado = "ini_ok"
			$tempo = 2000
		 Case $passo = 3
			$ficha = _GUICtrlListView_GetItemText($lv_lista_arq, $atual, 0)
			if (envia_comando("proc_ficha()")) Then
			   Sleep (200)
			   Send($ficha)
			   Sleep (100)
			   $passo = 4
			   $tempo = 2000
			Else
			   $passo = 0
			   GUICtrlSetData ($status, "Parado")
			   MsgBox (0, "Submete GEDEX", "ERRO!")
			EndIf
		 Case $passo = 4
			if (envia_comando("abre_ficha()")) Then
			   $passo = 5
			   $tempo = 2000
			Else
			   $passo = 0
			   GUICtrlSetData ($status, "Parado")
			   MsgBox (0, "Submete GEDEX", "ERRO!")
			EndIf
		 Case $passo = 5
			SetError(0)
			$clip = ClipGet()
			If Not @error Then
			   If (StringCompare ($clip, 'ficha_erro') = 0) Then
				  $passo = 50
				  $ok = False
			   Else
				  $comando = "ver_ficha()"
				  $passo = 1
				  $prox_passo = 6
				  $desejado = "ficha_ok"
				  $tempo = 2000

			   EndIf
			Else
			   $passo = 50
			EndIf
		 Case $passo = 6
			$ficha = _GUICtrlListView_GetItemText($lv_lista_arq, $atual, 3)
			_PathSplit($ficha, $arq_drive, $arq_dir, $arquivo, $arq_ext)
			$comando = 'up_arq("' & $arquivo & $arq_ext & '")'
			;MsgBox (0, "Submete GEDEX", $comando)
			$passo = 1
			$prox_passo = 7
			$desejado = "up_ok"
			$tempo = 2000

		 Case $passo = 7
			$ficha = _GUICtrlListView_GetItemText($lv_lista_arq, $atual, 3)
			envia_comando("abre_arq()")
			;ClipPut($ficha)
			WinWaitActive("Abrir")

			Sleep (500)
			ClipPut($ficha)
			Send("{CTRLDOWN}v{CTRLUP}")
			Sleep (100)
			Send("{ENTER}")
			$passo = 8
			$tempo = 4000


		 Case $passo = 8
			$comando = "ver_ficha()"
			$passo = 1
			$prox_passo = 9
			$desejado = "ficha_ok"
			$tempo = 2000

		 Case $passo = 9
			If (StringCompare(_GUICtrlListView_GetItemText($lv_lista_arq, $atual, 1), "x") = 0) Then
			   envia_comando("aprova_ini()")
			   Sleep(200)
			   $comando = "cont_aprov()"
			   $passo = 1
			   $prox_passo = 10
			   $desejado = "aprov_ini_ok"
			   $tempo = 2000
			Else
			   envia_comando("reprova_ini()")
			   Sleep(200)
			   $comando = "cont_reprov()"
			   $passo = 1
			   $prox_passo = 11
			   $desejado = "reprov_ini_ok"
			   $tempo = 2000
			EndIf

		 Case $passo = 10
			$comentarios = GUICtrlRead ($e_status)
			envia_comando('aprova("' & $comentarios &'")')
			Sleep(200)
			$comando = "cont_reprov()"
			$passo = 1
			$prox_passo = 20
			$desejado = "nao"
			$tempo = 2000
		 Case $passo = 11
			$comentarios = GUICtrlRead ($e_status)
			envia_comando('reprova("' & $comentarios &'")')
			Sleep(200)
			$comando = "cont_reprov()"
			$passo = 1
			$prox_passo = 20
			$desejado = "nao"
			$tempo = 2000
		 Case $passo = 20
			$ok = True ;-----------------------------------------------
			envia_comando("fecha_ficha()")
			$passo = 50
			$tempo = 1000

		 Case Else
			envia_comando("fecha_ficha()")
			If ($ok) Then
			   _GUICtrlListView_SetItem($lv_lista_arq, "x", $atual, 4)
			EndIf
			If ($prox = -1) Then
			   $passo = 0
			   GUICtrlSetData ($status, "Parado")
			   MsgBox (0, "Submete GEDEX", "Pronto!")
			Else
			   _GUICtrlListView_SetItemSelected($lv_lista_arq, $prox)
			   $selecao = _GUICtrlListView_GetSelectedIndices($lv_lista_arq, True)
			   If ($selecao[0] > 0) Then
				  $atual = $selecao[1]
				  $prox = _GUICtrlListView_GetNextItem($lv_lista_arq, $atual,0,0)
			   EndIf
			   $passo = 3
			EndIf

		 EndSelect
	  EndIf

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

Func WM_NOTIFY($hWnd, $iMsg, $wParam, $lParam)
    #forceref $hWnd, $iMsg, $wParam
    Local $hWndFrom, $iIDFrom, $iCode, $tNMHDR, $tInfo, $hWndListView = $lv_lista_arq
    If Not IsHWnd($lv_lista_arq) Then $hWndListView = GUICtrlGetHandle($lv_lista_arq)

    $tNMHDR = DllStructCreate($tagNMHDR, $lParam)
    $hWndFrom = HWnd(DllStructGetData($tNMHDR, "hWndFrom"))
    $iIDFrom = DllStructGetData($tNMHDR, "IDFrom")
    $iCode = DllStructGetData($tNMHDR, "Code")
    Switch $hWndFrom
        Case $hWndListView
            Switch $iCode
                Case $LVN_BEGINLABELEDITA, $LVN_BEGINLABELEDITW ; Start of label editing for an item
                    Return False ; Allow the user to edit the label
                Case $LVN_ENDLABELEDITA, $LVN_ENDLABELEDITW ; The end of label editing for an item
                    $tInfo = DllStructCreate($tagNMLVDISPINFO, $lParam)
                    If (DllStructGetData($tInfo, "Text") <> 0) Then
                        Return True ; If Text is empty the return value is ignored
                    EndIf
                Case $NM_DBLCLK ; Sent by a list-view control when the user double-clicks an item with the left mouse button
					 $tInfo = DllStructCreate($tagNMITEMACTIVATE, $lParam)
					 $index = DllStructGetData($tInfo, "Index")

					 If (StringCompare(_GUICtrlListView_GetItemText($lv_lista_arq, $index, 1), "x") = 0) Then
						_GUICtrlListView_SetItem($lv_lista_arq, "", $index, 1)
						_GUICtrlListView_SetItem($lv_lista_arq, "x", $index, 2)
					 Else
						_GUICtrlListView_SetItem($lv_lista_arq, "x", $index, 1)
						_GUICtrlListView_SetItem($lv_lista_arq, "", $index, 2)
					 EndIf
            EndSwitch
    EndSwitch
    Return $GUI_RUNDEFMSG
 EndFunc   ;==>WM_NOTIFY

 Func envia_comando($comando)
   If (WinActivate("GEDEX - Caixa de Entrada - Google Chrome") <> 0) Then
	  ClipPut($comando)
	  Send("{CTRLDOWN}l{CTRLUP}")
	  Sleep (200)
	  Send("javascript: ")
	  Sleep (100)
	  Send("{CTRLDOWN}v{CTRLUP}")
	  Sleep (100)
	  Send("{ENTER}")
	  Return True
   Else
	  Return False
   EndIf
EndFunc

 Func envia_comando2($comando)
   If (WinActivate("GEDEX - Portal - Google Chrome") <> 0) Then
	  ClipPut($comando)
	  Send("{CTRLDOWN}l{CTRLUP}")
	  Sleep (200)
	  Send("javascript: ")
	  Sleep (100)
	  Send("{CTRLDOWN}v{CTRLUP}")
	  Sleep (100)
	  Send("{ENTER}")
	  Return True
   Else
	  Return False
   EndIf
EndFunc

Func busca()
   If (WinActivate("GEDEX - Portal - Google Chrome") <> 0) Then
	  ;Sleep (200)
	  ClipPut('http://gedoc2/?sm=corp&q=111424cs05080c386|111424cs05082c19a28|11142cs05082c19a28&t=5&m=true')
	  ;MsgBox(0, "teste", "teste")

	  Send("{CTRLDOWN}l{CTRLUP}")
	  Sleep (200)
	  ;Send("javascript: ")
	  ;Sleep (100)
	  Send("{CTRLDOWN}v{CTRLUP}")
	  Sleep (100)
	  Send("{ENTER}")
	  Return True
   Else
	  Return False
   EndIf
EndFunc

