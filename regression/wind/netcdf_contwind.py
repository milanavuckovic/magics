from Magics.macro import *
#setting the output
output = output(   
                output_formats = ['png'],
                output_name = "xsect_step6",
                output_name_first_page_number = "off"
        )
# Setting the cartesian view
projection = mmap(
       
        )
        
coastlines = mcoast()

tempe = mnetcdf(netcdf_filename = "fc_surf.nc",
      netcdf_value_variable = "v2t",
      netcdf_type = "geomatrix",

    )
contour = mcont()
wind_data = mnetcdf(netcdf_filename = "fc_surf.nc",
		netcdf_type = "geomatrix",
		netcdf_x_component_variable = "v10u",
		netcdf_y_component_variable = "v10v"
		)

wind = mwind() 

title = mtext(
        text_lines= ['Example of NetcdfData contour and Wind...',
        '<magics_title/>'],
        text_html= 'true',
        text_justification= 'left',
        text_font_size= 0.8,
        text_colour= 'charcoal',
        )
 
plot(output, projection, coastlines, tempe, contour, wind_data, wind, title)
