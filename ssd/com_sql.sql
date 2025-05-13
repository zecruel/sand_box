SELECT DISTINCT ln,
                d_o
  FROM geral
 ORDER BY ln,
          d_o;

CREATE TABLE log_nodes (
    ln    TEXT,
    d_o   TEXT,
    tipo  TEXT,
    descr TEXT,
    obs   TEXT
);

INSERT INTO log_nodes (
                          ln,
                          d_o
                      )
                      SELECT DISTINCT ln,
                                      "Mod"
                        FROM geral
                      UNION
                      SELECT DISTINCT ln,
                                      "Beh"
                        FROM geral
                       ORDER BY ln;

WITH cte_ln AS (
    SELECT DISTINCT ln,
                    d_o
      FROM geral
     ORDER BY ln,
              d_o
)
SELECT DISTINCT ln,
                "Beh" AS d_o,
                "ENS" AS tipo,
                'Status do modo (N/T/B)' AS descr
  FROM cte_ln
UNION ALL
SELECT ln,
       d_o,
       CASE d_o WHEN 'Str' THEN 'ACD' WHEN 'Loc' THEN 'SPS' WHEN 'LocKey' THEN 'SPS' WHEN 'Pos' THEN 'DPC' WHEN 'OpCnt' THEN 'INS' WHEN 'Auto' THEN 'SPC' WHEN 'BlkOpn' THEN 'SPC' WHEN 'BlkCls' THEN 'SPC' WHEN 'EnaOpn' THEN 'SPS' WHEN 'EnaCls' THEN 'SPS' WHEN 'Op' THEN 'ACT' END AS tipo,
       CASE d_o WHEN 'Str' THEN 'Partida' WHEN 'Loc' THEN 'Chave local/remoto' WHEN 'LocKey' THEN 'Chave local/remoto' WHEN 'Pos' THEN 'Aberto/Fechado' WHEN 'OpCnt' THEN 'Contador de operacoes' WHEN 'Auto' THEN 'Automatico/Manual' WHEN 'BlkOpn' THEN 'Bloqueio abertura' WHEN 'BlkCls' THEN 'Bloqueio fechamento' WHEN 'EnaOpn' THEN 'Habilita abertura' WHEN 'EnaCls' THEN 'Habilita fechamento' WHEN 'Op' THEN 'Trip' ELSE (
                SELECT descr
                  FROM geral
                 WHERE geral.ln = cte_ln.ln AND 
                       geral.d_o = cte_ln.d_o
                 LIMIT 1
            )
       END AS descr
  FROM cte_ln
 ORDER BY ln;

INSERT INTO log_nodes (
                          ln,
                          d_o,
                          tipo,
                          descr
                      )
                  WITH cte_ln AS (
                          SELECT DISTINCT ln,
                                          d_o
                            FROM geral
                           ORDER BY ln,
                                    d_o
                      )
                      SELECT DISTINCT ln,
                                      "Beh" AS d_o,
                                      "ENS" AS tipo,
                                      'Status do modo (N/T/B)' AS descr
                        FROM cte_ln
                      UNION ALL
                      SELECT ln,
                             d_o,
                             CASE d_o WHEN 'Str' THEN 'ACD' WHEN 'Loc' THEN 'SPS' WHEN 'LocKey' THEN 'SPS' WHEN 'Pos' THEN 'DPC' WHEN 'OpCnt' THEN 'INS' WHEN 'Auto' THEN 'SPC' WHEN 'BlkOpn' THEN 'SPC' WHEN 'BlkCls' THEN 'SPC' WHEN 'EnaOpn' THEN 'SPS' WHEN 'EnaCls' THEN 'SPS' WHEN 'Op' THEN 'ACT' END AS tipo,
                             CASE d_o WHEN 'Str' THEN 'Partida' WHEN 'Loc' THEN 'Chave local/remoto' WHEN 'LocKey' THEN 'Chave local/remoto' WHEN 'Pos' THEN 'Aberto/Fechado' WHEN 'OpCnt' THEN 'Contador de operacoes' WHEN 'Auto' THEN 'Automatico/Manual' WHEN 'BlkOpn' THEN 'Bloqueio abertura' WHEN 'BlkCls' THEN 'Bloqueio fechamento' WHEN 'EnaOpn' THEN 'Habilita abertura' WHEN 'EnaCls' THEN 'Habilita fechamento' WHEN 'Op' THEN 'Trip' ELSE (
                                      SELECT descr
                                        FROM geral
                                       WHERE geral.ln = cte_ln.ln AND 
                                             geral.d_o = cte_ln.d_o
                                       LIMIT 1
                                  )
                             END AS descr
                        FROM cte_ln
                       ORDER BY ln;

