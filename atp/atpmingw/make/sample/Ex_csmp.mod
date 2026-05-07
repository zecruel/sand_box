MODEL Ex_csmp
  -- Sample MODELS for an external function
INPUT
  Idmy -- dummy
VAR
  a[1..2]
  Fseed

FUNCTION seed FOREIGN C_SEED {ixarg: 1}
FUNCTION rand FOREIGN C_RANDOM {ixarg: 1}

INIT
  a[1..2]:=0
  Fseed:=0
ENDINIT
EXEC
  IF Fseed = 0 THEN
    a[1]:=seed(Idmy)
    write("Set a seed for random number")
    Fseed:=1
  ENDIF
  a[2]:=rand(Idmy)
  write("Random = ",a[2])
ENDEXEC
ENDMODEL
