C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM fc_plot

!     Program written by Bjorn Hansen  Wed May 12 13:10:29 BST 1999

C      IMPLICIT NONE !
C
C      INTEGER            :: jf=0, ib=1, il=2
C      CHARACTER (LEN=19) :: cl_map_projection = 'CYLINDRICAL'
C      CHARACTER (LEN=19) :: cl_device = 'ps_vax_c'
C
Cc      READ *, ib, il, cl_map_projection, cl_device
C      ib = 1
C      il = 1
C      cl_map_projection = 'CYLINDRICAL'
C      cl_device = 'ps_vax_a3_c'
C      
C      IF ( il == 0 ) il = ib
C
C      WRITE(*,'("Device....: ", A)') cl_device(:LEN_TRIM(cl_device))
C      WRITE(*,'("Projection: ", A)')
C     x         cl_map_projection(:LEN_TRIM(cl_map_projection))
C      WRITE(*,'("Field ", I2, " to ", I2 )') ib, il
C
C      CALL layout(cl_device, cl_map_projection )
C      fields: DO jf=ib, il
C        CALL draw(jf)
C      ENDDO fields
C      CALL pclose
C
C      END PROGRAM fc_plot
C
C
C
C      SUBROUTINE layout (cd_device, cd_map_projection)
C      IMPLICIT NONE!
C
C      CHARACTER (LEN=*) ::  cd_map_projection
C      CHARACTER (LEN=*) ::  cd_device
C
C      CHARACTER (LEN=2), DIMENSION(2) ::  cla = (/'a3', 'a4'/)
C
C      REAL, DIMENSION(2) :: zs_pxl = (/42.0,  21.0 /)
C      REAL, DIMENSION(2) :: zs_pyl = (/29.7,  29.7 /)
C
C      REAL, DIMENSION(2) :: z__pxl = (/21.0,  29.7 /)
C      REAL, DIMENSION(2) :: z__pyl = (/14.84, 29.7 /)
C
C      INTEGER           ::  isp = 2
C
C      IF (index(cd_device, 'a3') == 7 .OR.
C     x    index(cd_device, 'a3') == 8 ) isp = 1
C
C      CALL popen
C      CALL PARSE_COMMAND_LINE ('metops_wave_height')
Cc      CALL psetc ('PS_DEVICE',                     cd_device         )
Cc      CALL pseti ('PS_RESOLUTION',                   400             )
Cc      CALL psetc ('PS_ACTION',                     'SHOWPS'          )
C        
C      CALL psetc ('PLOT_START',                    'TOP'             )
C      CALL psetc ('PLOT_DIRECTION',                'HORIZONTAL'      )
C
C      CALL psetc ('SUPER_PAGE_FRAME',              'ON'              )
C      CALL psetc ('SUPER_PAGE_FRAME_COLOUR',       'GREEN'             )
C      CALL psetr ('SUPER_PAGE_X_LENGTH',            zs_pxl(isp)      )
C      CALL psetr ('SUPER_PAGE_Y_LENGTH',            zs_pyl(isp)      )
C
C      CALL psetc ('PAGE_ID_LINE_USER_TEXT',        'bjorn hansen'    )
C      CALL psetc ('PAGE_FRAME',                    'ON'              )
C      CALL psetc ('PAGE_FRAME_COLOUR',             'BLACK'           )
C      CALL psetr ('PAGE_X_LENGTH',                   21.0            )
C      CALL psetr ('PAGE_Y_LENGTH',                   14.84           )
C
C      CALL psetr ('SUBPAGE_Y_LENGTH',                12.8            )
C      CALL psetr ('SUBPAGE_X_POSITION',               1.0            )
C      CALL psetr ('SUBPAGE_Y_POSITION',               0.8            )
Cc      CALL psetr ('SUBPAGE_LOWER_LEFT_LONGITUDE',   120.0            )
Cc      CALL psetr ('SUBPAGE_UPPER_RIGHT_LONGITUDE',  120.0            )
Cc      CALL psetr ('SUBPAGE_LOWER_LEFT_LATITUDE',    -90.0            )
Cc      CALL psetr ('SUBPAGE_UPPER_RIGHT_LATITUDE',    90.0            )
Cc      CALL psetc ('SUBPAGE_MAP_PROJECTION',        cd_map_projection )
Cc      CALL psetc ('SUBPAGE_MAP_AREA_DEFINITION',   'CENTRE'          )
Cc      CALL psetr ('SUBPAGE_MAP_CENTRE_LONGITUDE',   -35.0            )
Cc      CALL psetr ('SUBPAGE_MAP_CENTRE_LATITUDE',     37.5            )
Cc      CALL psetr ('SUBPAGE_MAP_VERTICAL_LONGITUDE', -12.5            )
Cc      CALL psetr ('SUBPAGE_MAP_SCALE',               80.0 * 10.0 ** 6)
C
C      CALL psetc ('MAP_COASTLINE',                 'ON'           )
C      CALL psetc ('MAP_COASTLINE_COLOUR',          'BLACK'        )
CC      CALL psetc ('MAP_COASTLINE_RESOLUTION',      'HIGH'         )
C      CALL pseti ('MAP_COASTLINE_THICKNESS',          2           )
CC      CALL psetc ('MAP_COASTLINE_RESOLUTION',      'HIGH'         )
C      CALL psetc ('MAP_COASTLINE_LAND_SHADE_COLOUR', 'CREAM'      )
C      CALL psetr ('MAP_LABEL_HEIGHT',                 0.19        )
C      CALL psetc ('MAP_GRID_COLOUR',               'BLACK'        )
C      CALL psetc ('MAP_GRID_LINE_STYLE',           'DOT'          )
C      CALL psetr ('MAP_GRID_LONGITUDE_REFERENCE',     0.0         )
C      CALL psetr ('MAP_GRID_LATITUDE_INCREMENT',     10.0         )
C      CALL psetc ('MAP_GRID_LINE_STYLE',           'DOT'          )
C
C      CALL psetc ('GRIB_INPUT_TYPE',               'FILE'         )
C      CALL psetc ('GRIB_INPUT_FILE_NAME',   'data/wave_height.grib')
Cc      CALL pseti ('GRIB_INPUT_FILE_UNIT',            23           )
C      CALL psetc ('GRIB_TEXT_EXPERIMENT',          'ON'           )
Cc      CALL psetc ('PS_FILE_NAME',                  'out.ps'       )
C
C      IF (cd_map_projection == 'POLAR_STEREOGRAPHIC') THEN
Cc    !WRITE(*, '(" psfile: ",a10)') 'law.'//cla(isp)//'.ps'
Cc    !CALL psetc ('PS_FILE_NAME',             'law.'//cla(isp)//'.ps')
C        CALL psetr ('SUBPAGE_X_LENGTH',                17.75       )
C        CALL pseti ('MAP_LABEL_LONGITUDE_FREQUENCY',    1          )
C        CALL pseti ('MAP_LABEL_LATITUDE_FREQUENCY',     1          )
C        CALL psetr ('MAP_GRID_LATITUDE_REFERENCE',     30.0        )
C        CALL psetr ('MAP_GRID_LONGITUDE_INCREMENT',    10.0        )
C        CALL psetr ('WIND_THINNING_FACTOR',            6.0         )
C      ELSE
Cc    !WRITE(*, '(" psfile: ",a10)') 'o.'//cla(isp)//'.ps'
Cc    !CALL psetc ('PS_FILE_NAME',             'o.'//cla(isp)//'.ps'  )
C        CALL psetr ('SUBPAGE_X_LENGTH',                19.00        )
C        CALL pseti ('MAP_LABEL_LONGITUDE_FREQUENCY',    2           )
C        CALL pseti ('MAP_LABEL_LATITUDE_FREQUENCY',     2           )
C        CALL psetr ('MAP_GRID_LATITUDE_REFERENCE',      0.0         )
C        CALL psetr ('MAP_GRID_LONGITUDE_INCREMENT',    20.0         )
C        CALL psetr ('WIND_THINNING_FACTOR',            4.0          )
C      ENDIF
C      RETURN
C      END SUBROUTINE layout
C
C
C
C      SUBROUTINE draw (kf)
C      IMPLICIT NONE!
C  
C      INTEGER           :: kf, jl, jj1, jj2
C
C      INTEGER, PARAMETER:: jpl=9, jpr=512
C
C      CHARACTER (LEN=10), DIMENSION(6)::  colo
C
C      REAL                    :: zhight = 0.25
C      REAL,    DIMENSION(jpl) :: zfcl, zden, zdos=0.02, zang=45.0
C      INTEGER, DIMENSION(jpl) :: icol
C      INTEGER, DIMENSION(jpr) :: ib1par, ib2par
C
C      DATA zfcl   /  0., 0.5,  1., 1.5,  2., 3., 4., 8., 25./
Cc  ! DATA zden   /100., 300., 600., 300., 600., 300., 600., 600./
Cc  ! DATA icol   /  4,    4,    4,    5,    5,    3,    3,    6 /
Cc  ! Fix for Linux cluster, Cihan 11/2005
C      DATA zden   /100., 300., 600., 300., 600., 300., 600., 600., 600./
C      DATA icol   /  4,    4,    4,    5,    5,    3,    3,    6, 6 /
C  
C      colo = ' '
C      colo(3) = 'BLUE'
C      colo(4) = 'YELLOW'
C      colo(5) = 'CYAN'
C      colo(6) = 'MAGENTA'
C
C        WRITE(*,'(" Field ", I2, " ", $)') kf ; CALL flush(6)
C        jj1=(kf-1)*2+1
C        jj2=kf*2
C        CALL psetc ('MAP_COASTLINE_LAND_SHADE',       'ON'     )
C        CALL pcoast
C        CALL psetr ('GRIB_MISSING_VALUE_INDICATOR',    0.0     )
C        CALL pset1i ('GRIB_PRODUCT_BLOCK',IB1PAR,      jpr     )
C        CALL pset1i ('GRIB_GRID_BLOCK',IB2PAR,         jpr     )
C        CALL pseti ('GRIB_FIELD_POSITION',             jj1     )
C        CALL pgrib
C
Cc    ! Draw the shadings.
Cc    ! ------------------
C        CALL psetc ('LEGEND',                         'ON'          )
C        CALL psetc ('LEGEND_BORDER',                  'OFF'         )
C        CALL psetc ('LEGEND_BORDER_COLOUR',           'BLACK'       )
C        CALL psetc ('LEGEND_ENTRY',                   'ON'          )
C        CALL psetc ('LEGEND_TITLE',                   'ON'          )
C        CALL psetc ('LEGEND_DISPLAY_TYPE',            'CONTINUOUS'  )
C        CALL psetc ('LEGEND_USER_TEXT',               'cllutxt'     )
C        CALL psetc ('LEGEND_TITLE_TEXT',              'm'           )
C        CALL psetc ('LEGEND_TEXT_QUALITY',            'HIGH'        )
C        CALL psetc ('LEGEND_TEXT_COLOUR',             'BLACK'       )
C        CALL psetc ('LEGEND_AUTOMATIC_TEXT_EXTREMA',  'BOTH'        )
C        CALL psetr ('LEGEND_BOX_Y_LENGTH',             0.3          )
C        CALL psetr ('LEGEND_TEXT_MAXIMUM_HEIGHT',      0.25         )
C        CALL psetr ('LEGEND_ENTRY_MAXIMUM_HEIGHT',     0.25         )
C        CALL psetc ('CONTOUR',                        'OFF'         )
C        CALL psetc ('CONTOUR_SHADE',                  'ON'          )
C        CALL psetc ('CONTOUR_LABEL',                  'OFF'         )
C        CALL psetc ('CONTOUR_HILO',                   'OFF'         )
C        shades: DO jl = 2, jpl - 1
C          CALL psetr ('CONTOUR_REFERENCE_LEVEL',         zfcl(jl)      )
C          CALL psetr ('CONTOUR_SHADE_MIN_LEVEL_DENSITY', zden(jl)      )
C          CALL psetr ('CONTOUR_SHADE_MAX_LEVEL_DENSITY', zden(jl)      )
C          CALL PSETC ('CONTOUR_SHADE_TECHNIQUE',  'POLYGON_SHADING')
C          CALL PSETC ('CONTOUR_SHADE_METHOD',     'AREA_FILL')
C          CALL psetr ('CONTOUR_SHADE_DOT_SIZE',          zdos(jl)      )
C          CALL psetr ('CONTOUR_SHADE_ANGLE',             zang(jl)      )
C          CALL psetr ('CONTOUR_MIN_LEVEL',               zfcl(jl)      )
C          CALL psetr ('CONTOUR_MAX_LEVEL',               zfcl(jl+1)    )
C          CALL psetr ('CONTOUR_SHADE_MIN_LEVEL',         zfcl(jl)      )
C          CALL psetr ('CONTOUR_SHADE_MAX_LEVEL',         zfcl(jl+1)    )
C          CALL PSETC('CONTOUR_SHADE_MIN_LEVEL_COLOUR',   colo(icol(jl)))
C          CALL PSETC('CONTOUR_SHADE_MAX_LEVEL_COLOUR',   colo(icol(jl)))
C
C          CALL psetr ('CONTOUR_INTERVAL',               
C     x                 zfcl(jl+1)-zfcl(jl) )
C          CALL pcont
C          WRITE(*, '("l.",$)') ; CALL flush(6)
C        ENDDO shades
C        WRITE(*, '("-")')
C
Cc    ! Draw the contour lines.
Cc    ! -----------------------
C        CALL psetc ('LEGEND_ENTRY',                   'OFF'           )
C        CALL psetc ('CONTOUR',                        'ON'            )
C        CALL psetc ('CONTOUR_SHADE',                  'OFF'           )
C        CALL psetc ('CONTOUR_HIGHLIGHT',              'OFF'           )
C        CALL psetc ('CONTOUR_LABEL',                  'ON'            )
C        CALL psetc ('CONTOUR_LABEL_QUALITY',          'HIGH'          )
C        CALL psetr ('CONTOUR_LABEL_HEIGHT',            zhight         )
C        CALL pseti ('CONTOUR_LABEL_FREQUENCY',         1              )
C        CALL psetr ('CONTOUR_MIN_LEVEL',               zfcl(2)        )
C        CALL psetc ('CONTOUR_LEVEL_SELECTION_TYPE',   'LEVEL_LIST'    )
C        CALL pset1r('CONTOUR_LEVEL_LIST',              zfcl(2),jpl-1  )
C        CALL psetc ('CONTOUR_LINE_COLOUR',            'BLUE'          )
C        CALL pseti ('CONTOUR_HIGHLIGHT_FREQUENCY',     5              )
C        CALL pseti ('CONTOUR_HIGHLIGHT_THICKNESS',     2              )
Cc        CALL psetc ('CONTOUR_METHOD',                 'C3'            )
C        CALL pcont
C
Cc    ! Draw the arrows.
Cc    ! ----------------
C        CALL psetc ('LEGEND',                         'ON'         )
C        CALL psetc ('LEGEND_ENTRY',                   'ON'         )
C        CALL psetc ('LEGEND_USER_TEXT',               '10m'        )
C        CALL psetc ('LEGEND_TEXT_COMPOSITION',        'BOTH'       )
C        CALL psetc ('WIND_ARROW_LEGEND',              'OFF'        )
C        CALL psetr ('WIND_ARROW_UNIT_VELOCITY',       10.0         )
C        CALL psetc ('WIND_ARROW_COLOUR',              'BLACK'      )
C        CALL pseti ('GRIB_WIND_POSITION_1',           jj1          )
C        CALL pseti ('GRIB_WIND_POSITION_2',           jj2          )
C        CALL psetc ('GRIB_WIND_MODE',                 'SD')
C        WRITE(*,'("UV ", I2,I2 )') jj1, jj2
C        CALL pgrib
C        CALL pwind
C        CALL pseti ('TEXT_LINE_COUNT',                 2           )
C        CALL psetc ('TEXT_COLOUR',                    'BLACK'      )
C        CALL ptext
C        CALL psetc ('MAP_COASTLINE_LAND_SHADE',       'OFF'        )
C        CALL pcoast
C        CALL pnew('PAGE')
C
C        RETURN
C
C      END SUBROUTINE draw



C --------------------------------------------------------------------
C     PARSE_COMMAND_LINE
C     Checks the command-line for any arguments.
C     Arguments can come in pairs. Currently supported arguments are:
C     PROJECTION <CYLINDRICAL | POLAR_STEREOGRAPHIC>
C     DEVICE <PS | SVG | PNG>
C     e.g. Run the program with:
C       <progname> PROJECTION CYLINDRICAL  DEVICE SVG
C --------------------------------------------------------------------

      SUBROUTINE PARSE_COMMAND_LINE (OUTROOTNAME)

      CHARACTER*32 ARG
      CHARACTER*64 ID_TEXT
      CHARACTER*32 PROJECTION
      CHARACTER*32 DEVICE
      CHARACTER*48 EXENAME
      CHARACTER*8  MINIMAL
      CHARACTER*8  SEPARATOR
      CHARACTER*64 OUTNAME
      CHARACTER*(*) OUTROOTNAME

      CHARACTER*16 FORMATS_PS_GIF
      DIMENSION    FORMATS_PS_GIF(2)
      DATA         FORMATS_PS_GIF /'PS', 'GIF_ANIMATION'/

      CHARACTER*16 FORMATS_PS_GIF_PDF
      DIMENSION    FORMATS_PS_GIF_PDF(3)
      DATA         FORMATS_PS_GIF_PDF /'PS', 'GIF_ANIMATION', 'PDF'/

      CHARACTER*16 FORMATS_PS_GIF_SVG
      DIMENSION    FORMATS_PS_GIF_SVG(3)
      DATA         FORMATS_PS_GIF_SVG /'PS', 'GIF_ANIMATION', 'SVG'/

      CHARACTER*16 FORMATS_PS_PNG_SVG
      DIMENSION    FORMATS_PS_PNG_SVG(3)
      DATA         FORMATS_PS_PNG_SVG /'PS', 'PNG', 'SVG'/

      CHARACTER*16 FORMATS_PS_GIF_PNG_SVG
      DIMENSION    FORMATS_PS_GIF_PNG_SVG(4)
      DATA         FORMATS_PS_GIF_PNG_SVG /'PS', 'GIF_ANIMATION', 
     +                                     'PNG', 'SVG'/

      CHARACTER*16 FORMATS_ALL
      DIMENSION    FORMATS_ALL(5)
      DATA         FORMATS_ALL /'PS', 'SVG', 'PDF',
     +                          'KML', 'PNG'/

      INTEGER NUM_ARGS
      INTEGER DEVICE_SET

      DEVICE_SET = 0

      ID_TEXT = ''

      NUM_ARGS = IARGC()

      I = 1
C
C  Start GoTo
C
20    IF (I.LE.NUM_ARGS) THEN
          CALL GETARG ( I, ARG )
C
C         Set the projection?
C
          IF (ARG.EQ.'PROJECTION') THEN
              I = I + 1 
              CALL GETARG ( I, PROJECTION ) 
              CALL PSETC ('SUBPAGE_MAP_PROJECTION', PROJECTION)
C
C        Set the device?
C
          ELSEIF (ARG.EQ.'DEVICE') THEN
              I = I + 1 
              CALL GETARG ( I, DEVICE ) 

C             Set the output filename

              IF     (DEVICE.EQ.'PS')  THEN
                OUTNAME = OUTROOTNAME //   '.ps'
                CALL PSETC ('DEVICE',       DEVICE)
                CALL PSETC ('PS_DEVICE',   'ps_a4')
                CALL PSETC ('PS_FILE_NAME', OUTNAME)
              ELSEIF (DEVICE.EQ.'PS_NEW') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'PS')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'EPS') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'EPS')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'GIF') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'GIF_ANIMATION')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
                CALL PSETI ('OUTPUT_GIF_DELAY',     150)
              ELSEIF (DEVICE.EQ.'GIF_MULTI') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'GIF')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'PNG') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'PNG')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'JPEG') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'JPEG')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'SVG') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'SVG')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'CSVG') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'CSVG')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'KML') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'KML')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)

              ELSEIF (DEVICE.EQ.'BAD') THEN
                CALL PSETC ('OUTPUT_FORMAT',  'BAD')
                CALL PSETC ('OUTPUT_NAME', OUTROOTNAME)

              ELSEIF (DEVICE.EQ.'PS_GIF') THEN
                CALL PSET1C ('OUTPUT_FORMATS', FORMATS_PS_GIF, 2)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'PS_GIF_PDF') THEN
                CALL PSET1C ('OUTPUT_FORMATS', FORMATS_PS_GIF_PDF, 3)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'PS_GIF_SVG') THEN
                CALL PSET1C ('OUTPUT_FORMATS', FORMATS_PS_GIF_SVG, 3)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'PS_PNG_SVG') THEN
                CALL PSET1C ('OUTPUT_FORMATS', FORMATS_PS_PNG_SVG, 3)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'PS_GIF_PNG_SVG') THEN
                CALL PSET1C ('OUTPUT_FORMATS',
     +                               FORMATS_PS_GIF_PNG_SVG, 4)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSEIF (DEVICE.EQ.'ALL') THEN
                CALL PSET1C ('OUTPUT_FORMATS', FORMATS_ALL, 5)
                CALL PSETC  ('OUTPUT_NAME', OUTROOTNAME)
              ELSE
                WRITE(*, '(A)') 'BAD DEVICE: ', DEVICE
              ENDIF
            
            DEVICE_SET = 1