UPDATE log_nodes
   SET tipo = "ENS_Beh"
 WHERE d_o = "Beh";

UPDATE log_nodes
   SET ln = classe || '_CEMIG';
 

UPDATE do_types
   SET t_bas = "Timestamp"
 WHERE d_a = "t";

WITH cte_ln AS (
    SELECT DISTINCT tipo
      FROM log_nodes
     ORDER BY tipo
)
SELECT cte_ln.tipo,
       (
           SELECT log_nodes.ln
             FROM log_nodes
            WHERE cte_ln.tipo = log_nodes.tipo
            LIMIT 1
       )
       AS ln,
       (
           SELECT log_nodes.d_o
             FROM log_nodes
            WHERE cte_ln.tipo = log_nodes.tipo
            LIMIT 1
       )
       AS d_o,
       (
           SELECT log_nodes.descr
             FROM log_nodes
            WHERE cte_ln.tipo = log_nodes.tipo
            LIMIT 1
       )
       AS descr
  FROM cte_ln;

CREATE TABLE do_types (
    d_o    TEXT,
    classe TEXT,
    d_a    TEXT,
    t_bas  TEXT,
    fc     TEXT,
    trg    TEXT,
    tipo   TEXT,
    descr  TEXT,
    obs    TEXT
);

CREATE TABLE structs (
    d_a   TEXT,
    bda   TEXT,
    t_bas TEXT,
    tipo  TEXT,
    descr TEXT,
    obs   TEXT
);

CREATE TABLE enums (
    enum_id TEXT,
    ord     INTEGER,
    valor   TEXT,
    descr   TEXT,
    obs     TEXT
);

INSERT INTO do_types (
                         d_o,
                         classe,
                         d_a,
                         t_bas,
                         fc,
                         trg
                     )
                 WITH cte_ln AS (
                         SELECT DISTINCT tipo
                           FROM log_nodes
                          ORDER BY tipo
                     )
                     SELECT cte_ln.tipo,
                            substr(cte_ln.tipo, 1, 3),
                            NULL AS d_a,
                            NULL AS t_bas,
                            NULL AS fc,
                            NULL AS trg
                       FROM cte_ln
                     UNION ALL
                     SELECT cte_ln.tipo,
                            substr(cte_ln.tipo, 1, 3),
                            "q",
                            "Quality",
                            "ST",
                            "qchg"
                       FROM cte_ln
                     UNION ALL
                     SELECT cte_ln.tipo,
                            substr(cte_ln.tipo, 1, 3),
                            "t",
                            "TimeStamp",
                            "ST",
                            NULL
                       FROM cte_ln
                      ORDER BY tipo;

WITH cte_ln AS (
    SELECT DISTINCT ld, pre, ln,inst
      FROM geral
     ORDER BY ld,ln,pre,inst
) select replace(replace(replace(replace(replace(replace(replace(replace(upper(
case when instr(ld, 'Equip') then replace(ld, 'Equip - ', 'E_')
when instr(ld, 'Ctrl') then replace(ld, 'Ctrl - ', 'C_')
when instr(ld, 'Prot') then replace(ld, 'Prot - ', 'P_')
else ld end
)
, 'MONO', '1F')
, 'TRIF', '3F')
, 'TRI', '3F')
, 'MANOBRA', 'VAO')
, 'TRANSF', 'VT')
, 'TRAFO', 'TR')
, 'DISJ', 'DJ')
, ' ', '_')
 as ld,pre,ln,inst from cte_ln