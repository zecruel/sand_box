(async () => {
function julian_date (time) { return 2440587.5 + time / 86400; };
//console.log(julian_date(Math.floor(Date.now()/1000))); // Prints the current time in milliseconds

let locale = await w2utils.locale('./w2ui/pt-br.json');

const comp_term_dados = [];
let pausa = false;
setInterval(atualiza, 2000);
let changes = -1;
let db_changes = 0;
let comp_term_init = false;
let proj_atualiza = false;
let paineis_atualiza = false;
let componentes_atualiza = false;

const cor_claro = [
  '#909090',
  '#ffccff',
  '#ffcccc',
  '#ffffcc',
  '#ccffcc',
  '#ccffff',
  '#ccccff',
];

const cor_escuro = [
  '#cccccc',
  '#ff90ff',
  '#ff9090',
  '#ffff90',
  '#90ff90',
  '#90ffff',
  '#9090ff',
];


window.resizeTo(1300, 700);

const tab = '\
<div id="area_princ"> \
    <div id="componentes_terminais" class="area_princ_cont"></div> \
    <div id="descr_projeto" class="area_princ_cont" style="width: 1000px; height: 350px; text-align: left; padding-right: 1px; padding-left: 1px; margin-right: 1px; margin-left: 1px;"></div> \
    <div id="lista_paineis" class="area_princ_cont" style="width: 1000px; height: 350px; text-align: left; padding-right: 1px; padding-left: 1px; margin-right: 1px; margin-left: 1px;"></div> \
    <div id="lista_componentes" class="area_princ_cont" style="width: 1000px; height: 350px; text-align: left; padding-right: 1px; padding-left: 1px; margin-right: 1px; margin-left: 1px;"></div> \
    <div id="tab3" class="area_princ_cont"> \
      What did you expect, of course it is the third tab. \
    </div> \
</div>';

let pstyle = 'border: 1px solid #efefef; padding: 5px; color: black;'
let princ_layout = new w2layout({
    box: '#princ_layout',
    name: 'princ_layout',
    panels: [
        { type: 'top', size: 50, style: pstyle, html: 'Edição dos terminais' },
        { type: 'left', size: 200, style: pstyle, html: 'left' },
        { type: 'main', html: tab }
    ]
});

//<i class="fa-solid fa-list-check"></i>
//<i class="fa-light fa-file-lines"></i>
//<i class="fa-light fa-list-ol"></i>
//<i class="fa-solid fa-toilets-portable"></i>
//<i class="fa-solid fa-kaaba"></i>
//<i class="fa-solid fa-computer"></i>
//<i class="fa-solid fa-pen-to-square"></i>
//<i class="fa-solid fa-sliders"></i>
//<i class="fa-solid fa-code-compare"></i>
//<i class="fa-solid fa-database"></i>
//<i class="fa-solid fa-network-wired"></i>
//<i class="fa-solid fa-tags"></i>
//<i class="fa-solid fa-server"></i>
//<i class="fa-solid fa-file-invoice"></i>
//<i class="fa-solid fa-marker"></i
//<i class="fa-solid fa-cube"></i>
//<i class="fa-solid fa-code-branch"></i>
//<i class="fa-solid fa-receipt"></i>
//<i class="fa-solid fa-keyboard"></i>
//<i class="fa-solid fa-truck-monster"></i>
//<i class="fa-solid fa-route"></i>
//<i class="fa-solid fa-dragon"></i>
//<i class="fa-solid fa-xmarks-lines"></i>
//<i class="fa-solid fa-trash-can"></i>
//<i class="fa-solid fa-toilet-portable"></i>
//<i class="fa-solid fa-timeline"></i>
//<i class="fa-solid fa-tachograph-digital"></i>
//<i class="fa-solid fa-table-list"></i>
//<i class="fa-solid fa-square-poll-horizontal"></i>
//<i class="fa-solid fa-screwdriver-wrench"></i>
//<i class="fa-solid fa-shapes"></i>
//<i class="fa-solid fa-ruler-combined"></i>
//<i class="fa-solid fa-pallet"></i>
//<i class="fa-solid fa-folder-tree"></i>
//<i class="fa-solid fa-chart-gantt"></i>



let sidebar = new w2sidebar({
  name: 'sidebar',
  flatButton: true,
  nodes: [
      { id: 'paleta', text: 'Editar', group: true, expanded: true, groupShowHide: false,
          nodes: [
            { id: 'projeto', text: 'Projeto', icon: "fa-solid fa-file-invoice", selected: true },
            { id: 'componentes', text: 'Componentes', icon: "fa-solid fa-server"},
            { id: 'paineis', text: 'Paineis', icon: "fa-solid fa-toilets-portable"},
            { id: 'terminais', text: 'Terminais', icon: 'fa-solid fa-microchip'},
            { id: 'bornes', text: 'Bornes', icon: "fa-solid fa-code-compare" },
            { id: 'variaveis', text: 'Variáveis', icon: "fa-solid fa-keyboard" }
          ],
          onCollapse(event) {
            event.preventDefault()
          }
      }
  ],
  onFlat(event) {
      princ_layout.sizeTo('left', (event.detail.goFlat ? 35 : 200), true)
  },
  onClick(event) {
    $('#area_princ .area_princ_cont').hide();
    switch (event.target) {
      case 'projeto':
        $('#area_princ #descr_projeto').show();
        princ_layout.html('top', 'Identificação do projeto');
        break
      case 'paineis':
        $('#area_princ #lista_paineis').show();
        princ_layout.html('top', 'Identificação dos painéis');
        break
      case 'terminais':
        $('#area_princ #componentes_terminais').show();
        princ_layout.html('top', 'Edição dos terminais');
        break
      case 'componentes':
        $('#area_princ #lista_componentes').show();
        princ_layout.html('top', 'Edição dos componentes');
        break
      case 'bornes':
        $('#area_princ #descr_projeto').show();
        princ_layout.html('top', 'Edição dos bornes');
        break
      case 'variaveis':
        $('#area_princ #tab3').show();
        //princ_layout.html('main', '<div style="padding: 10px">Some HTML</div>')
        //query(princ_layout.el('main'))
         //   .css('border-left', '1px solid #efefef')
        princ_layout.html('top', 'Edição dos variáveis dos IED\'s (entradas/saídas)');
        break
    }
  }
});


let projeto_form = new w2form({
  box: '#descr_projeto',
  name: 'descr_projeto',
  fields: [
    { field: 'titulo', type: 'text', required: true,
      html: {
        label: 'Título',
        column: 0,
        attr: 'style="width: 400px"'
      }
    },
    { field: 'rev',  type: 'text', required: true,
      html: {
        label: 'Rev.',
        column: 1,
        attr: 'style="width: 60px"'
      }
    },
    { field: 'data',  type: 'text', required: true,
      html: {
        label: 'Data',
        column: 2,
        attr: 'style="width: 80px"'
      }
    },
    { field: 'descr', type: 'textarea',
      html: {
        label: 'Descrição',
        column: 0,
        attr: 'style="width: 400px; height: 60px"'
      },
    },
    { field: 'aplicacao',  type: 'text', required: true,
      html: {
        label: 'Aplic.',
        column: 1,
        attr: 'style="width: 60px"'
      }
    },
    { field: 'codigo',  type: 'text',
      html: {
        label: 'Cód.',
        column: 2,
        attr: 'style="width: 80px"'
      }
    },
    { field: 'instalacao',  type: 'text', required: true,
      html: {
        label: 'Instalação',
        column: 0,
        attr: 'style="width: 300px"'
      }
    },
    { field: 'projetista',  type: 'text',
      html: {
        label: 'Proj.',
        column: 0,
        attr: 'style="width: 300px"'
      }
    },
    { field: 'visto',  type: 'text',
      html: {
        label: 'Visto',
        column: 0,
        attr: 'style="width: 300px"'
      }
    },
    { field: 'aprovacao',  type: 'text',
      html: {
        label: 'Aprov.',
        column: 0,
        attr: 'style="width: 300px"'
      }
    },
    
  ],
  record : {
    titulo: '-',
    instalacao: '-',
    rev: '-',
    data: '-',
    aplicacao: '-'
    
  },
  actions: {
    Atualiza(event) {
      proj_atualiza = false;
    },
    Salva(event) {
      if (projeto_form.validate().length == 0) {
        let comando = "UPDATE projeto SET valor = CASE chave " +
          "WHEN 'titulo' THEN '" + this.getValue('titulo') + "' " +
          "WHEN 'instalacao' THEN '" + this.getValue('instalacao') + "' " +
          "WHEN 'aplicacao' THEN '" + this.getValue('aplicacao') + "' " +
          "WHEN 'codigo' THEN '" + this.getValue('codigo') + "' " +
          "WHEN 'rev' THEN '" + this.getValue('rev') + "' " +
          "WHEN 'aprovacao' THEN '" + this.getValue('aprovacao') + "' " +
          "WHEN 'visto' THEN '" + this.getValue('visto') + "' " +
          "WHEN 'projetista' THEN '" + this.getValue('projetista') + "' " +
          "WHEN 'descr' THEN '" + this.getValue('descr') + "' " +
          "WHEN 'data' THEN '" + this.getValue('data') + "' " +
          "ELSE valor END WHERE chave IN('titulo', 'instalacao', " +
          "'aplicacao', 'codigo', 'rev', 'aprovacao', 'visto', " +
          "'projetista', 'descr', 'data');";
        webui_fn('Sqlite_exec', comando).then((response) => {
          if (response) {
            proj_atualiza = false;
          }
        });
      }
    }
  }
});



let tabela_paineis = new w2grid({
  name: 'tabela_paineis',
  box: '#lista_paineis',
  show: {
    toolbar: true,
    lineNumbers: true,
    footer: true,
    toolbarAdd: true,
    toolbarDelete: true,
    toolbarEdit: true
  },
  columns: [
    { field: 'id', text: 'ID', size: '15%', sortable: true, searchable: true, editable: { type: 'text' } },
    { field: 'titulo', text: 'Título', size: '30%', sortable: true, searchable: true, editable: { type: 'text' } },
    { field: 'descr', text: 'Descrição', size: '50%', editable: { type: 'text' } },
    { field: 'fiacao', text: 'Fia', size: '5%', sortable: true, editable: { type: 'checkbox', style: 'text-align: center' } },
    { field: 'existe', text: 'Esq', size: '5%', sortable: true, style: 'text-align: center'},
  ],
  onAdd: function (event) {
    console.log('add', event);
  },
  onEdit: function (event) {
    console.log('edit', event);
  },
  onDelete: function (event) {
    console.log('delete has default behavior');
  },
  onReload: function (event) {
    console.log('atualiza');
    paineis_atualiza = false;
  },
  records: []
});

tabela_paineis.on('change', function(event) {
  if (event.target == 'tabela_paineis'){
    console.log(event);
    paineis_atualiza = false;
  }
});

let tabela_componentes = new w2grid({
  name: 'tabela_componentes',
  box: '#lista_componentes',
  show: {
    toolbar: true,
    lineNumbers: true,
  },
  columns: [
    { field: 'painel', text: 'Painel', size: '15%', sortable: true, searchable: true },
    { field: 'componente', text: 'Componente', size: '25%', sortable: true, searchable: true },
    { field: 'item', text: 'Item LE', size: '10%', sortable: true, searchable: true },
    { field: 'id_fiacao', text: 'Id Fiação', size: '20%', sortable: true, searchable: true },
    { field: 'tipo', text: 'Tipo', size: '25%', sortable: true, searchable: true },
    { field: 'existe', text: 'Esq', size: '5%', sortable: true, style: 'text-align: center'},
  ],
  toolbar: {
    items: [
      { id: 'add', type: 'button', text: 'Add Record', icon: 'w2ui-icon-plus' },
      { type: 'break' },
      { type: 'button', id: 'showChanges', text: 'Show Changes' }
    ],
    onClick(event) {
      //console.log(event);
      if (event.target == 'w2ui-reload') {
        //console.log('atualiza');
        componentes_atualiza = false;
      }
      if (event.target == 'add') {
        let recid = this.owner.records.length + 1
        this.owner.add({ recid });
        this.owner.scrollIntoView(recid);
        this.owner.editField(recid, 0)
      }
      if (event.target == 'showChanges') {
        
      }
    }
  },
  records: [
  ]
});

tabela_componentes.on('change', function(event) {
  if (event.target == 'tabela_componentes'){
    console.log(event);
    paineis_atualiza = false;
  }
});

//projeto_form.on('change', function (event) {
//  console.log('Target: '+ event.target, event.detail.value.current);
//});

princ_layout.html('left', sidebar);
$('#area_princ .area_princ_cont').hide()
$('#area_princ #descr_projeto').show()
princ_layout.html('top', 'Identificação do projeto');

const comp_term_mod = function(instance, cell, col, lin, value) {
  if (comp_term_init) {
    //const cellName = jspreadsheet.getColumnNameFromId([col,lin]);
    //console.log('The cell ' + cellName + ' will be changed =' + value + '-' + col + '-' + lin);
    let y = Number(lin) + 1;
    unico = comp_term.getMeta('H' + y).id;
  
    if (col == 7) { // modificando o terminal
      webui_fn('Sqlite_exec', "UPDATE terminais_esq SET terminal = '" + value +
        "' WHERE componente = " + unico + " AND id = " + comp_term_dados[lin][6] +
        ";").then((response) => {
        if (response) {
          //const obj = JSON.parse(response);
          //console.log(obj);
          let time = Math.floor(Date.now()/1000);
          let jd = julian_date(time);
          
          webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
          ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
            }
          });
        }
      });
    }
    
    if (col == 3) { // modificando a parte
      webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value +
        "' WHERE unico = " + unico + ";").then((response) => {
        if (response) {
          //const obj = JSON.parse(response);
          //console.log(obj);
          let time = Math.floor(Date.now()/1000);
          let jd = julian_date(time);
          
          webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
          ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
            }
          });
        }
      });
    }
  }
};

