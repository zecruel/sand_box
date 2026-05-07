      SUBROUTINE HOPCOD ( N4, CARD, TEXVEZ, TRZ, TXZ, CZ )
      IMPLICIT REAL*8 (A-H, O-Z),  INTEGER*4 (I-N)
      CHARACTER*(*)  CARD
      CHARACTER*6  TEXVEZ(*)
      DIMENSION  TRZ(*)
      DIMENSION  TXZ(*)
      DIMENSION   CZ(*)
      INCLUDE  'blkcom.ins'
      INCLUDE  'deksal.ins'
      EQUIVALENCE  ( MONCAR(42), MAXBUS )
      COMMON  / C0B051 /   C     (    1 )
      COMMON  / C0B068 /   TEXVEC(    1 )
      CHARACTER*6  TEXVEC
      COMMON  / C0B052 /   TR    (    1 )
      COMMON  / C0B053 /   TX    (    1 )
      IF ( IPRSUP .GE. 1 )
     1 write (LUNIT6, *) ' Top of HOPCOD.  MAXBUS, IT =',  maxbus, it
      TEXVEC(MAXBUS+1) = 'GEN-A '
      TEXVEC(MAXBUS+2) = '1-A   '
      TEXVEC(MAXBUS+3) = 'GEN-B '
      TEXVEC(MAXBUS+4) = '1-B   '
      TEXVEC(MAXBUS+5) = 'GEN-C '
      TEXVEC(MAXBUS+6) = '1-C   '
      TR(IT)   = 34.372D0
      TR(IT+1) = 35.735D0
      TR(IT+2) = 38.002D0
      TR(IT+3) = 35.735D0
      TR(IT+4) = 37.455D0
      TR(IT+5) = 38.002D0
      TX(IT)   = .02428067811809955D0
      TX(IT+1) = .008723282430866784D0
      TX(IT+2) = .02396820391316247D0
      TX(IT+3) = .008723282430866784D0
      TX(IT+4) = .008048995988634121D0
      TX(IT+5) = .02396820391316247D0
      C(IT)   = .15781D-6
      C(IT+1) = -.0315D-6
      C(IT+2) = .16587D-6
      C(IT+3) = -.0315D-6
      C(IT+4) = -.0219D-6
      C(IT+5) = .16587D-6
      IF ( N4 .EQ. 1                .OR.  CARD(1:1) .EQ. ' '  .OR.
     1     TEXVEZ(1) .EQ. 'UNUSED'  .OR.  TRZ(1) .GT. 0.0     .OR.
     2     TXZ(1)    .GT. 0.0       .OR.  CZ(1)  .GT. 0.0 )
     3 RETURN
      RETURN
      END
