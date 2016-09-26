import Tkinter as tk
import tkFileDialog
import dxf
import window
import desenha
import os
import util



class teste:
	drawing = None
	selecao = util.selecao()
	view = window.viewer(args=(1,2,3),
	kwargs = {
	'largura': 800,
	'altura': 600,
	'titulo': 'Visualisador DXF',
	'selecao': selecao
	})
	with view.pronto:
		view.pronto.wait(5)

	def abrir(self):
		arquivo = tkFileDialog.askopenfilename(title='Abrir', filetypes=[("DXF","*.dxf")])
		if arquivo:
			dir = os.path.dirname(os.path.abspath(arquivo))
			os.chdir(dir)
			
			self.selecao.lista_selecao.conteudo = []
			
			#le o arquivo em uma estrutura DXF
			self.drawing = dxf.dxf_le(arquivo)
			
			#imprime a estrutura DXF indentada
			#print self.drawing.imprime()
			
			ents = dxf.dxf_item(self.drawing, 'SECTION', 'ENTITIES')
			
			#busca os valores de x dentro das Entidades, com recursividade
			lista_x = dxf.dxf_proc_fixo(ents, 'x', 1)
			#print lista_x
			
			#listas das outras coordenadas
			lista_y = dxf.dxf_proc_fixo(ents, 'y', 1)
			lista_z = dxf.dxf_proc_fixo(ents, 'z', 1)
			
			#encontra os limites dos objetos
			maior = max(lista_x)
			menor = min(lista_x)
			lim_x = [menor, maior]
			
			maior = max(lista_y)
			menor = min(lista_y)
			lim_y = [menor, maior]
			
			#maior = max(lista_z)
			#menor = min(lista_z)
			#lim_z = [menor, maior]
			
			zoom_x = 800/(lim_x[1] - lim_x[0])
			zoom_y = 600/(lim_y[1] - lim_y[0])
			
			self.view.zoom = min([zoom_x, zoom_y])
			self.view.offset = [i * self.view.zoom for i in [lim_x[0], lim_y[0]]]
			
			render = desenha.dxf_render(self.drawing, self.selecao)
			render.tela = self.view
			self.view.redesenha = render.desenha_ents
			self.view.des_selec = render.desenha_selec
			render.desenha_ents()

	def salvar(self):
		arquivo = tkFileDialog.asksaveasfilename(title='Salvar')
		if arquivo:
			with open(arquivo, 'w') as f:
				dxf.dxf_escreve(f, self.drawing, 1)
			


testeobj = teste()



raiz = tk.Tk()
b_abrir = tk.Button(raiz, text="Abrir", command=testeobj.abrir)
b_abrir.pack(side=tk.LEFT)
b_salvar = tk.Button(raiz, text="Salvar", command=testeobj.salvar)
b_salvar.pack(side=tk.LEFT)
raiz.mainloop()