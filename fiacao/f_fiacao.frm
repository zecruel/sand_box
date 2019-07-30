VERSION 5.00
Begin {C62A69F0-16DC-11CE-9E98-00AA00574A4F} f_fiacao 
   Caption         =   "Fiacao"
   ClientHeight    =   4830
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   4365
   OleObjectBlob   =   "f_fiacao.frx":0000
   StartUpPosition =   1  'CenterOwner
End
Attribute VB_Name = "f_fiacao"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

'------------------------------------------------------------------------------------------------------
'  OBJETO E CONSTANTES PARA A JANELA DE DIALOGO DE ABRIR
'------------------------------------------------------------------------------------------------------

Option Explicit
 
Private Const OFN_ALLOWMULTISELECT = &H200
Private Const OFN_CREATEPROMPT = &H2000
Private Const OFN_ENABLEHOOK = &H20
Private Const OFN_ENABLETEMPLATE = &H40
Private Const OFN_ENABLETEMPLATEHANDLE = &H80
Private Const OFN_EXPLORER = &H80000                         '  new look commdlg
Private Const OFN_EXTENSIONDIFFERENT = &H400
Private Const OFN_FILEMUSTEXIST = &H1000
Private Const OFN_HIDEREADONLY = &H4
Private Const OFN_LONGNAMES = &H200000                       '  force long names for 3.x modules
Private Const OFN_NOCHANGEDIR = &H8
Private Const OFN_NODEREFERENCELINKS = &H100000
Private Const OFN_NOLONGNAMES = &H40000                      '  force no long names for 4.x modules
Private Const OFN_NONETWORKBUTTON = &H20000
Private Const OFN_NOREADONLYRETURN = &H8000
Private Const OFN_NOTESTFILECREATE = &H10000
Private Const OFN_NOVALIDATE = &H100
Private Const OFN_OVERWRITEPROMPT = &H2
Private Const OFN_PATHMUSTEXIST = &H800
Private Const OFN_READONLY = &H1
Private Const OFN_SHAREAWARE = &H4000
Private Const OFN_SHAREFALLTHROUGH = 2
Private Const OFN_SHARENOWARN = 1
Private Const OFN_SHAREWARN = 0
Private Const OFN_SHOWHELP = &H10
 
Private Type OPENFILENAME
        lStructSize As Long
        hwndOwner As Long
        hInstance As Long
        lpstrFilter As String
        lpstrCustomFilter As String
        nMaxCustFilter As Long
        nFilterIndex As Long
        lpstrFile As String
        nMaxFile As Long
        lpstrFileTitle As String
        nMaxFileTitle As Long
        lpstrInitialDir As String
        lpstrTitle As String
        flags As Long
        nFileOffset As Integer
        nFileExtension As Integer
        lpstrDefExt As String
        lCustData As Long
        lpfnHook As Long
        lpTemplateName As String
End Type

Private Type BROWSEINFO
    hwndOwner As Long
    pidlRoot As Long
    pszDisplayName As String
    pszTitle As String
    ulFlags As Long
    lpfn As Long
    lParam As Long
    iImage As Long
End Type
 
Private Declare Function GetOpenFileName Lib "comdlg32.dll" Alias "GetOpenFileNameA" (pOpenfilename As OPENFILENAME) As Long
Private Declare Function SHBrowseForFolder Lib "shell32.dll" Alias "SHBrowseForFolderA" (lpBrowseInfo As BROWSEINFO) As Long
Private Declare Function SHGetPathFromIDList Lib "shell32" (ByVal pidList As Long, ByVal lpBuffer As String) As Long
Private Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

Private diretorio As String
Private lista_componentes As Collection
Private num_componentes
Private num_ligacoes



 
Private Function AddBS(ByVal sPath As String) As String
'------------------------------------------------------------------------------------------------------
'  FUNCAO PARA ADICIONAR A BARRA AOS CAMINHOS
'------------------------------------------------------------------------------------------------------
    If Right$(sPath, 1) <> "\" Then sPath = sPath & "\"
    AddBS = sPath
End Function


Private Sub b_carrega_Click()
    l_num_comp.Caption = ""
    l_num_lig.Caption = ""
    
    If Trim(t_file.Value & vbNullString) <> vbNullString Then
        Set lista_componentes = carrega_fiacao(t_file.Value)
        num_componentes = 0
        num_ligacoes = 0
        
        Dim comp As Collection
        Dim term As Collection
        Dim i As Integer
        
        For Each comp In lista_componentes
            'Debug.Print comp("_id_")
            num_componentes = num_componentes + 1
            For i = 2 To comp.Count
                Set term = comp(i)
                'Debug.Print term("terminal") & "-" & term("ligacao") & "<->" & term("cont")
                num_ligacoes = num_ligacoes + 1
                
            Next i
        Next comp
        
        l_num_comp.Caption = "Número de componentes: " & CStr(num_componentes)
        l_num_lig.Caption = "Número de ligações: " & CStr(num_ligacoes)
    End If
End Sub

Private Sub b_dupli_Click()
    Dim comp As Collection
    Dim term As Collection
    Dim i As Integer
    t_log.Value = "--Ligações duplicadas--" & vbCrLf
    For Each comp In lista_componentes
        For i = 2 To comp.Count
            Set term = comp(i)
            If (term("cont") > 1) Then
                t_log.Value = t_log.Value & comp("_id_") & "." & term("terminal") & vbTab & term("ligacao") & vbCrLf
            End If
            
        Next i
    Next comp
End Sub

