C TACS HYBRID
  GALT    +G
90SCHLT
99STROM   = 1.E6 * ABS(SCHLT)
99GSTAT   = STROM * STROM / (170000. + 890. * STROM)
99GKOR    = 1.003 * GSTAT
99SSGNL   = ISTEP - 9.
99GG    58+GSTAT                                         1.0  .29E-6SSGNL GKOR
C
AALSGNL   = GG .LE. 1.E-6 .OR. GALT .LE. 1.E-6
BB      IF ( GG .LE. 1.E-6 .OR. GALT .LE. 1.E-6 ) THEN
BB        LSGNL = 1
BB      ELSE
BB        LSGNL = 0
BB      ENDIF
88G       = 1.E-6 * LSGNL + GG * NOT(LSGNL)
98RB      = INVRS(G)
33RB    G     GSTAT STROM SCHLT SSGNL LSGNL GG    GALT  GKOR
77RB       1.
77GG       1.
77G        1.
77GALT     1.
C END OF TACS