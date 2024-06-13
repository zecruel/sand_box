
const comp_term_dados = [];
let pausa = false;
setInterval(atualiza, 2000);
let changes = -1;
let db_changes = 0;

const cor_claro = [
  '#cccccc',
  '#ffccff',
  '#ffcccc',
  '#ffffcc',
  '#ccffcc',
  '#ccffff',
  '#ccccff',
];

const cor_escuro = [
  '#909090',
  '#ff90ff',
  '#ff9090',
  '#ffff90',
  '#90ff90',
  '#90ffff',
  '#9090ff',
];
  

const data = [
    ['Mazda', 2001, 2000],
    ['Pegeout', 2010, 5000],
    ['Honda Fit', 2009, 3000],
    ['Honda CRV', 2010, 6000],
];

const beforeChange = function(instance, cell, col, lin, value) {
    const cellName = jspreadsheet.getColumnNameFromId([col,lin]);
    //console.log('The cell ' + cellName + ' will be changed =' + value + '-' + col + '-' + lin);
    let y = Number(lin) + 1;
    document.getElementById("demo").innerHTML = 'Meta H'+ y +' = '+ comp_term.getMeta('H' + y).id;
};

const undo = function(instance, obj) {
  if (obj){
    if (obj.action == "setValue"){
      obj.records.forEach(function(value, index, array) {
        console.log(value.x, value.y, value.oldValue);
      });
    }
  }
};

const redo = function(instance, obj) {
  if (obj){
    if (obj.action == "setValue"){
      obj.records.forEach(function(value, index, array) {
        console.log(value.x, value.y, value.newValue);
      });
    }
  }
};


const loaded = function(instance) {
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
  
  //agrupa as celulas repetidas
  if (comp_term_dados[0]) {
    painel_ant = comp_term_dados[0][0];
    comp_ant = comp_term_dados[0][1];
    unico_ant = comp_term.getMeta('H1').id;
    parte_ant = comp_term_dados[0][3];
    modulo_ant = comp_term_dados[0][2];
  }
  let y = 1;
  let claro = cor_claro;
  let cor_uni = claro[unicos % 7];
  
  for (let linha of comp_term_dados){
    
    
    
    
    
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
    
    if ((paineis % 2) == 0){
      claro = cor_claro;
    }
    else {
      claro = cor_escuro;
    }
    
    y++;
  }
  
  if ((y - ini_painel) > 1) {
    comp_term.setMerge("A"+ini_painel, 1, y - ini_painel);
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
    onbeforechange: beforeChange,
    onundo: undo,
    onredo: redo,
    onload: loaded,
});

function teste_ant() {
  data[1][1] = "teste";
  table.setData();
  //table.setMerge("A1", 1, 2);

  console.log(JSON.stringify(table.getMeta()));
  
  const cel = table.getCell("B2");
  console.log(cel.style);
  cel.classList.add('readonly');
  cel.style.backgroundColor = "#ffcccc";
  cel.style.color = "#000";
  
  console.log(JSON.stringify(table.getStyle('B3')));
};

function meta() {
  table.setMeta('B2', 'id', '123');
  //table.setStyle('B2', "background-color", "red");
  
};

function del_meta() {
  const all_meta = table.getMeta();
  for (let cel in all_meta) {
    all_meta[cel] = {};
  }
  table.setMeta(all_meta);
  
  const cel = table.getCell("B2");
  cel.classList.remove('readonly');
};

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
          comp_term.setData();
          
        }
      });
      
      
      
      
      
    }
  }
};

atualiza();