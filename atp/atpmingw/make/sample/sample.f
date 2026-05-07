      FUNCTION fmodels(arg)
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      DIMENSION arg(2)
      arg(1)=arg(1) +arg(2)
      arg(2)=10*arg(1)
      fmodels=2
      RETURN
      END