const comp_term_undo = function(instance, obj) {
  if (obj){
    if (obj.action == "setValue"){
      obj.records.forEach(function(value, index, array) {
        let y = Number(value.y) + 1;
        unico = comp_term.getMeta('H' + y).id;
        //console.log(value.x, value.y, value.oldValue);
        if (value.x == 7) { // modificando o terminal
          webui_fn('Sqlite_exec', "UPDATE terminais_esq SET terminal = '" + value.oldValue +
            "' WHERE componente = " + unico + " AND id = " + comp_term_dados[value.y][6] +
            ";").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
              let time = Math.floor(Date.now()/1000);
              let jd = julian_date(time);
              
              webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
              ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
                if (response) {
                  //const obj = JSON.parse(response);
                  //console.log(obj);
                }
              });
            }
          });
        }
        
        if (value.x == 3) { // modificando a parte
          webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value.oldValue +
            "' WHERE unico = " + unico + ";").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
              let time = Math.floor(Date.now()/1000);
              let jd = julian_date(time);
              
              webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
              ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
                if (response) {
                  //const obj = JSON.parse(response);
                  //console.log(obj);
                }
              });
            }
          });
        }
      });
    }
  }
};

const comp_term_redo = function(instance, obj) {
  if (obj){
    if (obj.action == "setValue"){
      obj.records.forEach(function(value, index, array) {
        //console.log(value.x, value.y, value.newValue);
        let y = Number(value.y) + 1;
        unico = comp_term.getMeta('H' + y).id;
        if (value.x == 7) { // modificando o terminal
          webui_fn('Sqlite_exec', "UPDATE terminais_esq SET terminal = '" + value.newValue +
            "' WHERE componente = " + unico + " AND id = " + comp_term_dados[value.y][6] +
            ";").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
              let time = Math.floor(Date.now()/1000);
              let jd = julian_date(time);
              
              webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
              ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
                if (response) {
                  //const obj = JSON.parse(response);
                  //console.log(obj);
                }
              });
            }
          });
        }
        
        if (value.x == 3) { // modificando a parte
          webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value.newValue +
            "' WHERE unico = " + unico + ";").then((response) => {
            if (response) {
              //const obj = JSON.parse(response);
              //console.log(obj);
              let time = Math.floor(Date.now()/1000);
              let jd = julian_date(time);
              
              webui_fn('Sqlite_exec', "UPDATE arquivos SET modificado = " + time +
              ", jd = " + jd + " WHERE caminho = 'projeto.db';").then((response) => {
                if (response) {
                  //const obj = JSON.parse(response);
                  //console.log(obj);
                }
              });
              
            }
            
          });
        }
      });
    }
  }
};


