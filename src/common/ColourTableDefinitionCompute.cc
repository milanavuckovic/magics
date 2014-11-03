/******************************** LICENSE ********************************

 Copyright 2007 European Centre for Medium-Range Weather Forecasts (ECMWF)

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at 

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 ******************************** LICENSE ********************************/

/*! \file ColourTableDefinitionCompute.cc
    \brief Implementation of the Template class ColourTableDefinitionCompute.
    
    Magics Team - ECMWF 2005
    
    Started: Mon 4-Jul-2005
    
    Changes:
    
*/



#include "ColourTableDefinitionCompute.h"
#include "XmlNode.h"

using namespace magics;

ColourTableDefinitionCompute::ColourTableDefinitionCompute() 
{
}


ColourTableDefinitionCompute::~ColourTableDefinitionCompute() 
{
}

/*!
 Class information are given to the output-stream.
*/		
void ColourTableDefinitionCompute::print(ostream& out)  const
{
	out << "ColourTableDefinitionCompute[";
	out << "]";
}

void ColourTableDefinitionCompute::set(const ColourTableDefinitionComputeInterface& attributes)
{
	minColour_ = attributes.getMin();
	maxColour_ = attributes.getMax();
	direction_ = attributes.getDirection();
}

void ColourTableDefinitionCompute::set(const XmlNode& node)
{
	direction_ = node.getAttribute("direction");
	MagLog::warning() << "ColourTableDefinitionCompute::set(const XmlNode&): to be implemented\n";
	
	for (XmlNode::ElementIterator elt = node.firstElement(); elt != node.lastElement(); ++elt) {
		if ( magCompare((*elt)->name(), "min_colour") ) {
			minColour_ = Colour((*elt)->data());
		}
		if ( magCompare((*elt)->name(), "max_colour") ) {
			maxColour_ = Colour((*elt)->data());
		}
		
	}
	
	
}

void ColourTableDefinitionCompute::set(ColourTable& table, int nb)
{
	prepare();
	double step_hue;
	double step_light;
	double step_alpha;
	MagLog::dev() << "ColourTableDefinitionCompute::set->min-->" << minColour_ << endl;
	MagLog::dev() << "ColourTableDefinitionCompute::set->max-->" << maxColour_ << endl;
	MagLog::dev() << "nb interval-->" << nb << endl;
    Hsl hmin = minColour_.hsl()
    Hsl hmax = maxColour_.hsl();
    if (nb == 1) {
       	table.push_back(minColour_);
       	return;
       }
    if (nb == 2) {
    	table.push_back(minColour_);
    	return;
    }
    if (nb == 3) {
       	table.push_back(minColour_);
       	table.push_back(maxColour_);
       	return;
       }
    


    	step_light = (hmax.light_ - hmin.light_)/(nb-2);
    	step_alpha = (hmax.alpha_ - hmin.alpha_)/(nb-2);
        if ( magCompare(direction_, "anti_clockwise") ) {
           if ( hmax.hue_ < hmin.hue_ )  hmax.hue_ += 360;
           step_hue = (hmax.hue_ - hmin.hue_)/(nb-2);
        } 
        else {
            if ( hmin.hue_ < hmax.hue_ )  hmin.hue_ += 360;
            step_hue =  (hmax.hue_ - hmin.hue_)/(nb-2);
        }

    
    float step_sat =  (hmax.saturation_ - hmin.saturation_)/(nb-2);
    // WE have nb levels : we need nb-1 colours! 
   
    for ( int i = 0;  i < nb-1; i++) {
    	MagLog::dev() << "ColourTableDefinitionCompute::set->add-->" << Colour(hmin) << endl;
       table.push_back(Colour(hmin));
       hmin.saturation_ += step_sat;
       hmin.hue_ += step_hue;
       hmin.light_ += step_light;
       hmin.alpha_ += step_alpha;
    }
}