C
C        Split the PostScript pages into separate files?
C
          ELSEIF (ARG.EQ.'PS_SPLIT') THEN
                CALL PSETC ('OUTPUT_PS_SPLIT',     'ON')
C
C        Turn on the numbering for the first page?
C
          ELSEIF (ARG.EQ.'FIRST_PAGE_NUMBER') THEN
                CALL PSETC ('OUTPUT_NAME_FIRST_PAGE_NUMBER', 'ON')
C
C         Set the minimal filename number width?
C
          ELSEIF (ARG.EQ.'MINIMAL') THEN
              I = I + 1 
              CALL GETARG (I, MINIMAL) 
              IF     (MINIMAL.EQ.'0')  THEN
                CALL PSETI ('OUTPUT_FILE_MINIMAL_WIDTH', 0)
              ELSEIF (MINIMAL.EQ.'1')  THEN
                CALL PSETI ('OUTPUT_FILE_MINIMAL_WIDTH', 1)
              ELSEIF (MINIMAL.EQ.'2') THEN
                CALL PSETI ('OUTPUT_FILE_MINIMAL_WIDTH', 2)
              ELSEIF (MINIMAL.EQ.'3') THEN
                CALL PSETI ('OUTPUT_FILE_MINIMAL_WIDTH', 3)
              ENDIF
C
C         Set the file number separator?
C
          ELSEIF (ARG.EQ.'SEP') THEN
              I = I + 1 
              CALL GETARG ( I, SEPARATOR ) 
              CALL PSETC ('OUTPUT_FILE_SEPARATOR', SEPARATOR)
C
C        Run using linear contouring?
C
          ELSEIF (ARG.EQ.'LINEAR') THEN
                CALL PSETC ('CONTOUR_METHOD', 'LINEAR')
                CALL PSETC ('PAGE_ID_LINE_USER_TEXT_PLOT', 'ON')
                CALL PSETC ('PAGE_ID_LINE_USER_TEXT', 'LINEAR')
          ENDIF
          I = I + 1 
          GOTO 20
      ENDIF
C
C     If no device has been set, then use PostScript by default
C
      IF (DEVICE_SET.EQ.0) THEN
        OUTNAME = OUTROOTNAME // '.ps'
        CALL PSETC ('PS_DEVICE',    'ps_a4')
        CALL PSETC ('PS_FILE_NAME', OUTNAME)
      ENDIF
C
      END
