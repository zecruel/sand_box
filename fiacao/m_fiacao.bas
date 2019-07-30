Attribute VB_Name = "m_fiacao"
Function sort_crit(el_a As Element, el_b As Element) As Boolean
    ax = el_a.AsCellElement.Origin.X
    bx = el_b.AsCellElement.Origin.X
    ay = el_a.AsCellElement.Origin.Y
    by = el_b.AsCellElement.Origin.Y
    If (bx = ax) Then
        sort_crit = ay < by
    Else
        sort_crit = ax > bx
    End If
End Function

Sub Quicksort(values() As Element, ByVal min As Long, ByVal max As Long)

  Dim med_value As Element
  Dim hi As Long
  Dim lo As Long
  Dim i As Long

  ' If the list has only 1 item, it's sorted.
  If min >= max Then Exit Sub

  ' Pick a dividing item randomly.
  i = (min + max) \ 2
  Set med_value = values(i)

  ' Swap the dividing item to the front of the list.
  Set values(i) = values(min)

  ' Separate the list into sublists.
  lo = min
  hi = max
  Do
    ' Look down from hi for a value < med_value.
    'Do While values(hi).AsCellElement.Origin.Y - values(hi).AsCellElement.Origin.X < med_value.AsCellElement.Origin.Y - med_value.AsCellElement.Origin.X
    Do While sort_crit(values(hi), med_value)
      hi = hi - 1
      If hi <= lo Then Exit Do
    Loop

    If hi <= lo Then
      ' The list is separated.
      Set values(lo) = med_value
      Exit Do
    End If

    ' Swap the lo and hi values.
    Set values(lo) = values(hi)

    ' Look up from lo for a value >= med_value.
    lo = lo + 1
    'Do While values(lo).AsCellElement.Origin.Y - values(lo).AsCellElement.Origin.X >= med_value.AsCellElement.Origin.Y - med_value.AsCellElement.Origin.X
    Do While sort_crit(med_value, values(lo))
      lo = lo + 1
      If lo >= hi Then Exit Do
    Loop

    If lo >= hi Then
      ' The list is separated.
      lo = hi
      Set values(hi) = med_value
      Exit Do
    End If

    ' Swap the lo and hi values.
    Set values(hi) = values(lo)
  Loop ' Loop until the list is separated.

  ' Recursively sort the sublists.
  Quicksort values, min, lo - 1
  Quicksort values, lo + 1, max

End Sub

Function le_fiacao(path As String)
    Dim componentes As New Collection
    Dim componente As Collection

    Dim linha
    
    Open path For Input As #1    ' Open file.
    Do While Not EOF(1)    ' Loop until end of file.
        Line Input #1, linha    ' Read line into variable.
        If Left(linha, 1) = "#" Then
            If Not (componente Is Nothing) Then
                componentes.Add componente
            End If
            Set componente = New Collection
            componente.Add Mid(linha, 2)
            'Debug.Print linha    ' Print to the Immediate window.
        ElseIf Not (componente Is Nothing) Then
            componente.Add Split(linha, ";")
        
        End If
        
    Loop
    Close #1    ' Close file.
    
    If Not (componente Is Nothing) Then
        componentes.Add componente
    End If
    
    Set le_fiacao = componentes

End Function

'Search a element's tag by its name. Return the found TagElement.
Function get_tag_name(el As Element, tag_name As String) As TagElement
    If (el.HasAnyTags) Then
        Dim oTags() As TagElement
        oTags = el.GetTags() 'get all tags in array
        
        For i = LBound(oTags) To UBound(oTags) 'process tag array
            If (oTags(i).TagDefinitionName = tag_name) Then 'check if name match
                Set get_tag_name = oTags(i) 'return current TagElement
                Exit Function
            End If
        Next i
    End If
    Set get_tag_name = Nothing 'if not found, return nothing
End Function

