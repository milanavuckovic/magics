C (C) Copyright 1996-2016 ECMWF.
C 
C This software is licensed under the terms of the Apache Licence Version 2.0
C which can be obtained at http://www.apache.org/licenses/LICENSE-2.0. 
C In applying this licence, ECMWF does not waive the privileges and immunities 
C granted to it by virtue of its status as an intergovernmental organisation nor
C does it submit to any jurisdiction.
C

      PROGRAM METOPSENPLOT
C
C      PROGRAM TO PLOT ENSEMBLE FORECASTS
C             PLUS CONTROL, OPERATIONAL (AND ANALYSIS)
C
C     DATA ON FILE cluster  IS CLUSTER MEAN
C     DATA ON FILE contoper IS CONTROL AND OPERATIONAL
C     DATA ON FILE analysis IS ANALYSIS
C     DATA ON FILE ensemble IS ENSEMBLES
C
C
      parameter(nlon=640,nlat=241,nlen=nlon*nlat)
      parameter(ngrib=nlen,nbytes=4,ngbytes=ngrib*nbytes)
      parameter(nmaxc=6,nens=9)
c      parameter(nmaxc=6,nens=50)
      dimension iens(0:nens),indc(2)
      dimension ksec0(2),ksec1(912),ksec2(900),psec2(900),
     1  ksec3(2),psec3(200),ksec4(99000),kgrib(ngrib)
      character*130 cline1,cline2
      character*22 cname(2),yfile
      character*90 cval
      character*200 title
      character*15 clusters(6)
      character*5 number
      logical lan

      data clusters /'\\CLBLUE_GREEN\\','\\CLORANGE\\',
     x '\\CLGREEN\\','\\CLMAGENTA\\','\\CLGOLD\\','\\CLKHAKI\\'/
c
c  -------------------------------------------------------------------
c
c            1.  Initialisation
c           --------------------
c
  100 continue
      lan=.FALSE.
      cname(1)='Cntr T399'
      cname(2)='OPER T799'
      yfile='data/enplot_cluster'
c
c       1.1  Unit numbers
c
      call pbopen (iunm,yfile,'r',iret)
      if(iret.ne.0) then
           write (*,'("  unable to open file ",a," error= ",i5)')
     x                   yfile,iret
           call abort
      endif
      iuna=21
C
C       1.2  EXTRACT DETAILS FROM GRIB SECTION 1 OF CLUSTERS
C
      do 126 k=1,nmaxc
      call pbgrib (iunm,kgrib,ngbytes,koutlen,iret)
      if(iret.eq.-1) then
           go to 312
      elseif(iret.ne.0) then
           go to 314
      endif
      iflen=((koutlen-1)/nbytes)+1
      call gribex (ksec0,ksec1,ksec2,psec2,ksec3,psec3,ksec4,
     1             field,1,kgrib,iflen,kword,'I',iret)
      if(iret.ne.0) then
           write (*,'("  GRIBEX error ",i5)') iret
           call abort
      endif
      ninc=ksec1(53)
      do 124 j=1,ninc
      iens(ksec1(53+j))=ksec1(42)
  124 continue
  126 continue
      indc(2)=ksec1(51)
      indc(1)=ksec1(52)
      call pbclose (iunm,iret)
c
c   ----------------------------------------------------------------
c
c                    2.  Plot fields
c                   -----------------
c
  200 continue
c
c           2.1  Open MAGICS
c
      CALL POPEN
      CALL PARSE_COMMAND_LINE ('metops_enplot')
c      CALL PSETC ('PS_FILE_NAME', 'metops_enplot.ps')


C
C           2.2  DEFINE PAGE SIZE AND MAPPING PARAMETERS
C
C - removed by Iain
C      CALL PSETC('WORKSTATION_1','PS_A3_COL')
      
C - added by Iain
      CALL PSETC ('PS_DEVICE', 'ps_a3')
      
      CALL PSETR('SUPER_PAGE_X_LENGTH',42.0)
      CALL PSETR('SUPER_PAGE_Y_LENGTH',29.7)
      CALL PSETC('PLOT_START','TOP')
      CALL PSETC('PLOT_DIRECTION','HORIZONTAL')
      CALL PSETR('PAGE_X_LENGTH',4.19)
      CALL PSETR('PAGE_Y_LENGTH',4.94)
      CALL PSETC('PAGE_FRAME','ON')
