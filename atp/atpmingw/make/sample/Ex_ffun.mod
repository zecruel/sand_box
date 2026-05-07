MODEL Ex_ffun
  -- Sample MODELS for an external function
INPUT
  Idmy -- dummy
VAR
  a[1..2]

FUNCTION f1 FOREIGN SAMPLE_FUNCTION {ixarg:2}

INIT
  a[1..2]:=0
ENDINIT
EXEC
  write("****** In MODELS 'SAMPLE_FUNCTION' at t=",t)
  a[1..2]:=f1(t+a[1],-a[1])
  write(t," : ",a[1])
ENDEXEC
ENDMODEL
