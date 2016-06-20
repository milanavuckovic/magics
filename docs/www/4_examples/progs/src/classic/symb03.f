C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM SYMB03

*     SYMBOL PLOTTING EXAMPLE
*
*     PLOT THE MAGICS MARKER TABLE USING THE SYMBOL
*     PLOTTING SEATURES OF MAGICS
*
      CALL POPEN 
      CALL PSETC ('PS_DEVICE','ps_a4')
      CALL PSETC ('PS_FILE_NAME','symb03.ps')
      CALL PSETC('PAGE_ID_LINE_USER_TEXT','Tests/Symb03')
      CALL PSETR('SUPER_PAGE_X_LENGTH',21.0)
      CALL PSETR('SUPER_PAGE_Y_LENGTH',29.7)
      CALL PSETR('PAGE_X_LENGTH',21.0)
      CALL PSETR('PAGE_Y_LENGTH',29.7)
      CALL PSETR('SUBPAGE_X_LENGTH',21.0)
      CALL PSETR('SUBPAGE_Y_LENGTH',27.7)
      CALL PSETR('SUBPAGE_X_POSITION',0.0)
      CALL PSETR('SUBPAGE_Y_POSITION',0.0)
      CALL PSETC('SYMBOL_POSITION_MODE','PAPER')
      CALL PSETC('SUBPAGE_MAP_PROJECTION','NONE')
      CALL PSETC('SYMBOL_QUALITY','LOW')
      CALL SYMB03_ONE
      CALL SYMB03_TWO
*
*     PLOT THE TEXT
*     -------------
      CALL PSETR('TEXT_BOX_X_POSITION',2.0)
      CALL PSETR('TEXT_BOX_Y_POSITION',1.5)
      CALL PSETR('TEXT_BOX_X_LENGTH',17.0)
      CALL PSETR('TEXT_BOX_Y_LENGTH',1.0)
      CALL PSETC('TEXT_JUSTIFICATION','CENTRE')
      CALL PSETC('TEXT_LINE_1','MAGICS Marker Table')
      CALL PSETI('TEXT_LINE_COUNT',1)
      CALL PSETC('TEXT_MODE','POSITIONAL')
      CALL PSETC('TEXT_COLOUR','BLUE')
      CALL PTEXT
      CALL PCLOSE
      STOP
      END
      SUBROUTINE SYMB03_ONE
*
*     PLOT THE MARKER NUMBERS
*     -----------------------
      DIMENSION X(29),Y(29),RAR(29)
      DATA X/15*13.0,14*6.0/                                      
      DATA Y/2.0,3.25,4.5,5.75,7.,8.25,9.5,10.75,12.,13.25,14.5,
     x 15.75,17.,18.25,19.5,2.0,3.25,4.5,5.75,7.,8.25,9.5,10.75,12.,
     x 13.25,14.5,15.75,17.,18.25/
      DATA RAR/28.,27.,26.,25.,24.,23.,22.,21.,
     x20.,19.,18.,17.,16.,15.,14.,13.,12.,11.,10.,9.,
     x 8.,7.,6.,5.,4.,3.,2.,1.,0./
       DO 100 J=1,29
          Y(j)=Y(j)+4.0
100    CONTINUE
      CALL PSETC('SYMBOL_TABLE_MODE','OFF')
      CALL PSETC('SYMBOL_TYPE','NUMBER')
      CALL PSET1R('SYMBOL_INPUT_NUMBER_LIST',RAR,29)
      CALL PSET1R('SYMBOL_INPUT_X_POSITION',X,29)
      CALL PSET1R('SYMBOL_INPUT_Y_POSITION',Y,29)
      CALL PSETC('SYMBOL_COLOUR','MAGENTA')
      CALL PSETR('SYMBOL_HEIGHT',0.5)
      CALL PSYMB
      RETURN
      END
      SUBROUTINE SYMB03_TWO
*
*     PLOT THE MARKER SYMBOLS
*     -----------------------
      DIMENSION XX(29),YY(29)
      DIMENSION IMAR(29)
      DATA IMAR/28,27,26,25,24,23,22,21,
     X          20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,
     x          2,1,0/
      DATA XX/15*15.0,14*8.0/
      DATA YY/2.0,3.25,4.5,5.75,7.,8.25,9.5,10.75,12.,13.25,14.5,
     x 15.75,17.,18.25,19.5,2.0,3.25,4.5,5.75,7.,8.25,9.5,10.75,12.,
     X 13.25,14.5,15.75,17.,18.25/
       DO 100 J=1,29
          YY(j)=YY(j)+4.0
100    CONTINUE
      CALL PSETC('SYMBOL_TABLE_MODE','OFF')
      CALL PSETC('SYMBOL_TYPE','MARKER')
      CALL PSET1R('SYMBOL_INPUT_X_POSITION',XX,29)
      CALL PSET1R('SYMBOL_INPUT_Y_POSITION',YY,29)
      CALL PSET1I('SYMBOL_INPUT_MARKER_LIST',IMAR,29)
      CALL PSETC('SYMBOL_COLOUR','BLACK')
      CALL PSETR('SYMBOL_HEIGHT',0.4)
      CALL PSYMB
      RETURN
      END