'Search a element's tag by its name and check if its value match.
Function get_tag_value(el As Element, tag_name As String, tag_value As String) As Boolean
    If (el.HasAnyTags) Then
        Dim oTag As TagElement
        
        Set oTag = get_tag_name(el, tag_name) 'get tag by name
        If Not (oTag Is Nothing) Then
            If (oTag.Value = tag_value) Then 'check if tag value match
                get_tag_value = True 'return success
                Exit Function
            End If
        End If
    End If
    get_tag_value = False 'if not found, return fail
End Function
Function GetTagSet(Name As String) As TagSet
    Dim oTagSets As TagSets
    
    Set oTagSets = ActiveDesignFile.TagSets
    On Error Resume Next
    Set GetTagSet = oTagSets(Name)
    If GetTagSet Is Nothing Then
        Dim td As TagDefinition
        Set GetTagSet = oTagSets.Add(Name)
        Set td = GetTagSet.TagDefinitions.Add(Name, msdTagTypeCharacter)
        'td.IsConstant = True
        td.DefaultValue = Name
    End If
End Function

Sub match_tag_attr(dest As TagElement, source As TagElement)
    Dim mtrx As Matrix3d
    Dim Rotation1 As Matrix3d
    Dim Rotation2 As Matrix3d
    Dim scaleFactors As Point3d
    Dim xRot As Double, yRot As Double, zRot As Double, uniformScale As Double
    
    
    
    dest.Font = source.Font
    dest.Height = source.Height
    dest.Justification = source.Justification
    
    
    mtrx = source.Rotation
    Matrix3dIsRotateScaleRotate mtrx, Rotation1, scaleFactors, Rotation2
    Matrix3dIsXRotationYRotationZRotationScale Rotation1, xRot, yRot, zRot, uniformScale
    'Debug.Print "     rotation is " & xRot & ", " & yRot & ", " & zRot
    dest.Rotate source.Origin, xRot, yRot, zRot
    
    dest.Move source.Offset
    dest.Width = source.Width
    dest.Color = source.Color
    dest.Level = source.Level
    dest.LineWeight = source.LineWeight
    dest.Redraw msdDrawingModeNormal
    dest.Rewrite
    
End Sub

Sub executa_fiacao(componentes As Collection, rev As String)
    If Not (componentes Is Nothing) Then
        Dim oEnumerator As ElementEnumerator
        Dim esc As ElementScanCriteria
        
        Set esc = New ElementScanCriteria
        esc.ExcludeAllTypes
        esc.IncludeType msdElementTypeCellHeader
        esc.IncludeType msdElementTypeSharedCell
        
        Set oEnumerator = ActiveModelReference.Scan(esc)
    
        Do While oEnumerator.MoveNext
            
            Dim elemento As Element
            Dim tag As TagElement
            Dim comp As Collection
            Dim term As Collection
            
            Set elemento = oEnumerator.Current
            
            For Each comp In componentes
                If (get_tag_value(elemento, "id", comp("_id_"))) Then
                    
                    For i = 2 To comp.Count
                        
                        Set term = comp(i)
                        If (get_tag_value(elemento, "terminal", CStr(term("terminal")))) Then
                            Set tag = get_tag_name(elemento, "ligacao")
                            If Not (tag Is Nothing) Then
                                tag.Value = term("ligacao")
                                tag.Redraw
                                tag.Rewrite
                                Set tag = get_tag_name(elemento, "rev")
                                If Not (tag Is Nothing) Then
                                    tag.Value = rev
                                    tag.Redraw
                                    tag.Rewrite
                                End If
                                
                                cont = term("cont") + 1
                                term.Remove "cont"
                                term.Add cont, "cont"
                            End If
                            
                            Exit For
                        End If
                    Next i
                    
                    Exit For
                End If
            Next comp
        Loop
    End If
    
End Sub

