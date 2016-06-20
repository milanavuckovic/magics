C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM MISSINGGRIB

C     This program demonstrates magics contouring facilities. 
C     The meteorological data field is a standard global 500 hpa
C     model output field on a regular 1.5 degree grid. 
C     Contours and coastlines are projected onto a 
C     polar stereographic map.


C     Open MAGICS and set the output device

      CALL POPEN
      CALL PARSE_COMMAND_LINE ('errors_missinggrib')



C     Set up the coastline attributes

      CALL PSETC ('MAP_COASTLINE_COLOUR', 'GREY')
      CALL PSETC ('MAP_GRID_COLOUR',      'GREY')     


C     Pass the data to MAGICS

      CALL PSETC ('GRIB_INPUT_TYPE',      'FILE')
      CALL PSETC ('GRIB_INPUT_FILE_NAME', 'data/no_file.grb')
      CALL PGRIB
      

C     Define the contour     

      CALL PSETC  ('CONTOUR_LINE_COLOUR',      'BLUE')
      CALL PSETC  ('CONTOUR_HIGHLIGHT_COLOUR', 'BLUE')
      CALL PCONT
      

C     Set up and plot the title text

      CALL PSETR ('TEXT_REFERENCE_CHARACTER_HEIGHT', 1.0)
      CALL PSETC ('TEXT_LINE_1', 'No data file')
      CALL PSETC ('TEXT_LINE_2', 'What does MAGICS do?')
      CALL PSETI ('TEXT_LINE_COUNT',     2)
      CALL PSETC ('TEXT_JUSTIFICATION', 'CENTRE')
      CALL PTEXT


C     Plot the coastlines

      CALL PCOAST


C     New page, this time using a different area specification
C     Area specification (SOUTH, WEST, NORTH, EAST )

      CALL PNEW  ('SUPER_PAGE')

      CALL PSETR ('SUBPAGE_LOWER_LEFT_LATITUDE',   20.0)
      CALL PSETR ('SUBPAGE_LOWER_LEFT_LONGITUDE',  20.0)
      CALL PSETR ('SUBPAGE_UPPER_RIGHT_LONGITUDE', 80.0)
      CALL PSETR ('SUBPAGE_UPPER_RIGHT_LATITUDE',  80.0)

      CALL PSETC ('TEXT_LINE_1',
     +            'Page 2 after missing data')


      CALL PCONT
      CALL PTEXT
      CALL PCOAST


      CALL PCLOSE

      STOP
      END



#include "parse_command_line.h"

