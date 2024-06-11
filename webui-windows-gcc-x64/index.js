const tabela_dados = [];
const comp_term_dados = [];
let pausa = false;
setInterval(atualiza, 1000);
  


const data = [
    ['Mazda', 2001, 2000],
    ['Pegeout', 2010, 5000],
    ['Honda Fit', 2009, 3000],
    ['Honda CRV', 2010, 6000],
];

const beforeChange = function(instance, cell, col, lin, value) {
    const cellName = jspreadsheet.getColumnNameFromId([col,lin]);
    console.log('The cell ' + cellName + ' will be changed =' + value + '-' + col + '-' + lin);
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

const table = jspreadsheet(document.getElementById('my-spreadsheet'), {
    data:data,
    columns:[
        { title:'Model', width:300 },
        { title:'Price', width:80 },
        { title:'Model', width:100 }
    ],
    meta:{
        A1: { myMeta: 'this is just a test', otherMetaInformation: 'other test' },
        A2: { info: 'test' }
    },
    onchange: beforeChange,
    onundo: undo,
    onredo: redo,
});

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
    onchange: beforeChange,
    onundo: undo,
    onredo: redo,
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
  //webui_fn('Sqlite', MyInput.value);
  webui_fn('Sqlite', MyInput.value).then((response) => {
      //document.getElementById("demo").innerHTML = response;
      //webui_fn('MyID_One', response);
      const obj = JSON.parse(response);
      
      tabela_dados.splice(0,tabela_dados.length); //limpa a tabela
      for (let linha of obj.data) {
        const novo = new Object();
        for (let coluna in linha) {
          novo[coluna] = linha[coluna];
        }
        tabela_dados.push(novo);
      }
      //tabela_dados = obj.data;
      
      document.getElementById("demo").innerHTML = obj.data[0].caminho;
  });
  
  //webui_fn('MyID_One', tabledata[0].name);
  //tabledata[0].name = MyInput.value;
  //tabledata[0].name = "teste";
  //webui_fn('MyID_One', tabledata[0].name);
};

function atualiza() {
  if(!pausa){
    webui_fn('Sqlite', "SELECT * FROM arquivos;").then((response) => {
      const obj = JSON.parse(response);
      
      tabela_dados.splice(0,tabela_dados.length); //limpa a tabela
      for (let linha of obj.data) {
        const novo = new Object();
        for (let coluna in linha) {
          novo[coluna] = linha[coluna];
        }
        tabela_dados.push(novo);
      }
    });
    
    webui_fn('Sqlite', "SELECT * FROM comp_term;").then((response) => {
      const obj = JSON.parse(response);
      
      comp_term_dados.splice(0,comp_term_dados.length); //limpa a tabela
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
      }
    });
    /* atualiza a exibição */
    comp_term.setData();
  }
};