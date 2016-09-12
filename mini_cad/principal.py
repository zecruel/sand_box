import Tkinter as tk
import tkFileDialog
import dxf
import window
import desenha
import os

class teste:
	drawing = None

	def abrir(self):
		arquivo = tkFileDialog.askopenfilename(title='Abrir', filetypes=[("DXF","*.dxf")])
		if arquivo:
			dir = os.path.dirname(os.path.abspath(arquivo))
			os.chdir(dir)
			
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
			
			jan.zoom = min([zoom_x, zoom_y])
			jan.offset = [i * jan.zoom for i in [lim_x[0], lim_y[0]]]
			
			render = desenha.dxf_render(self.drawing)
			render.tela = jan
			jan.redesenha = render.desenha_ents
			render.desenha_ents()

	def salvar(self):
		arquivo = tkFileDialog.asksaveasfilename(title='Salvar')
		if arquivo:
			with open(arquivo, 'w') as f:
				dxf.dxf_escreve(f, self.drawing, 1)
			
jan = window.viewer(args=(1,2,3),
	kwargs = {
	'largura': 800,
	'altura': 600,
	'titulo': 'Visualisador DXF'
	})

testeobj = teste()

with jan.pronto:
	jan.pronto.wait(5)

raiz = tk.Tk()
b_abrir = tk.Button(raiz, text="Abrir", command=testeobj.abrir)
b_abrir.pack(side=tk.LEFT)
b_salvar = tk.Button(raiz, text="Salvar", command=testeobj.salvar)
b_salvar.pack(side=tk.LEFT)
raiz.mainloop()