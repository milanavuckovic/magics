C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM CONT01                    
C
C     THIS PROGRAM DEMONSTRATES MAGICS CONTOURING FACILITIES. 
C     THE METEOROLOGICAL DATA FIELD IS A STANDARD GLOBAL 500 hPa
C     MODEL OUTPUT FIELD ON A REGULAR 1.5 DEGREE GRID. 
C     CONTOURS AND COASTLINES ARE PROJECTED ONTO A 
C     POLAR STEREOGRAPHIC MAP.
C
C
C     OPEN MAGICS
C
      CALL POPEN
      CALL PSETC ('PS_DEVICE','ps_a4')
      CALL PSETC ('PS_FILE_NAME','cont01.ps')
      CALL PSETC ('PAGE_ID_LINE_USER_TEXT','TESTS/CONT01')
      CALL PSETR ('SUPER_PAGE_X_LENGTH',20.0)              
      CALL PSETR ('SUPER_PAGE_Y_LENGTH',13.0)              
      CALL PSETR ('PAGE_X_LENGTH',20.0)              
      CALL PSETR ('PAGE_Y_LENGTH',13.0)              
      CALL PSETC ('MAP_COASTLINE_COLOUR','GREEN')     
C
C     PASS THE DATA TO MAGICS
C
      CALL PSETC ('GRIB_INPUT_TYPE','FILE')
      CALL PSETC ('GRIB_INPUT_FILE_NAME',
     x 'data/oldts/500Z.grb')
      CALL PGRIB
C
C
C
C     GENERATE TEXT FOR TITLE
C
      CALL PSETC ('TEXT_LINE_1',
     1'Input Field: 500 hPa Geopotential Height on a 1.5 degree grid')
C
      CALL PSETC ('TEXT_LINE_2',
     1'Global Cylindrical area: Default values for all '//
     2 'Contour parameters')
C
      CALL PSETI ('TEXT_LINE_COUNT',2)
      CALL PSETR ('TEXT_REFERENCE_CHARACTER_HEIGHT',0.5)
      CALL PSETC ('TEXT_JUSTIFICATION','CENTRE')
C
C     PLOT THE CONTOURS AND TITLE
C
      CALL PTEXT
      CALL PCONT
      CALL PCOAST
      CALL PNEW('PAGE')
      CALL PSETC ('SUBPAGE_MAP_PROJECTION','POLAR_STEREOGRAPHIC')
      CALL PSETC ('SUBPAGE_MAP_AREA_DEFINITION','CENTRE')  
      CALL PSETR ('SUBPAGE_MAP_CENTRE_LATITUDE',50.0)
      CALL PSETR ('SUBPAGE_MAP_CENTRE_LONGITUDE',10.0)
      CALL PSETR ('SUBPAGE_MAP_VERTICAL_LONGITUDE',10.0)
      CALL PSETR ('SUBPAGE_MAP_SCALE',25.E6)         
      CALL PSETC ('TEXT_LINE_1',
     1'Input Field: 500 hPa Geopotential Height on a 1.5 degree grid')
C
      CALL PSETC ('TEXT_LINE_2',
     1'Polar Stereographic area: Default values for all '//
     2  'Contour parameters')
C
      CALL PSETI ('TEXT_LINE_COUNT',2)
      CALL PSETR ('TEXT_REFERENCE_CHARACTER_HEIGHT',0.5)
      CALL PSETC ('TEXT_JUSTIFICATION','CENTRE')
      CALL PTEXT
      CALL PCONT
      CALL PCOAST
C
      CALL PCLOSE
C
      STOP
      END
