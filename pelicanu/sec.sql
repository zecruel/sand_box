with cte_b as (select bbb.barra barra_id, d.componente, d.tipo,
d.modulo, t.terminal, b.nome_painel from (select bb.barra, count(*) from barra_consol bb Group by bb.barra having count(*) = 2) bbb
left join barra_consol b on b.barra = bbb.barra
left join descr_comp d on d.unico = b.componente
left join comp_term t on t.unico = b.componente and t.num = b.terminal
order by bbb.barra)
select * from cte_b;
select componente from cte_b where tipo = 'CONTATO_NA';
drop table if exists bornes_sec;
create table bornes_sec (barra TEXT, comp TEXT, tipo TEXT, mod TEXT, term TEXT, painel TEXT);
insert into bornes_sec (barra, comp, tipo, mod, term, painel)
select bbb.barra barra_id, d.componente, d.tipo,
d.modulo, t.terminal, b.nome_painel from (select bb.barra, count(*) from barra_consol bb Group by bb.barra having count(*) = 2) bbb
left join barra_consol b on b.barra = bbb.barra
left join descr_comp d on d.unico = b.componente
left join comp_term t on t.unico = b.componente and t.num = b.terminal
order by bbb.barra;

select a.comp, a.term, b.comp regua,
b.mod borne, a.tipo, a.painel from bornes_sec a
inner join bornes_sec b on a.barra = b.barra and b.tipo = 'BORNE_SEC'
where not a.tipo = 'BORNE_SEC';

SELECT r.unico, des.projeto, des.titulo,
CASE WHEN r.des_ref = c.desenho THEN 'NESTE'
ELSE c.desenho END desenho,
CASE WHEN r.des_ref = c.desenho AND
r.fl_ref = c.fl THEN 'NESTA FL.' ELSE c.fl END fl,

borne.regua, borne.borne

FROM (SELECT rf.unico, comp.painel, comp.componente,
comp.modulo, comp.arquivo, rf.elemento, rf.terminal,
comp.desenho des_ref, comp.fl fl_ref
FROM referencias_esq rf
LEFT JOIN descr_comp comp ON rf.unico = comp.unico
ORDER BY painel, componente, modulo, terminal) r
LEFT JOIN (SELECT DISTINCT dc.unico, dc.painel,
dc.componente, dc.modulo, dc.tipo, dc.desenho,
dc.fl, group_concat(t.terminal, '-') OVER
(PARTITION BY t.componente) terms
FROM descr_comp dc
INNER JOIN (SELECT componente, terminal 
FROM terminais_esq
ORDER BY componente, terminal) t
ON dc.unico = t.componente) c
ON r.painel = c.painel AND r.elemento = c.tipo
AND r.componente = c.componente
AND (r.modulo = c.modulo
OR (r.modulo IS NULL AND c.modulo IS NULL))
AND r.terminal LIKE '%' || c.terms || '%'

left join (select a.comp, a.term, b.comp regua,
b.mod borne, a.tipo, a.painel from bornes_sec a
inner join bornes_sec b on a.barra = b.barra and b.tipo = 'BORNE_SEC'
where not a.tipo = 'BORNE_SEC') borne 
ON r.painel = borne.painel AND r.elemento = borne.tipo
AND r.componente = borne.comp
AND r.terminal LIKE '%' || borne.term || '%'

LEFT JOIN desenhos des
ON c.desenho = des.ident AND c.fl = des.fl
WHERE r.arquivo = '/home/ezequiel/proj/bd3/22012vision4201_000004p.dxf';

select distinct b.id barra from engates_esq e
inner join barras_esq b on e.unico = b.componente
where engate = 'TERRA';

select bbb.barra, d.painel, d.componente, d.tipo,
d.modulo, t.terminal, ROW_NUMBER() OVER (PARTITION BY bbb.barra ORDER BY 
bbb.barra, d.painel, d.componente, d.modulo, t.terminal) as rn from 
(select distinct bb.id barra from engates_esq e
inner join barras_esq bb on e.unico = bb.componente
where engate = 'TERRA') bbb
left join barras_esq b on b.id = bbb.barra
inner join descr_comp d on d.unico = b.componente
left join comp_term t on t.unico = b.componente and t.num = b.terminal
order by bbb.barra, d.painel, d.componente, d.modulo, t.terminal;

select painel, componente, modulo, terminal,
ROW_NUMBER() OVER (partition by painel
order by painel, componente, modulo, terminal) as bt from (
select bbb.barra, d.painel, d.componente, d.tipo,
d.modulo, t.terminal, ROW_NUMBER() OVER (
PARTITION BY bbb.barra ORDER BY bbb.barra,
case when d.tipo = 'BORNE' then 1
when d.tipo = 'CT_CORR' then 2
when d.tipo = 'CT_TENSAO' then 2
when d.tipo = 'BORNE_SEC' then 7
else 6 end, d.painel, 
d.componente, d.modulo, t.terminal) as rn from 
(select distinct bb.id barra from engates_esq e
inner join barras_esq bb on e.unico = bb.componente
where engate = 'TERRA') bbb
left join barras_esq b on b.id = bbb.barra
inner join descr_comp d on d.unico = b.componente
left join comp_term t on t.unico = b.componente
and t.num = b.terminal
)
where rn = 1 and painel = 'P27'