const comp_term_load = function(instance) {
  if (comp_term_init) return;
  let comp_ant = "";
  let unico_ant = "";
  let tipo_ant = "";
  let parte_ant = "";
  let modulo_ant = "";
  let painel_ant = "", unico = "";

  let ini_comp = 1;
  let ini_unico = 1;
  let ini_modulo = 1;
  let ini_painel = 1;
  
  let paineis = 0, unicos = 0, componentes = 0, modulos = 0;
  let y = 1;
  let claro = cor_escuro;
  let cor_uni = claro[0];
  
  if (comp_term_dados[0]) {
    painel_ant = comp_term_dados[0][0];
    comp_ant = comp_term_dados[0][1];
    unico_ant = comp_term.getMeta('H1').id;
    parte_ant = comp_term_dados[0][3];
    modulo_ant = comp_term_dados[0][2];
    
    let celula = comp_term.getCell("A1");
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
    
    celula = comp_term.getCell("B1");
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
    
    celula = comp_term.getCell("C1");
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
    
    celula = comp_term.getCell("D1");
    celula.style.backgroundColor = cor_uni;
    if (parte_ant !== ""){
      celula.classList.remove('readonly');
      celula.style.color = "#000";
    }
    else {
      celula.classList.add('readonly');
      celula.style.color = "#900";
    }
    
    celula = comp_term.getCell("E1");
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
    
    celula = comp_term.getCell("F1");
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
  }
  
  //agrupa as celulas repetidas
  for (let linha of comp_term_dados){
    if ((paineis % 2) == 0){
      claro = cor_claro;
    }
    else {
      claro = cor_escuro;
    }
    
    unico = comp_term.getMeta('H' + y).id;
    
    if (painel_ant != linha[0]){
      if ((y - ini_painel) > 1) {
        comp_term.setMerge("A"+ini_painel, 1, y - ini_painel);
      }
      ini_painel = y;
      
      let cor = claro[paineis % 7];
      
      const cel = comp_term.getCell("A" + y);
      cel.style.backgroundColor = cor;
      cel.classList.add('readonly');
      cel.style.color = "#900";
      
      paineis++;
      unicos = 0;
      componentes = 0;
      modulos = 0;
    }
    
    if (painel_ant != linha[0] || comp_ant != linha[1]){
      if ((y - ini_comp) > 1) {
        comp_term.setMerge("B" + ini_comp, 1, y - ini_comp);
      }
      ini_comp = y;
      
      let cor = claro[componentes % 7];
      
      let cel = comp_term.getCell("B" + y);
      cel.style.backgroundColor = cor;
      cel.classList.add('readonly');
      cel.style.color = "#900";
      componentes++;
    }
    
    if (painel_ant != linha[0] || comp_ant != linha[1] || modulo_ant != linha[2]){
      if ((y - ini_modulo) > 1) {
        comp_term.setMerge("C" + ini_modulo, 1, y - ini_modulo);
      }
      ini_modulo = y;
      
      let cor = claro[modulos % 7];
      
      let cel = comp_term.getCell("C" + y);
      cel.style.backgroundColor = cor;
      cel.classList.add('readonly');
      cel.style.color = "#900";
      modulos++;
    }
    
    //o ID unico eh o criterio para agrupar blocos e partes
    if (unico_ant != unico){
      if ((y - ini_unico) > 1) {
        comp_term.setMerge("D"+ini_unico, 1, y - ini_unico);
        comp_term.setMerge("E"+ini_unico, 1, y - ini_unico);
        comp_term.setMerge("F"+ini_unico, 1, y - ini_unico);
      }
      ini_unico = y;
      cor_uni = claro[unicos % 7];
      
      
      let cel = comp_term.getCell("D"+y);
      cel.style.backgroundColor = cor_uni;
      if (linha[3] !== ""){
        cel.classList.remove('readonly');
        cel.style.color = "#000";
      }
      else {
        cel.classList.add('readonly');
        cel.style.color = "#900";
      }
      
      cel = comp_term.getCell("E"+y);
      cel.style.backgroundColor = cor_uni;
      cel.classList.add('readonly');
      cel.style.color = "#900";
      
      cel = comp_term.getCell("F"+y);
      cel.style.backgroundColor = cor_uni;
      cel.classList.add('readonly');
      cel.style.color = "#900";
      
      unicos++;
    }
    
    let celula = comp_term.getCell("G" + y);
    celula.style.backgroundColor = cor_uni;
    celula.classList.add('readonly');
    celula.style.color = "#900";
    
    celula = comp_term.getCell("H" + y);
    celula.style.backgroundColor = cor_uni;
    //celula.classList.add('readonly');
    //celula.style.color = "#900";
    
    comp_ant = linha[1];
    unico_ant = unico;
    //tipo_ant = linha.tipo;
    parte_ant = linha[3];
    modulo_ant = linha[2];
    painel_ant = linha[0];//.painel;
    
    
    
    y++;
  }
  //finaliza os agrupamentos pendentes
  if ((y - ini_painel) > 1) {
    comp_term.setMerge("A"+ini_painel, 1, y - ini_painel);
  }
  if ((y - ini_comp) > 1) {
    comp_term.setMerge("B" + ini_comp, 1, y - ini_comp);
  }
  if ((y - ini_modulo) > 1) {
    comp_term.setMerge("C" + ini_modulo, 1, y - ini_modulo);
  }
  if ((y - ini_unico) > 1) {
    comp_term.setMerge("D"+ini_unico, 1, y - ini_unico);
    comp_term.setMerge("E"+ini_unico, 1, y - ini_unico);
    comp_term.setMerge("F"+ini_unico, 1, y - ini_unico);
  }
  
  comp_term_init = true;
  
  if (comp_term_dados[0]) {
    // limpa o histórico de undo/redo
    comp_term.history = [];
    comp_term.historyIndex = -1;
  }
};