c - removed by Iain
c      CALL PSETC('PAGE_FRAME_COLOUR','BACK')
c - added by Iain
      CALL PSETC('PAGE_FRAME_COLOUR','BLACK')
      CALL PSETC('SUBPAGE_FRAME_COLOUR','BLACK')
      CALL PSETC('PAGE_ID_LINE','OFF')
      CALL PSETC('SUBPAGE_MAP_PROJECTION','POLAR_STEREOGRAPHIC')
      CALL PSETC('SUBPAGE_MAP_HEMISPHERE','NORTH')
      CALL PSETC('SUBPAGE_MAP_AREA_DEFINITION','CENTRE')
      CALL PSETR('SUBPAGE_MAP_CENTRE_LATITUDE',55.)
      CALL PSETR('SUBPAGE_MAP_CENTRE_LONGITUDE',10.)
      CALL PSETR('SUBPAGE_MAP_VERTICAL_LONGITUDE',10.0)
      CALL PSETR('SUBPAGE_MAP_SCALE',125E6)   
      CALL PSETC('SUBPAGE_MAP_HEMISPHERE','NORTH')
      CALL PSETC('SUBPAGE_FRAME','ON')
      CALL PSETC('MAP_GRID_LINE_STYLE','DOT')
      CALL PSETR('MAP_GRID_LATITUDE_INCREMENT',20.0)
      CALL PSETR('MAP_GRID_LONGITUDE_INCREMENT',20.0)
      CALL PSETC('MAP_LABEL','OFF')
      CALL PSETC('TEXT_QUALITY','HIGH')
c - removed by Iain
c      CALL PSETC('MAP_COASTLINE_RESOLUTION','HIGH')
c - added by Iain
c      CALL PSETC('MAP_COASTLINE_RESOLUTION','VERY_LOW')
c - removed by Iain
c      CALL PSETC('MAP_COASTLINE_COLOUR','RUST')
c - added by Iain
      CALL PSETC('MAP_COASTLINE_COLOUR','CREAM')
      CALL PSETC('MAP_GRID_COLOUR','GREY')
      CALL PSETC('MAP_COASTLINE_LAND_SHADE','ON')
      CALL PSETC('MAP_COASTLINE_LAND_SHADE_COLOUR','CREAM')

      CALL PSETC('GRIB_INPUT_TYPE','FILE')

C         2.3  LOOP OVER CONTROL AND OPERATIONS

      do 235 ic=1,2
C
      CALL PCOAST
C
C              PLOT THE FIELD
      CALL PSETC('GRIB_INPUT_FILE_NAME','data/enplot_msl_ct_op')
      CALL PSETI('GRIB_FIELD_POSITION',ic)
      CALL PGRIB

      if (ic.eq.2) then
           CALL PENQC ('TEXT_LINE_1',cline1)
           CALL PENQC ('TEXT_LINE_2',cline2)
      endif

      CALL PSETC('CONTOUR_HILO','ON')
      CALL PSETC('CONTOUR_HILO_TYPE','TEXT')
      CALL PSETC('CONTOUR_HILO_QUALITY','HIGH')
      CALL PSETR('CONTOUR_HILO_REDUCTION_RADIUS',20.)
      CALL PSETR('CONTOUR_HILO_SUPPRESS_RADIUS',25.)
      CALL PSETR('CONTOUR_HILO_HEIGHT',0.20)
      CALL PSETI('CONTOUR_LABEL_FREQUENCY',1)
      CALL PSETR('CONTOUR_LABEL_HEIGHT',0.15)
      CALL PSETR('CONTOUR_REFERENCE_LEVEL',1015.)
      CALL PSETC('CONTOUR_HIGHLIGHT','ON')
      CALL PSETI('CONTOUR_HIGHLIGHT_FREQUENCY',20)
      CALL PSETC('CONTOUR_HIGHLIGHT_STYLE','SOLID')
      CALL PSETI('CONTOUR_HIGHLIGHT_THICKNESS',4)
      CALL PSETC('contour_highlight_colour','black')
      CALL PSETC('contour_line_colour','black')
      CALL PSETC('CONTOUR_LINE_STYLE','SOLID')
      CALL PSETR('CONTOUR_INTERVAL',5.0)
      CALL PCONT



CC    T850 PLOT
      CALL PSETC('GRIB_INPUT_FILE_NAME','data/enplot_t_ct_op')
      CALL PSETI('GRIB_FIELD_POSITION',ic)
      CALL PGRIB