Sub selec_fiacao(nome As String)
    
    Dim oEnumerator As ElementEnumerator
    Dim esc As ElementScanCriteria
    Dim oTags() As TagElement
    Dim i As Integer
    
    
    Set esc = New ElementScanCriteria
    esc.ExcludeAllTypes
    esc.IncludeType msdElementTypeCellHeader
    esc.IncludeType msdElementTypeSharedCell
    
    ActiveModelReference.UnselectAllElements
    Set oEnumerator = ActiveModelReference.Scan(esc)

    Do While oEnumerator.MoveNext
        
        Dim elemento As Element
        
        Set elemento = oEnumerator.Current
        If (get_tag_value(elemento, "id", nome)) Then
            ActiveModelReference.SelectElement elemento, True
            'seleciona todos tags associados
            If (elemento.HasAnyTags) Then
                oTags = elemento.GetTags() 'get all tags in array
                
                For i = LBound(oTags) To UBound(oTags) 'process tag array
                    If Not (oTags(i).AsTagElement.IsHidden) Then
                        ActiveModelReference.SelectElement oTags(i), True
                    End If
                Next i
            End If
        End If
    Loop
    
End Sub

Sub selec_rev(rev As String)
    
    Dim oEnumerator As ElementEnumerator
    Dim esc As ElementScanCriteria
    
    Set esc = New ElementScanCriteria
    esc.ExcludeAllTypes
    esc.IncludeType msdElementTypeCellHeader
    esc.IncludeType msdElementTypeSharedCell
    
    ActiveModelReference.UnselectAllElements
    Set oEnumerator = ActiveModelReference.Scan(esc)

    Do While oEnumerator.MoveNext
        
        Dim elemento As Element
        
        Set elemento = oEnumerator.Current
        If (get_tag_value(elemento, "rev", rev)) Then
            ActiveModelReference.SelectElement elemento, True
            'seleciona todos tags associados
            If (elemento.HasAnyTags) Then
                oTags = elemento.GetTags() 'get all tags in array
                
                For i = LBound(oTags) To UBound(oTags) 'process tag array
                    If Not (oTags(i).AsTagElement.IsHidden) Then
                        ActiveModelReference.SelectElement oTags(i), True
                    End If
                Next i
            End If
            
        End If
    Loop
    
End Sub

Sub fiacao_identifica(nome As String)
    
    Dim id As TagElement
    Dim ee As ElementEnumerator
    
    contador = inicial
    Set ee = ActiveModelReference.GetSelectedElements
    Do While ee.MoveNext
        If ee.Current.Type = msdElementTypeCellHeader Or ee.Current.Type = msdElementTypeSharedCell Then
            Set id = get_tag_name(ee.Current, "id")
            If Not (id Is Nothing) Then
                id.Value = nome
                id.Redraw
                id.Rewrite
            End If
        End If
    Loop
End Sub

Sub numera_sequencial(Optional ByVal nome_tag As String = "terminal", _
    Optional ByVal inicial As Integer = 1, _
    Optional ByVal prefixo As String = "", _
    Optional ByVal sufixo As String = "")
    
    Dim id As TagElement
    Dim ee As ElementEnumerator
    Dim contador As Integer
    Dim i As Integer
    Dim lista() As Element
    
    'cria uma lista com os elementos selecionados
    i = 0
    Set ee = ActiveModelReference.GetSelectedElements
    Do While ee.MoveNext
        ' na lista so existem elementos do tipo cell
        If ee.Current.Type = msdElementTypeCellHeader Or ee.Current.Type = msdElementTypeSharedCell Then
            ReDim Preserve lista(i + 1)
            Set lista(i) = ee.Current
            i = i + 1
        End If
    Loop
    If Not Not lista Then
        'ordena a lista pela posicao do elemento (de cima para baixo e da esqueda para direita)
        Quicksort lista, LBound(lista), UBound(lista) - 1
        
        'executa a numeracao, varrendo a lista de elementos ordenada
        contador = inicial
        For i = LBound(lista) To UBound(lista) - 1
            Set id = get_tag_name(lista(i), nome_tag)
            If Not (id Is Nothing) Then
                id.Value = prefixo & CStr(contador) & sufixo
                id.Redraw
                id.Rewrite
                
                contador = contador + 1
            End If
        Next i
    End If