const comp_term = jspreadsheet(document.getElementById('componentes_terminais'), {
    data:comp_term_dados,
    columns:[
        { title:'Painel', width:120 },
        { title:'Componente', width:200 },
        { title:'Módulo', width:120 },
        { title:'Parte', width:120 },
        { title:'Bloco', width:120 },
        { title:'Tipo', width:120 },
        { title:'Num', width:50 },
        { title:'Term', width:80 },
    ],
    onbeforechange: comp_term_mod,
    onundo: comp_term_undo,
    onredo: comp_term_redo,
    onload: comp_term_load,
});



function teste() {
  const MyInput = document.getElementById('teste_input');
  //document.getElementById("proj_tit").innerHTML = "Projeto: ";
  //document.getElementById("proj_descr.innerHTML = "Descrição: <em>-</em>";
  //webui_fn('Sqlite_exec', MyInput.value);
  webui_fn('Sqlite_exec', MyInput.value).then((response) => {
      //document.getElementById("demo").innerHTML = response;
      //webui_fn('MyID_One', response);
      const obj = JSON.parse(response);
      /*
      tabela_dados.splice(0,tabela_dados.length); //limpa a tabela
      for (let linha of obj.data) {
        const novo = new Object();
        for (let coluna in linha) {
          novo[coluna] = linha[coluna];
        }
        tabela_dados.push(novo);
      }
    */
      //tabela_dados = obj.data;
      
      document.getElementById("demo").innerHTML = obj.data[0].data_version;
  });
  
  //webui_fn('MyID_One', tabledata[0].name);
  //tabledata[0].name = MyInput.value;
  //tabledata[0].name = "teste";
  //webui_fn('MyID_One', tabledata[0].name);
};