ccc   -6 and 16 C lines in different colours
c      CALL PSETR('contour_min_level',-6.)
c      CALL PSETR('contour_max_level',16.)
c      CALL PSETC('contour_level_selection_type','interval')
c      CALL PSETR('contour_interval',22.)
c      CALL PSETR('contour_reference_level',16.)
      CALL PSETC('contour_line_style','solid')
      CALL PSETI('contour_line_thickness',3)   
      CALL PSETC('contour_line_colour','cyan')
      CALL PSETI('contour_label_frequency',1)
      CALL PSETI('contour_highlight_frequency',2)
      CALL PSETC('contour_highlight_colour','red')
      CALL PSETI('contour_highlight_thickness',3)
      CALL PSETC('contour_hilo','off')
      CALL PCONT   


C
C               PLOT TITLE TEXT
      CALL PSETC('TEXT_COLOUR','BLACK')
      CALL PSETC('TEXT_MODE','TITLE')
      CALL PSETC('TEXT_BORDER','OFF')
      CALL PSETI('TEXT_LINE_COUNT',1)
      cval=cname(ic)
      indx=index(clusters(indc(ic)),' ')
      write (number,'(i1)') indc(ic)
      CALL PSETC('TEXT_LINE_1',cval(1:9)//
     &clusters(indc(ic))(1:indx-1)//
     &'                          Cluster ' // ' '//number)
      CALL PTEXT
      CALL PNEW('PAGE')
  235 CONTINUE


C          2.4  TEST WHETHER ANALYSIS AVAILABLE
C
      open(iuna,file='analysis',form='UNFORMATTED',status='OLD',
     x     err=242)
      lan=.TRUE.
      close (iuna)
  242 continue
      if(lan) then
C
           CALL PCOAST
C
C              PLOT THE FIELD
C
           CALL PSETC('GRIB_INPUT_FILE_NAME','data/enplot_analysis')
           CALL PSETI('GRIB_FIELD_POSITION',1)
           CALL PGRIB
           CALL PSETC('CONTOUR_HILO','ON')
           CALL PSETC('CONTOUR_HILO_TYPE','TEXT')
           CALL PSETC('CONTOUR_HILO_QUALITY','HIGH')
       CALL PSETR('CONTOUR_HILO_REDUCTION_RADIUS',20.)
           CALL PSETR('CONTOUR_HILO_SUPPRESS_RADIUS',25.)
           CALL PSETR('CONTOUR_HILO_HEIGHT',0.20)
           CALL PSETI('CONTOUR_LABEL_FREQUENCY',1)
           CALL PSETR('CONTOUR_LABEL_HEIGHT',0.15)
           CALL PSETC('CONTOUR_HIGHLIGHT','ON')
           CALL PSETI('CONTOUR_HIGHLIGHT_FREQUENCY',20)
           CALL PSETR('CONTOUR_REFERENCE_LEVEL',1015.)
           CALL PSETC('CONTOUR_HIGHLIGHT_STYLE','SOLID')
           CALL PSETC('contour_highlight_colour','black')
           CALL PSETC('contour_line_colour','black')
           CALL PSETI('CONTOUR_HIGHLIGHT_THICKNESS',4)
           CALL PSETC('CONTOUR_LINE_STYLE','SOLID')
           CALL PSETR('CONTOUR_INTERVAL',5.0)
           CALL PCONT

           CALL PSETC('TEXT_LINE_1','Verifying Analysis')


      else
           CALL PSETC('TEXT_LINE_1',' ')
           CALL PSETC('SUBPAGE_FRAME','OFF')
      endif
C
C               PLOT TITLE TEXT
C
      CALL PSETC('TEXT_MODE','TITLE')
      CALL PSETC('TEXT_BORDER','OFF')
      CALL PSETI('TEXT_LINE_COUNT',1)
      CALL PTEXT
      CALL PNEW('PAGE')
C
C          2.5  PLOT SUMMARY TEXT
C
      CALL PSETC ('SUBPAGE_FRAME','OFF')
      CALL PSETR ('PAGE_X_LENGTH',29.33)
      CALL PSETC ('TEXT_MODE','POSITIONAL')
      CALL PSETR ('TEXT_BOX_X_POSITION',1.0)
      CALL PSETR ('TEXT_BOX_Y_POSITION',1.25)
      CALL PSETR ('TEXT_BOX_X_LENGTH',27.0)
      CALL PSETR ('TEXT_BOX_Y_LENGTH',2.5)
      CALL PSETC ('TEXT_JUSTIFICATION','LEFT')
      CALL PSETI ('TEXT_LINE_COUNT',3)
      CALL PSETC ('TEXT_LINE_1','ECMWF ENSEMBLE FORECASTS')
      CALL PSETC ('TEXT_LINE_2',cline1)
      title = 'MSLP (contour every 5hPa)   and Temperature at 850hPa'
     &//' (only\\CLCYAN\\-6 \\CLBLACK\\ and'
     &// ' \\CLRED\\16\\CLBLACK\\ isolines are plotted)'
      CALL PSETC ('TEXT_LINE_3',title)
      CALL PTEXT
      CALL PNEW('PAGE')
      CALL PRESET('SUBPAGE_FRAME')
      CALL PRESET('TEXT_MODE')
      CALL PSETR('PAGE_X_LENGTH',4.19)
C
C         2.6  LOOP OVER ENSEMBLES
C
      do 265 ie=1,nens
C
      CALL PCOAST
C
C              PLOT THE FIELD
C
      CALL PSETC('GRIB_INPUT_FILE_NAME','data/enplot_ensemble')
      CALL PSETI('GRIB_FIELD_POSITION',ie)
      CALL PGRIB
      CALL PSETC('CONTOUR_HILO','ON')
      CALL PSETC('CONTOUR_HILO_TYPE','TEXT')
      CALL PSETC('CONTOUR_HILO_QUALITY','HIGH')
      CALL PSETR('CONTOUR_HILO_REDUCTION_RADIUS',20.)
      CALL PSETR('CONTOUR_HILO_SUPPRESS_RADIUS',25.)
      CALL PSETR('CONTOUR_HILO_HEIGHT',0.20)
      CALL PSETI('CONTOUR_LABEL_FREQUENCY',1)
      CALL PSETR('CONTOUR_LABEL_HEIGHT',0.15)
      CALL PSETC('CONTOUR_HIGHLIGHT','ON')
      CALL PSETI('CONTOUR_HIGHLIGHT_FREQUENCY',20)
      CALL PSETC('CONTOUR_HIGHLIGHT_STYLE','SOLID')
      CALL PSETR('CONTOUR_REFERENCE_LEVEL',1015.)
      CALL PSETC('contour_highlight_colour','black')
      CALL PSETC('contour_line_colour','black')
      CALL PSETI('CONTOUR_HIGHLIGHT_THICKNESS',4)
      CALL PSETC('CONTOUR_LINE_STYLE','SOLID')
      CALL PSETR('CONTOUR_INTERVAL',5.0)
      CALL PCONT

CC    T850 PLOT
      CALL PSETC('GRIB_INPUT_FILE_NAME','data/enplot_t_ens')
      CALL PSETI('GRIB_FIELD_POSITION',ie)
      CALL PGRIB

ccc   -6 and 16 C lines in different colours
      CALL PSETR('contour_min_level',-6.)
      CALL PSETR('contour_max_level',16.)
      CALL PSETC('contour_level_selection_type','interval')
      CALL PSETR('contour_interval',22.)
      CALL PSETR('contour_reference_level',16.)
      CALL PSETC('contour_line_style','solid')
      CALL PSETI('contour_line_thickness',3)   
      CALL PSETC('contour_line_colour','cyan')
      CALL PSETI('contour_label_frequency',1)
      CALL PSETI('contour_highlight_frequency',2)
      CALL PSETC('contour_highlight_colour','red')
      CALL PSETI('contour_highlight_thickness',3)
      CALL PSETC('contour_hilo','off')
      CALL PCONT


C
C               PLOT TITLE TEXT
C
      CALL PSETC('TEXT_MODE','TITLE')
      CALL PSETC('TEXT_BORDER','OFF')
      CALL PSETI('TEXT_LINE_COUNT',1)
      write (cval,'(a7,i2)')'Member ',ie
      indx=index(clusters(iens(ie)),' ')
      write (number,'(i1)') iens(ie)
      CALL PSETC('TEXT_LINE_1',cval(1:9)//
     &clusters(iens(ie))(1:indx-1)//
     &'                          Cluster ' // ' '//number)
      CALL PTEXT
      CALL PNEW('PAGE')
  265 CONTINUE
C
C          2.7  Close MAGICS
C
      CALL PCLOSE
      go to 320
c
c   -------------------------------------------------------------------
c
c                 3.  Errors and Exit
c                ---------------------
c
  300 continue
c
c          3.1  error conditions
c
  312 continue
      write (*,'("  END OF FILE ",a)') yfile
      call abort
  314 continue
      write (*,'("  pbgrib error ",i5," on file ",a)') iret,yfile
      call abort
  320 continue
      STOP
      END 



#include "parse_command_line.h"

