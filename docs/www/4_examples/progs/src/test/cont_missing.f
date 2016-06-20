C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM CONTMISSING

C     This program demonstrates how MAGICS handles missing data
C     in its plots.


C     Open MAGICS and set the output device

      CALL POPEN
      CALL PARSE_COMMAND_LINE ('cont_missing')


C     Set up the coastline attributes

      CALL PSETC ('MAP_COASTLINE_COLOUR', 'GREY')
      CALL PSETC ('MAP_GRID_COLOUR',      'GREY')     


C     Pass the data to MAGICS

      CALL PSETC ('GRIB_INPUT_TYPE',      'FILE')
      CALL PSETC ('GRIB_INPUT_FILE_NAME', 'data/sst.grib')
      CALL PSETC ('GRIB_SPECIFICATION',   'OFF')
#if defined (MAGPLUS)
      CALL PSETC ('GRIB_AUTOMATIC_SCALING', 'OFF')
#else
      CALL PSETC ('GRIB_SCALING', 'OFF')
#endif
      CALL PGRIB
      

C     Define the contour     

      CALL PSETC  ('CONTOUR_LINE_COLOUR',      'BLUE')
      CALL PSETC  ('CONTOUR_HIGHLIGHT_COLOUR', 'BLUE')
      CALL PCONT


C     Set up and plot the title text

      CALL PSETC ('TEXT_LINE_1', 'Contours with missing values')
      CALL PSETI ('TEXT_LINE_COUNT',1)
      CALL PSETC ('TEXT_JUSTIFICATION','CENTRE')
      CALL PTEXT


C     Plot the coastlines

      CALL PCOAST


C     ------------------------------------------------------------
C     New page - this time with solid shading
C     ------------------------------------------------------------

      CALL PNEW ('SUPER_PAGE')


C     Set up the new contouring parameters

      CALL PSETC  ('CONTOUR_SHADE',            'ON')      
      call PSETC  ('CONTOUR_SHADE_TECHNIQUE',  'POLYGON_SHADING')
      call PSETC  ('CONTOUR_SHADE_METHOD',     'AREA_FILL')


C     Plot everything

      CALL PSETC ('TEXT_LINE_1', 'Solid shading with missing values')
      CALL PTEXT
      CALL PCOAST
      CALL PCONT

C     ------------------------------------------------------------
C     New page - this time with cell shading
C     ------------------------------------------------------------

      CALL PNEW ('SUPER_PAGE')


C     Set up the new contouring parameters

      CALL PSETC ('CONTOUR_SHADE',            'ON')      
      CALL PSETC ('CONTOUR_SHADE_TECHNIQUE',  'CELL_SHADING')
      CALL PSETR ('CONTOUR_SHADE_CELL_RESOLUTION', 1.0)
c      CALL PSETR ('CONTOUR_SHADE_CELL_RESOLUTION', 4.0) 


C     Plot everything

      CALL PSETC ('TEXT_LINE_1', 'Cell shading with missing values')
      CALL PTEXT
      CALL PCOAST
      CALL PCONT


C     ------------------------------------------------------------
C     New page - this time showing grid values
C     ------------------------------------------------------------

      CALL PNEW ('SUPER_PAGE')


C     Area specification (SOUTH, WEST, NORTH, EAST )

      CALL PSETR ('SUBPAGE_LOWER_LEFT_LATITUDE',    30.0)
      CALL PSETR ('SUBPAGE_LOWER_LEFT_LONGITUDE',  -20.0)
      CALL PSETR ('SUBPAGE_UPPER_RIGHT_LATITUDE',   60.0)
      CALL PSETR ('SUBPAGE_UPPER_RIGHT_LONGITUDE',  20.0)

      
C     Set up the new contouring parameters

      CALL PSETC ('CONTOUR_SHADE',                'OFF')      
      CALL PSETC ('CONTOUR_GRID_VALUE_PLOT_TYPE', 'BOTH')
      CALL PSETC ('CONTOUR_GRID_VALUE_PLOT',      'ON')
      CALL PSETI ('CONTOUR_GRID_VALUE_LAT_FREQUENCY', 1)
      CALL PSETI ('CONTOUR_GRID_VALUE_LON_FREQUENCY', 1)
      CALL PSETR ('CONTOUR_GRID_VALUE_HEIGHT',        0.3)
      CALL PSETR ('CONTOUR_GRID_VALUE_MARKER_HEIGHT', 0.2)


C     Plot everything

      CALL PSETC ('TEXT_LINE_1', 'Grid values with missing values')
      CALL PTEXT
      CALL PCOAST
      CALL PCONT


C     Finish up

      CALL PCLOSE

      STOP
      END



#include "parse_command_line.h"