function atualiza() {
  if(!pausa){
    if (changes != -1){
      webui_fn('Sqlite_exec', "PRAGMA data_version;").then((response) => {
        if (response) {
          const obj = JSON.parse(response);
          db_changes = Number(obj.data[0].data_version);
          
        }
        
      });
    }
    //console.log(db_changes, changes, db_changes > changes);
    if (db_changes != changes){
      
      //console.log("Atualiza "+ db_changes);
      webui_fn('Sqlite_exec', "SELECT * FROM comp_term;").then((response) => {
        if (response) {
          //console.log(response);
          const obj = JSON.parse(response);
          
          comp_term_dados.splice(0, comp_term_dados.length); //limpa a tabela
          /* reseta as meta informações */
          const all_meta = comp_term.getMeta();
          if (all_meta){
            for (let cel in all_meta) {
              all_meta[cel] = {};
            }
            comp_term.setMeta(all_meta);
          }
          comp_term.destroyMerged();
          
          let y = 1; //numero da linha
          for (let linha of obj.data) {
            const novo = [
              linha.painel,
              linha.componente,
              linha.modulo,
              linha.parte,
              linha.bloco,
              linha.tipo,
              linha.num,
              linha.terminal
            ];
            comp_term_dados.push(novo);
            
            comp_term.setMeta('H' + y, 'id', linha.unico);
            
            
            
            
            
            
            y = y + 1;
          }
          webui_fn('Sqlite_exec', "PRAGMA data_version;").then((response) => {
            if (response) {
              const obj = JSON.parse(response);
              db_changes = Number(obj.data[0].data_version);
              changes = db_changes;
              //console.log("Atualiza"+ db_changes);
            }
            //console.log(response);
          });
          
          /* atualiza a exibição */
          comp_term_init = false;
          comp_term.setData();
          
        }
      });
      
      
      
      
      
    }
    
    if (!proj_atualiza){
      //const proj_id = new Object();

      webui_fn('Sqlite_exec', "SELECT * FROM projeto;").then((response) => {
        if (response) {
          //console.log(response);
          const obj = JSON.parse(response);
          
          for (let linha of obj.data) {
            projeto_form.record[linha.chave] = linha.valor;
            //console.log(proj_id[linha.chave]);
          }
          //console.log(projeto_form.record);
          projeto_form.refresh();
          
          document.getElementById("proj_tit").innerHTML = "Projeto: " + projeto_form.record.titulo;
          document.getElementById("proj_descr").innerHTML = "Descrição: <em>"+ projeto_form.record.descr +"</em>";
          
          proj_atualiza = true;
        }
      });
    }
    
    if (!paineis_atualiza){
      //const proj_id = new Object();
      let com = "WITH cte_paineis AS ( SELECT DISTINCT painel FROM descr_comp ) " +
        "SELECT paineis_total.painel, titulo, descr, fiacao, " +
        "CASE WHEN EXISTS ( SELECT cte_paineis.painel FROM cte_paineis " +
        "WHERE cte_paineis.painel = paineis_total.painel) THEN 'x' END existe " +
        "FROM ( SELECT painel FROM cte_paineis UNION SELECT id FROM paineis) " +
        "AS paineis_total LEFT JOIN paineis ON painel = id ORDER BY painel;";
      
      webui_fn('Sqlite_exec', com).then((response) => {
        if (response) {
          //console.log(response);
          const obj = JSON.parse(response);
          let recid = tabela_paineis.last.sel_recid;
          tabela_paineis.mergeChanges();
          tabela_paineis.clear(); //limpa a tabela
          let num = 1;
          for (let linha of obj.data) {
            let fiacao = linha.fiacao != "";
            tabela_paineis.add({
              recid: num,
              id: linha.painel,
              titulo: linha.titulo,
              descr: linha.descr,
              fiacao: fiacao,
              existe: linha.existe
            });
            num++;
          }
          tabela_paineis.refresh();
          tabela_paineis.scrollIntoView(recid);
          
          paineis_atualiza = true;
        }
      });
    }
    
    if (!componentes_atualiza){
      //const proj_id = new Object();
      let com = "SELECT comp_total.painel, comp_total.componente, " +
        "CASE WHEN componentes.item IS NULL THEN 'x' END estimado, " +
        "CASE WHEN componentes.item IS NULL THEN est.item ELSE componentes.item END item, " +
        "CASE WHEN componentes.item IS NULL THEN est.estimado ELSE componentes.tipo END tipo, " +
        "componentes.id_fiacao, CASE WHEN est.componente IS NOT NULL THEN 'x' END existe " +
        "FROM (SELECT painel, componente FROM estima_comp_esq " +
        "UNION SELECT painel, id FROM componentes ORDER BY painel, componente) comp_total " +
        "LEFT JOIN estima_comp_esq est ON est.painel = comp_total.painel AND  " +
        "est.componente = comp_total.componente " +
        "LEFT JOIN componentes ON comp_total.painel = componentes.painel AND  " +
        "comp_total.componente = componentes.id " +
        "ORDER BY comp_total.painel, comp_total.componente;";
      
      webui_fn('Sqlite_exec', com).then((response) => {
        if (response) {
          //console.log(response);
          const obj = JSON.parse(response);
          let recid = tabela_componentes.last.sel_recid;
          tabela_componentes.mergeChanges();
          tabela_componentes.clear(); //limpa a tabela
          let num = 1;
          for (let linha of obj.data) {
            //let fiacao = linha.fiacao != "";
            tabela_componentes.add({
              recid: num,
              painel: linha.painel,
              componente: linha.componente,
              tipo: linha.tipo,
              item: linha.item,
              id_fiacao: linha.id_fiacao,
              existe: linha.existe,
              estimado: linha.estimado
            });
            num++;
          }
          tabela_componentes.refresh();
          tabela_componentes.scrollIntoView(recid);
          
          componentes_atualiza = true;
        }
      });
    }
  }
};

//atualiza();

})()