End Sub

Sub numera_sequencial_secionavel(Optional ByVal inicial As Integer = 1, _
    Optional ByVal prefixo As String = "", _
    Optional ByVal sufixo As String = "")
    
    Dim id As TagElement
    Dim ee As ElementEnumerator
    Dim cont_e As Integer
    Dim cont_s As Integer
    Dim cont_c As Integer
    Dim i As Integer
    Dim lista() As Element
    
    'cria uma lista com os elementos selecionados
    i = 0
    Set ee = ActiveModelReference.GetSelectedElements
    Do While ee.MoveNext
        ' na lista so existem elementos do tipo cell
        If ee.Current.Type = msdElementTypeCellHeader Or ee.Current.Type = msdElementTypeSharedCell Then
            ReDim Preserve lista(i + 1)
            Set lista(i) = ee.Current
            i = i + 1
        End If
    Loop
    If Not Not lista Then
        'ordena a lista pela posicao do elemento (de cima para baixo e da esqueda para direita)
        Quicksort lista, LBound(lista), UBound(lista) - 1
        
        'executa a numeracao, varrendo a lista de elementos ordenada
        cont_e = inicial
        cont_s = inicial
        cont_c = inicial
        For i = LBound(lista) To UBound(lista) - 1
            If lista(i).AsCellElement.Name = "sec_e" Then
                Set id = get_tag_name(lista(i), "terminal")
                If Not (id Is Nothing) Then
                    id.Value = prefixo & CStr(cont_e) & sufixo & "E"
                    id.Redraw
                    id.Rewrite
                    
                    cont_e = cont_e + 1
                End If
            ElseIf lista(i).AsCellElement.Name = "sec_s" Then
                Set id = get_tag_name(lista(i), "terminal")
                If Not (id Is Nothing) Then
                    id.Value = prefixo & CStr(cont_s) & sufixo & "S"
                    id.Redraw
                    id.Rewrite
                    
                    cont_s = cont_s + 1
                End If
            ElseIf lista(i).AsCellElement.Name = "sec_central" Then
                Set id = get_tag_name(lista(i), "num")
                If Not (id Is Nothing) Then
                    id.Value = prefixo & CStr(cont_c) & sufixo
                    id.Redraw
                    id.Rewrite
                    
                    cont_c = cont_c + 1
                End If
            End If
        Next i
    End If
End Sub

Sub numera_lista(lista() As String, Optional ByVal nome_tag As String = "terminal")
    
    Dim id As TagElement
    Dim ee As ElementEnumerator
    Dim contador As Integer
    Dim ini As Integer
    Dim fim As Integer
    Dim i As Integer
    Dim lista_el() As Element
    
    'cria uma lista com os elementos selecionados
    i = 0
    Set ee = ActiveModelReference.GetSelectedElements
    Do While ee.MoveNext
        ' na lista so existem elementos do tipo cell
        If ee.Current.Type = msdElementTypeCellHeader Or ee.Current.Type = msdElementTypeSharedCell Then
            ReDim Preserve lista_el(i + 1)
            Set lista_el(i) = ee.Current
            i = i + 1
        End If
    Loop
    If (Not Not lista_el) And (UBound(lista) >= 0) Then
        'ordena a lista pela posicao do elemento (de cima para baixo e da esqueda para direita)
        Quicksort lista_el, LBound(lista_el), UBound(lista_el) - 1
        
        ini = LBound(lista)
        fim = UBound(lista)
        contador = ini
        'executa a numeracao, varrendo a lista de elementos ordenada
        For i = LBound(lista_el) To UBound(lista_el) - 1
            Set id = get_tag_name(lista_el(i), nome_tag)
            If Not (id Is Nothing) Then
                If Trim(lista(contador) & vbNullString) <> vbNullString Then
                    id.Value = Trim(lista(contador))
                    id.Redraw
                    id.Rewrite
                End If
                contador = contador + 1
                If contador > fim Then contador = ini
            End If
        Next i
    End If