Private Sub b_explora_Click()
'------------------------------------------------------------------------------------------------------
'  escolhe um arquivo
'------------------------------------------------------------------------------------------------------
    Dim tOPENFILENAME As OPENFILENAME
    Dim lResult As Long
    Dim vFiles As Variant
    Dim lIndex As Long, lStart As Long
    
    With tOPENFILENAME
        .lpstrTitle = "Escolha o arquivo"
        .flags = OFN_EXPLORER Or OFN_FILEMUSTEXIST Or OFN_LONGNAMES Or OFN_NOCHANGEDIR
        .hwndOwner = 0&
        .nMaxFile = 65535
        .lpstrInitialDir = diretorio
        .lpstrFilter = "Arquivos CSV" & Chr(0) & "*.csv" & Chr(0) & Chr(0)
        .lpstrFile = Space(.nMaxFile - 1) & Chr(0)
        .lStructSize = Len(tOPENFILENAME)
    End With
    
    lResult = GetOpenFileName(tOPENFILENAME)
    
    If lResult > 0 Then
        t_file.Value = Trim$(tOPENFILENAME.lpstrFile)
        diretorio = Left(t_file.Value, InStrRev(t_file.Value, "\"))
    End If

End Sub


Private Sub b_ident_Click()
    If Trim(t_ident.Value & vbNullString) <> vbNullString Then
        fiacao_identifica Trim(t_ident.Value)
    End If
    
End Sub

Private Sub b_n_conect_Click()
    Dim comp As Collection
    Dim term As Collection
    Dim i As Integer
    t_log.Value = "-- Ligações não executadas --" & vbCrLf
    For Each comp In lista_componentes
        For i = 2 To comp.Count
            Set term = comp(i)
            If (term("cont") = 0) Then
                t_log.Value = t_log.Value & comp("_id_") & "." & term("terminal") & vbTab & term("ligacao") & vbCrLf
            End If
            
        Next i
    Next comp
End Sub

Private Sub b_preenche_Click()
    Dim tags
    tags = Array("terminal", "ligacao", "esquematico", "esq_inf", "le", "rev", "num")
   
    If p_preenche.SelectedItem.Index = 0 Then
        If c_preenche.ListIndex < 6 Then
            numera_sequencial tags(c_preenche.ListIndex), CInt(t_ini.Value), CStr(t_prefixo.Value), CStr(t_sufixo.Value)
        Else
            numera_sequencial_secionavel CInt(t_ini.Value), CStr(t_prefixo.Value), CStr(t_sufixo.Value)
        End If
        
    Else
        If c_preenche.ListIndex < 6 Then
            Dim linhas() As String
            linhas = Split(t_preenche_lista.Text, vbCrLf)
            numera_lista linhas, tags(c_preenche.ListIndex)
        End If
    End If
End Sub

Private Sub b_print_Click()
    Dim comp As Collection
    Dim term As Collection
    Dim i As Integer
    t_vis.Value = ""
    For Each comp In lista_componentes
        t_vis.Value = t_vis.Value & "----" & comp("_id_") & "----" & vbCrLf
        For i = 2 To comp.Count
            Set term = comp(i)
            t_vis.Value = t_vis.Value & term("terminal") & vbTab & term("ligacao") & vbCrLf
            
            
        Next i
    Next comp

End Sub

Private Sub b_run_Click()
    
    executa_fiacao lista_componentes, t_rev.Value
    
    Dim executado As Integer
    Dim duplicado As Integer
    Dim comp As Collection
    Dim term As Collection
    Dim i As Integer
    
    
    executado = 0
    duplicado = 0
    l_num_dupli.Caption = ""
    l_num_exec.Caption = ""
    
    If Not (lista_componentes Is Nothing) Then
        For Each comp In lista_componentes
            
            For i = 2 To comp.Count
                Set term = comp(i)
                'Debug.Print term("terminal") & "-" & term("ligacao") & "<->" & term("cont")
                If (term("cont") > 0) Then executado = executado + 1
                If (term("cont") > 1) Then duplicado = duplicado + 1
                
            Next i
        Next comp
        
        l_num_exec.Caption = "Ligações executadas: " & executado
        l_num_dupli.Caption = "Ligações duplicadas: " & duplicado
    End If
    
    
End Sub

Private Sub b_sel_rev_Click()
    If Trim(t_rev.Value & vbNullString) <> vbNullString Then
        selec_rev Trim(t_rev.Value)
    End If
End Sub

Private Sub b_selec_Click()
    If Trim(t_ident.Value & vbNullString) <> vbNullString Then
        selec_fiacao Trim(t_ident.Value)
    End If
    
End Sub


Private Sub t_ini_Change()
    If Not t_ini.Value = "" Then
        If Not IsNumeric(t_ini.Value) Then
            t_ini.Value = "1"
        End If
    End If
End Sub

Private Sub t_ini_Exit(ByVal Cancel As MSForms.ReturnBoolean)
    If Not IsNumeric(t_ini.Value) Then
        t_ini.Value = "1"
    End If
End Sub

Private Sub UserForm_Initialize()
    If ActiveWorkspace.IsConfigurationVariableDefined("FIACAO_DIR") Then
        diretorio = ActiveWorkspace.ConfigurationVariableValue("FIACAO_DIR")
    Else
        diretorio = ""
    End If
    c_preenche.AddItem "Terminal"
    c_preenche.AddItem "Ligação"
    c_preenche.AddItem "Esquematico"
    c_preenche.AddItem "Esq Baixo"
    c_preenche.AddItem "LE"
    c_preenche.AddItem "Revisão"
    c_preenche.AddItem "Secionável"
    c_preenche.ListIndex = 0
    
    Set lista_componentes = Nothing
    num_componentes = 0
    num_ligacoes = 0
End Sub

Private Sub UserForm_Terminate()
    ActiveWorkspace.AddConfigurationVariable "FIACAO_DIR", diretorio, True
End Sub
