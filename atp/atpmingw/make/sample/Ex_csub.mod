MODEL Ex_csub
  -- Sample MODELS for an external subroutine
INPUT
  Idmy -- dummy -- dummy for AtpDraw
VAR a[1..3]

MODEL mymodel FOREIGN SAMPLE_C_MODEL {ixdata:1, ixin:3, ixout:3, ixvar:3}

EXEC -- using the sample C subroutine
  USE mymodel AS mymodel
    DATA xdata[1]:=3
    INPUT xin[1..3]:=[t,t+1,t+2]
    HISTORY histdef(xvar[1..3]):=0
    OUTPUT a[1..3]:=xout[1..3]
  ENDUSE
ENDEXEC
ENDMODEL