End Sub

Private Function check_comp(col As Collection, key As String) As Boolean
    On Error GoTo compNotFound
    Dim itm
    Set itm = col(key)
    check_comp = True
    Exit Function
compNotFound:
    check_comp = False
    Exit Function
End Function

Private Function check_term(col As Collection, key As String) As Boolean
    On Error GoTo termNotFound
    Dim itm
    itm = col(key)
    check_term = True
    Exit Function
termNotFound:
    check_term = False
    Exit Function
End Function
Function carrega_fiacao(arquivo As String) As Collection
    Dim linha
    Dim ligacoes As Collection
    Dim celulas
    Dim pares As New Collection
    Dim componentes As New Collection
    Dim componente As Collection
    Dim campos(0 To 1) As String
    Dim ident As String
    Dim terminal As String
    Dim lig As Collection
    
    
    '--------------------------------------------------------------------------------------------------------
    ' Abre o arquivo separado por virgulas (.csv) e monta os pares de ligaçoes um uma colecao
    '--------------------------------------------------------------------------------------------------------
    Open arquivo For Input As #1 'abre o arquivo
    Do While Not EOF(1)    ' varre o arquivo até o fim
        Line Input #1, linha    ' le uma linha
        celulas = Split(linha, ";") ' monta um vetor com os valores separados por ponto-virgula (;)
        'If (UBound(celulas) > 0) Then ' deve haver pelo menos um par
            Set ligacoes = New Collection
            For i = 0 To UBound(celulas)
                If Trim(celulas(i) & vbNullString) <> vbNullString Then ligacoes.Add celulas(i)
            Next i
            For i = 2 To ligacoes.Count
                pares.Add Array(ligacoes(i - 1), ligacoes(i)) ' monta os pares
            Next i
        'End If
    Loop
    Close #1    ' fecha o arquivo.
    
    
    '--------------------------------------------------------------------------------------------------------
    ' cadastra os componentes em uma colecao e seus respectivos terminais em uma sub-colecao
    '--------------------------------------------------------------------------------------------------------
    For Each par In pares ' varre os pares
    
        For i = 0 To 1 ' analiza cada elemento do par
            ' separa a identificacao do componete e o terminal, separados pelo "."
            campos(0) = Left(par(i), InStr(par(i), ".") - 1)
            campos(1) = Right(par(i), Len(par(i)) - InStr(par(i), "."))
            
            If (UBound(campos) > 0) Then
                ident = UCase(campos(0))
                terminal = UCase(campos(1))
                Set componente = Nothing
                If Not (check_comp(componentes, "id_" & ident)) Then
                    Set componente = New Collection
                    componente.Add ident, "_id_"
                    componentes.Add componente, "id_" & ident
                Else
                    Set componente = componentes("id_" & ident)
                End If
                
                If Not (componente Is Nothing) Then
                    'If Not (check_term(componente, "t_" & terminal)) Then
                        'componente.Add Array(terminal, UCase(par(1 - i))), "t_" & terminal
                    If Not (check_comp(componente, "t_" & terminal)) Then
                        Set lig = New Collection
                        lig.Add terminal, "terminal"
                        lig.Add UCase(par(1 - i)), "ligacao"
                        lig.Add 0, "cont"
                        
                        componente.Add lig, "t_" & terminal
                    Else
                        Set lig = componente("t_" & terminal)
                        texto = lig("ligacao") & "/" & UCase(par(1 - i))
                        lig.Remove ("ligacao")
                        lig.Add texto, "ligacao"
                    End If
                End If
                
            End If
        Next i
    Next par
    
    Set carrega_fiacao = componentes
End Function


Sub abre_janela()
    Load f_fiacao
    f_fiacao.Show 0
    
End Sub
