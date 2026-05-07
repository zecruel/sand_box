      SUBROUTINE DIMDEF ( LSIZE, NCHAIN )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)                              
      DIMENSION  LSIZE(50)
      IF ( NCHAIN  .NE.   0 )   GO TO 2000
      LSIZE( 1)  =    6002
      LSIZE( 2)  =   10000
      LSIZE( 3)  =  192000
      LSIZE( 4)  =     900
      LSIZE( 5)  =  420000
      LSIZE( 6)  =    1200
      LSIZE( 7)  =   15000
      LSIZE( 8)  =  120000
      LSIZE( 9)  =    2250
      LSIZE(10)  =    3800
      LSIZE(11)  =     720
      LSIZE(12)  =    2000
      LSIZE(13)  =   72800
      LSIZE(14)  =     510
      LSIZE(15)  =   90000
      LSIZE(16)  =     800
      LSIZE(17)  =      90
      LSIZE(18)  =     254
      LSIZE(19)  =  120000
      LSIZE(20)  =  100000
      LSIZE(21)  =    3000
      LSIZE(22)  =   15000
      LSIZE(23)  =  192000
      LSIZE(24)  =     120
      LSIZE(25)  =   45000
      LSIZE(26)  =  260000
      LSIZE(27)  =     600
      LSIZE(28)  =  210000
      LSIZE(29)  =    1100
      LSIZE(30)  =      60
      LSIZE(31)  =     200
      LSIZE(32)  =     300
      N7 = 32 + 1
      LSIZE(N7)  =  11093436
      RETURN
 2000 IF ( NCHAIN  .NE.  20 )   GO TO 2100
      LSIZE( 1)  =      23
      LSIZE( 2)  =  192000
      LSIZE( 3)  =       1
      LSIZE( 4)  =    6002
      LSIZE( 5)  =       0
      LSIZE( 6)  =  840000
      LSIZE( 7)  =      29
      LSIZE( 8)  =    1100
      RETURN
 2100 IF ( NCHAIN  .NE.  21 )   GO TO 2300
      LSIZE( 1)  =      13
      LSIZE( 2)  =   72800
      RETURN
 2300 IF ( NCHAIN  .NE.  23 )   GO TO  900
      LSIZE( 1)  =      71
      LSIZE( 2)  =     742
      RETURN
  900 IF ( NCHAIN  .NE.   9 )   GO TO 2500
      LSIZE( 1)  =       1
      LSIZE( 2)  =    6002
      LSIZE( 3)  =      23
      LSIZE( 4)  =  192000
      LSIZE( 5)  =      52
      LSIZE( 6)  =    3600
      LSIZE( 7)  =       4
      LSIZE( 8)  =     900
      RETURN
 2500 IF ( NCHAIN  .NE.  25 )   GO TO 2600
      LSIZE( 1)  =       9
      LSIZE( 2)  =    2250
      LSIZE( 3)  =      71
      LSIZE( 4)  =     190
      LSIZE( 5)  =      73
      LSIZE( 6)  =     191
      RETURN
 2600 IF ( NCHAIN  .NE.  26 )   GO TO 2700
      LSIZE( 1)  =       9
      LSIZE( 2)  =    2250
      RETURN
 2700 IF ( NCHAIN  .NE.  27 )   GO TO 9900
      LSIZE( 1)  =       9
      LSIZE( 2)  =    2250
      RETURN
 9900 CONTINUE
      RETURN
      END
