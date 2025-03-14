# -*- coding: utf-8 -*-

import os
from tkinterdnd2 import *
from tkinter import *
from tkinter import ttk
import re


def autoscroll(sbar, first, last):
    """Hide and show scrollbar as needed."""
    first, last = float(first), float(last)
    if first <= 0 and last >= 1:
        sbar.grid_remove()
    else:
        sbar.grid()
    sbar.set(first, last)


root = TkinterDnD.Tk()
root.withdraw()
root.title('Trago Amor 2.0')

estado = 'ficha'

#criamos barras horizontais
vsb = ttk.Scrollbar(orient="vertical")
hsb = ttk.Scrollbar(orient="horizontal")

tree = ttk.Treeview(root, columns=("ficha", "fullpath", "aprov"),
    displaycolumns="aprov", yscrollcommand=lambda f, l: autoscroll(vsb, f, l),
    xscrollcommand=lambda f, l:autoscroll(hsb, f, l))
 
# associamos as barras de scroll às vistas x e y do objeto Treeview
vsb['command'] = tree.yview
hsb['command'] = tree.xview

# define os cabeçalhos das diferentes colunas
tree.heading("#0", text="Ficha", anchor='w')
tree.heading("aprov", text="Ap", anchor='w')
tree.column("aprov", width=10)

tree.tag_configure('top', background='#DFDFDF', font=(None, 11,'bold'))

# Arrange the tree and its scrollbars in the toplevel
tree.grid(column=0, row=0, sticky='nswe')
vsb.grid(column=1, row=0, sticky='ns')
hsb.grid(column=0, row=1, sticky='ew')
root.grid_columnconfigure(0, weight=1)
root.grid_rowconfigure(0, weight=1)

def temporal():
    sel = tree.selection()
    if len(sel) > 0:
        ficha_el = sel[0]
        arq_el = ''
        prox = ''
        if tree.parent(ficha_el) == '': #processa a ficha
            child = tree.get_children(ficha_el)
            if len(child) > 0:
                prox = child[0]
        else:
            ficha_el = tree.parent(ficha_el)
            arq_el = sel[0]
            prox = tree.next(arq_el)
        if prox == '':
            prox = tree.next(ficha_el)
        
        #processa ficha e arquivo corrente
        if arq_el != '':
            print( 'Arquivo: ' +  tree.item(arq_el)['text'] )
            estado = 'proximo'
        elif ficha_el != '':
            print( 'Ficha: ' +  tree.item(ficha_el)['text'] )
            estado = 'proximo'

            
        if estado == 'proximo':
            tree.selection_set(prox)
        tree.after (1000, temporal)

def simula():
    estado = 'ficha'
    tree.after (1000, temporal)
    
def apaga():
    for item in tree.selection():
        tree.delete(item)

def muda_aprov():
    for item in tree.selection():
        temp = tree.item(item, 'values')
        if temp[2] == 'A':
            tree.item(item, values=(temp[0], temp[1], 'R'))
        if temp[2] == 'R':
            tree.item(item, values=(temp[0], temp[1], 'A'))

def renom_ok():
    sel = tree.selection()
    novo = nome.get()
    if len(sel) > 0 and novo != '':
        ficha_el = sel[0]
        tree.item(ficha_el, text=novo)
    renom_pop.destroy()

def renom_cancel():
    renom_pop.destroy()

def renomeia():
    global renom_pop
    global nome
    sel = tree.selection()
    if len(sel) > 0:
        ficha_el = sel[0]
        renom_pop = Toplevel(root)
        renom_pop.transient(root)
        renom_pop.grab_set()
        renom_pop.title('Novo nome')
        nome = StringVar()
        nome.set(tree.item(ficha_el)['text'] )
        en_nome = Entry(renom_pop, textvariable=nome)
        en_nome.pack()
        b_ok = Button(renom_pop, text='OK', command=renom_ok)
        b_ok.pack(side = LEFT)
        b_cancel = Button(renom_pop, text='Cancel', command=renom_cancel)
        b_cancel.pack(side = RIGHT)
    

buttonbox = Frame(root)
buttonbox.grid(row=2, column=0, columnspan=2, pady=5)

Button(buttonbox, text='Simula', command=simula).pack()
Button(buttonbox, text='Remove', command=apaga).pack()
Button(buttonbox, text='Aprov', command=muda_aprov).pack()
Button(buttonbox, text='Renomeia', command=renomeia).pack()
Button(buttonbox, text='Sai', command=root.quit).pack()

def proc_ficha_tree(item, busca):
    for children in tree.get_children(item):
        if tree.item(children)['text'] == busca:
            return children
    return None



# Drop callbacks can be shared between the Listbox and Text;
# according to the man page these callbacks must return an action type,
# however they also seem to work without

def drop_enter(event):
    event.widget.focus_force()
    return event.action

def drop_position(event):
    return event.action

def drop_leave(event):
    return event.action

def drop(event):
    if event.data:
       if event.widget == tree:
            files = tree.tk.splitlist(event.data)
            for f in files:
                if os.path.exists(f):
                    base = os.path.basename(f)
                    fname = os.path.splitext(base)[0]
                    pedacos = fname.lower().split('_')
                    ficha = pedacos[0]
                    if len(pedacos) > 1:
                        orgao = re.search('^(\w{2})(\w{2}\d?$)', pedacos[0])
                        if orgao:
                            ficha = orgao.group(1) + '/' + orgao.group(2) + '-' + pedacos[1]
                    fichaitem = proc_ficha_tree ('', ficha)
                    if fichaitem == None:
                        fichaitem = tree.insert('', END, text=ficha, open=True, values=[ficha,  "", "A"], tags=('top',))
                    if proc_ficha_tree (fichaitem, base) == None:
                        tree.insert(fichaitem, END, text=base, values=[ficha,  f, ""])
        #else:
            #print('Error: reported event.widget not known')
    return event.action

# now make the Listbox and Text drop targets
tree.drop_target_register(DND_FILES, DND_TEXT)


tree.dnd_bind('<<DropEnter>>', drop_enter)
tree.dnd_bind('<<DropPosition>>', drop_position)
tree.dnd_bind('<<DropLeave>>', drop_leave)
tree.dnd_bind('<<Drop>>', drop)

# define drag callbacks

def drag_init_tree(event):
    return ((ASK, COPY), (DND_FILES, DND_TEXT), data)


tree.dnd_bind('<<DragInitCmd>>', drag_init_tree)
root.update_idletasks()
root.deiconify()
root.mainloop()

