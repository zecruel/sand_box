let pstyle = 'border: 1px solid #efefef; padding: 5px; color: black;'
let layout = new w2layout({
    box: '#layout',
    name: 'layout',
    panels: [
        { type: 'top', size: 50, style: pstyle, html: 'top' },
        { type: 'left', size: 200, style: pstyle, html: 'left' },
        { type: 'main', html: '<div id="componentes_terminais"></div>' }
    ]
});

layout.html('top', '<div style="padding: 10px">Some HTML</div>');

let sidebar = new w2sidebar({
  name: 'sidebar',
  flatButton: true,
  nodes: [
      { id: 'general', text: 'General', group: true, expanded: true, groupShowHide: false,
          nodes: [
              { id: 'grid1', text: 'Grid 1', icon: 'fa-solid fa-microchip', selected: true },
              { id: 'grid2', text: 'Grid 2', icon: 'fa fa-list-alt' },
              { id: 'html', text: 'Some HTML', icon: "fa-solid fa-keyboard" }
          ],
          onCollapse(event) {
              event.preventDefault()
          }
      }
  ],
  onFlat(event) {
      layout.sizeTo('left', (event.detail.goFlat ? 35 : 200), true)
  },
  onClick(event) {
      switch (event.target) {
          case 'grid1':
              //layout.html('main', grid1)
              break
          case 'grid2':
              //layout.html('main', grid2)
              break
          case 'html':
              //layout.html('main', '<div style="padding: 10px">Some HTML</div>')
              //query(layout.el('main'))
               //   .css('border-left', '1px solid #efefef')
              break
      }
  }
});

layout.html('left', sidebar);


const comp_term_dados = [];
let pausa = false;
setInterval(atualiza, 2000);
let changes = -1;
let db_changes = 0;
let comp_term_init = false;

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
        }
      });
    }
    
    if (col == 3) { // modificando a parte
      webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value +
        "' WHERE unico = " + unico + ";").then((response) => {
        if (response) {
          //const obj = JSON.parse(response);
          //console.log(obj);
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
              const obj = JSON.parse(response);
              console.log(obj);
            }
          });
        }
        
        if (value.x == 3) { // modificando a parte
          webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value.oldValue +
            "' WHERE unico = " + unico + ";").then((response) => {
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
            }
          });
        }
        
        if (value.x == 3) { // modificando a parte
          webui_fn('Sqlite_exec', "UPDATE componentes_esq SET id = '" + value.newValue +
            "' WHERE unico = " + unico + ";").then((response) => {
            if (response) {
              const obj = JSON.parse(response);
              console.log(obj);
              
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
  //document.getElementById("demo").innerHTML = MyInput.value;
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
  }
};

atualiza();

