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


/*! 
    \brief Implementation of the Template class SpotDecoder.
    
    Magics Team - ECMWF 2005
    
    Started: Mon 19-Sep-2005
    
    Changes:
    
*/
 

#include <locale>
#include "WrepJSon.h"
#include "MetaData.h"
#include "CustomisedPoint.h"
#include "DateTime.h"
#include "TextVisitor.h"
#include "MagException.h"
#include "json_spirit.h"
#include "IntervalMap.h"
#include "EfiLegendEntry.h"

using namespace magics;
using namespace json_spirit;

static map<char, string> specials;

WrepJSon::WrepJSon()  : missing_(-9999),
		height_(-9999),
		mask_(9999),
		station_latitude_(9999),
		station_longitude_(9999), 
		latitude_(0), longitude_(0)
{
	methods_["date"] = &WrepJSon::date;
	methods_["time"] = &WrepJSon::time;
	methods_["eps_height"] = &WrepJSon::epsz;
	methods_["height"] = &WrepJSon::height;
	methods_["deterministic_height"] = &WrepJSon::detz;
	methods_["tracker"] = &WrepJSon::ignore;
	methods_["missing"] = &WrepJSon::missing;
	methods_["location"] = &WrepJSon::location;
	methods_["station_name"] = &WrepJSon::station_name;
	methods_["requested_location"] = &WrepJSon::station;
	methods_["land_sea_mask"] = &WrepJSon::mask;
	methods_["metadata"] = &WrepJSon::metadata;
	methods_["points_along_meridian"] = &WrepJSon::points_along_meridian;
	
	methods_["x_values"] = &WrepJSon::x_values;
	methods_["x_date_values"] = &WrepJSon::x_date_values;
	methods_["y_values"] = &WrepJSon::y_values;
	methods_["y_date_values"] = &WrepJSon::y_date_values;
	methods_["values"] = &WrepJSon::values;

	decoders_["eps"] = &WrepJSon::eps;
	decoders_["profile"] = &WrepJSon::profile;
	decoders_["efi"] = &WrepJSon::efi;
	decoders_["cdf"] = &WrepJSon::cdf;
	decoders_["basic"] = &WrepJSon::basic;
	decoders_["data"] = &WrepJSon::data;
	
	transformationHandlers_["eps"] = &WrepJSon::eps;
	transformationHandlers_["cdf"] = &WrepJSon::cdf;
	transformationHandlers_["efi"] = &WrepJSon::efi;
	transformationHandlers_["profile"] = &WrepJSon::profile;

    heightCorrections_["forecast"] = &WrepJSon::correctDetz;
    
    metaMethods_["temperature_correction"] = &WrepJSon::temperature_correction;
    metaMethods_["temperature_adjustment"] = &WrepJSon::temperature_adjustment;
    metaMethods_["eps_resolution"] = &WrepJSon::eps_resolution;
    metaMethods_["deterministic_resolution"] = &WrepJSon::deterministic_resolution;
    metaMethods_["station_name"] = &WrepJSon::station_name;
    metaMethods_["height"] = &WrepJSon::height;

	minx_ = std::numeric_limits<double>::max();
	maxx_ = -std::numeric_limits<double>::max();
	miny_ = std::numeric_limits<double>::max();
	maxy_ = -std::numeric_limits<double>::max();
	xdate_ = false;
	ydate_ = false;

    if ( specials.empty() ) {
        specials['\242'] = "&cent;"; 
        specials['\243'] = "&pound;"; 
        specials['\244'] = "&euro;"; 
        specials['\245'] = "&yen;"; 
        specials['\260'] = "&deg;"; 
        specials['\274'] = "&frac14;"; 
        specials['\274'] = "&OElig;"; 
        specials['\275'] = "&frac12;"; 
        specials['\275'] = "&oelig;"; 
        specials['\276'] = "&frac34;"; 
        specials['\276'] = "&Yuml;"; 
        specials['\241'] = "&iexcl;"; 
        specials['\253'] = "&laquo;"; 
        specials['\273'] = "&raquo;"; 
        specials['\277'] = "&iquest;"; 
        specials['\300'] = "&Agrave;"; 
        specials['\301'] = "&Aacute;"; 
        specials['\302'] = "&Acirc;"; 
        specials['\303'] = "&Atilde;"; 
        specials['\304'] = "&Auml;"; 
        specials['\305'] = "&Aring;"; 
        specials['\306'] = "&AElig;"; 
        specials['\307'] = "&Ccedil;"; 
        specials['\310'] = "&Egrave;"; 
        specials['\311'] = "&Eacute;"; 
        specials['\312'] = "&Ecirc;"; 
        specials['\313'] = "&Euml;"; 
        specials['\314'] = "&Igrave;"; 
        specials['\315'] = "&Iacute;"; 
        specials['\316'] = "&Icirc;"; 
        specials['\317'] = "&Iuml;"; 
        specials['\320'] = "&ETH;"; 
        specials['\321'] = "&Ntilde;"; 
        specials['\322'] = "&Ograve;"; 
        specials['\323'] = "&Oacute;"; 
        specials['\324'] = "&Ocirc;"; 
        specials['\325'] = "&Otilde;"; 
        specials['\326'] = "&Ouml;"; 
        specials['\330'] = "&Oslash;"; 
        specials['\331'] = "&Ugrave;"; 
        specials['\332'] = "&Uacute;"; 
        specials['\333'] = "&Ucirc;"; 
        specials['\334'] = "&Uuml;"; 
        specials['\335'] = "&Yacute;"; 
        specials['\336'] = "&THORN;"; 
        specials['\337'] = "&szlig;"; 
        specials['\340'] = "&agrave;"; 
        specials['\341'] = "&aacute;"; 
        specials['\342'] = "&acirc;"; 
        specials['\343'] = "&atilde;"; 
        specials['\344'] = "&auml;"; 
        specials['\345'] = "&aring;"; 
        specials['\346'] = "&aelig;"; 
        specials['\347'] = "&ccedil;"; 
        specials['\350'] = "&egrave;"; 
        specials['\351'] = "&eacute;"; 
        specials['\352'] = "&ecirc;"; 
        specials['\353'] = "&euml;"; 
        specials['\354'] = "&igrave;"; 
        specials['\355'] = "&iacute;"; 
        specials['\356'] = "&icirc;"; 
        specials['\357'] = "&iuml;"; 
        specials['\360'] = "&eth;"; 
        specials['\361'] = "&ntilde;"; 
        specials['\362'] = "&ograve;"; 
        specials['\363'] = "&oacute;"; 
        specials['\364'] = "&ocirc;"; 
        specials['\365'] = "&otilde;"; 
        specials['\366'] = "&ouml;"; 
        specials['\370'] = "&oslash;"; 
        specials['\371'] = "&ugrave;"; 
        specials['\372'] = "&uacute;"; 
        specials['\373'] = "&ucirc;"; 
        specials['\374'] = "&uuml;"; 
        specials['\375'] = "&yacute;"; 
        specials['\376'] = "&thorn;"; 
      }

}

WrepJSon::~WrepJSon()
{
	
}
void WrepJSon::visit(Transformation& transformation)
{

	regular_ = (transformation.xAxisType() == "regular");

	missing_ = missing_value_;

	decode();

	double add = (maxy_ - miny_ ) * y_percent_ /100.;
	maxy_ += add;
	miny_ -= add;





	if ( y_max_threshold_ < INT_MAX) 
		maxy_ = (maxy_ < y_max_threshold_ )? y_max_threshold_ : maxy_; 
 

	if ( miny_ == maxy_) {
		if ( miny_ == missing_ )  {
			miny_ = 0; 
			maxy_ = 0; 
		}
		maxy_++;
	}
	map<string,  TransformationHandler>::iterator handler = transformationHandlers_.find(family_);

	if ( handler !=  transformationHandlers_.end() ) {
			       (this->*handler->second)(transformation);
	}  		
	else {
		if ( xdate_ ) {

				transformation.setDataMinMaxX(minx_, maxx_, xBase_);


		}
		else {
			transformation.setDataMinMaxX(minx_, maxx_);

		}
		if ( ydate_ ) {
			transformation.setDataMinMaxY(miny_, maxy_, yBase_);

		}
		else {
			transformation.setDataMinMaxY(miny_, maxy_);

		}

	}
}

void WrepJSon::profile(Transformation& transformation)
{

	transformation.setDataMinMaxX(minx_, maxx_);


	transformation.setDataMinMaxY(miny_, maxy_);

}
void WrepJSon::eps(Transformation& transformation)
{
	if ( keyword_ != "clim" ) {
		transformation.setDataMinMaxX(minx_  * 3600, maxx_ * 3600, base_);

	}
	vector<double> maxs;
	vector<double> allvalues;
		
		
	for (vector<CustomisedPoint*>::iterator point = points_.begin(); point != points_.end(); ++point) {
	    	maxs.push_back((**point)["max"]);
	    	allvalues.push_back((**point)["seventyfive"]);
	    	allvalues.push_back((**point)["ninty"]);
	    	if ( (*point)->find("forecast") != (*point)->end() ) {
	    		allvalues.push_back((**point)["forecast"]);
	    		maxs.push_back((**point)["forecast"]);
	    	}
	    	if ( (*point)->find("control") != (*point)->end() ) {
	    		allvalues.push_back((**point)["control"]);  
	    		maxs.push_back((**point)["control"]);
	    	}

	    }
	

	
	if ( threshold_ < 30) {
	    	double limit = *std::max_element(allvalues.begin(), allvalues.end());
	    	
	    	std::sort(maxs.begin(), maxs.end());
	    	double mm = maxs.back();
	    
	    	double p1 = 1;
	    	double p2;
	    	int i = maxs.size() -1 ;
	    	int n = maxs.size();
	    	for ( vector<double>::reverse_iterator val = maxs.rbegin(); val != maxs.rend(); ++val) {
	    		p2 = (*val)/maxs.back();
	    	
	    		if ( p2 < (1 - i/n)*percentile_ && (p1-p2) > threshold_/n && *val >= limit ) {
	    			mm = *val;  			
	    		}
	   			p1 = p2;    
				i--;
	    	}
	    
	    	maxy_ = std::max(mm, 1.);
	    }
	    
	
        miny_ = std::min(correctDetz(miny_), correctEpsz(miny_));
        maxy_ = std::max(correctDetz(maxy_), correctEpsz(maxy_));

	if ( same(miny_, maxy_) ) 
        	maxy_ = miny_ +5.;
	transformation.setDataMinMaxY(miny_, maxy_);

}
void WrepJSon::cdf(Transformation& transformation)
{
	transformation.setDataMinMaxX(minx_, maxx_);
	transformation.setDataMinMaxY(0, 100);

}
void WrepJSon::efi(Transformation& transformation)
{
	transformation.setDataMinMaxX(-100,100);
	transformation.setDataMinMaxY(miny_, maxy_);
}
void WrepJSon::decode()
{	
	  map<string,  Decoder>::iterator decoder = decoders_.find(family_);

	  if ( decoder != decoders_.end() ) {
		       (this->*decoder->second)();
		        		    	    }  		
	  else 
		  basic();
	 
}
void WrepJSon::eps()
{
	if ( !points_.empty()) return;
	
	shift_ = 12;
	methods_[param_] = &WrepJSon::parameter;
	methods_[keyword_] = &WrepJSon::dig;
	     
    scaling_factor_ = param_scaling_factor_;
    offset_factor_ = param_offset_factor_;    
    file_ = path_;
    current_ = &values_;
    
    basic();

    
   
    base_ = DateTime(date_, time_);
   
    if ( values_.steps_.empty() ) {
    	 MagLog::error() << "Could not find data for parameter: " << param_ << endl;
    	 abort();
    } 
    minx_ = 0;
    maxx_ = values_.steps_.back()+shift_;
    if ( regular_ ) {
    	 minx_ = 0;
    	 maxx_= values_.steps_.back()/(24*3600.);
    }
    vector<double> yval;
    for (unsigned int i = 0; i < values_.steps_.size(); i++) {
	map<string, vector<double> >::iterator values = values_.values_.find("1");
	double value = (values ==  (values_.values_.end() ) ) ? 0 : values->second[i]; 
	yval.push_back(value);
    }
    double yy =  *std::min_element(yval.begin(), yval.end());
    MagLog::dev() << "minx= " <<  minx_ << "->maxx= " << maxx_ << endl;
    map<string, vector<double> >::iterator intensity = values_.values_.find("intensity");
    map<string, vector<double> >::iterator direction = values_.values_.find("direction");
    map<string, vector<double> >::iterator val = values_.values_.find("1");
	for (unsigned int i = 0; i < values_.steps_.size(); i++) {
		
		double value = (val ==  (values_.values_.end() ) ) ? 0 : val->second[i]; 
		double speed = (intensity ==  (values_.values_.end() ) ) ? 0 : intensity->second[i]; 
		double dir = (direction ==  (values_.values_.end() ) ) ? 0 : direction->second[i]; 
		CustomisedPoint* point = new CustomisedPoint();		
		point->longitude(values_.steps_[i] * 3600);		 
		point->latitude(values_.steps_[i] * 3600);		 
		(*point)["step"]    = values_.steps_[i] * 3600;

		(*point)["x"]    = values_.steps_[i] * 3600;
		if ( regular_ ) {
			(*point)["step"]    = values_.steps_[i]/24.;

				(*point)["x"]    = values_.steps_[i]/24.;
		}
		(*point)["x_upper"]    = values_.steps_[i] * 3600;
		(*point)["y_upper"]    = value;
		(*point)["y_lower"]    = yy;
		(*point)["y"]    = value;
		(*point)["shift"] = 0;
		(*point)["width"]    = 1 * 3600;
		(*point)["intensity"]    = speed;
		(*point)["direction"]    = dir;
		(*point)["missing"]    = missing_;
        (*point)["latitude"]    = latitude_;
        (*point)["longitude"]    = longitude_;
	
		point->base(base_);
		map<string, vector<double>  >& values = values_.values_;
		
		for ( map<string, vector<double>  >::iterator val = values.begin(); val != values.end(); ++val ) {
            if ( val->first=="forecast" )  
                 (*point)[val->first] = correctDetz((val->second)[i]);
            else 
			    (*point)[val->first] = correctEpsz((val->second)[i]);
        }
	
		points_.push_back(point);
	}
	
}
void WrepJSon::profile()
{
	if ( !points_.empty()) return;

	methods_[param_] = &WrepJSon::parameter;
	methods_[keyword_] = &WrepJSon::dig;

    scaling_factor_ = param_scaling_factor_;
    offset_factor_ = param_offset_factor_;
    file_ = path_;
    current_ = &values_;

    basic();





    if ( values_.levels_.empty() ) {
    	 MagLog::error() << "Could not find data for parameter: " << param_ << endl;
    	 abort();
    }
    miny_ = values_.levels_.front();
    maxy_ = values_.levels_.back();

    MagLog::dev() << "minx= " <<  minx_ << "->maxx= " << maxx_ << endl;
    map<string, vector<double> >::iterator intensity = values_.values_.find("intensity");
    map<string, vector<double> >::iterator direction = values_.values_.find("direction");
    map<string, vector<double> >::iterator val = values_.values_.find("1");
    vector<double> minmax;

	for (unsigned int i = 0; i < values_.levels_.size(); i++) {

		double value = (val ==  (values_.values_.end() ) ) ? 0 : val->second[i];
		double speed = (intensity ==  (values_.values_.end() ) ) ? 0 : intensity->second[i];
		double dir = (direction ==  (values_.values_.end() ) ) ? 0 : direction->second[i];
		CustomisedPoint* point = new CustomisedPoint();
		point->longitude(value);
		point->latitude(value);
		(*point)["step"]    =value;
		(*point)["x"]    = value;
		(*point)["x_upper"]    = value;
		(*point)["y_upper"]    = value;
		(*point)["y_lower"]    = 1000;
		(*point)["y"]    = values_.levels_[i];
		(*point)["shift"] = 0;
		(*point)["width"]    = 1 ;
		(*point)["intensity"]    = speed;
		(*point)["direction"]    = dir;
		(*point)["missing"]    = missing_;
        (*point)["latitude"]    = latitude_;
        (*point)["longitude"]    = longitude_;

		point->base(base_);
		map<string, vector<double>  >& values = values_.values_;


		for ( map<string, vector<double>  >::iterator val = values.begin(); val != values.end(); ++val ) {
			if ( val->first=="forecast" ) {
				double value =  (val->second)[i] == missing_ ? missing_ : correctDetz((val->second)[i]);

				(*point)[val->first] = value;
				if (value != missing_)
					minmax.push_back(value);

			}
			else {
				double value = (val->second)[i] == missing_ ? missing_ : correctEpsz((val->second)[i]);
				(*point)[val->first] = value;
				if (value != missing_)
					minmax.push_back(value);

			}

		}
		// Test for thermo
		if ( (*point)["y"] > 100. )
			points_.push_back(point);
	}

	minx_ = *std::min_element(minmax.begin(), minmax.end());
    maxx_ = *std::max_element(minmax.begin(), minmax.end());



}
void WrepJSon::customisedPoints(const Transformation& transformation, const std::set<string>& needs, CustomisedPointsList& out)
{

	decode();

	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point) {
		// Here we need to check we have date!
		if ( xdate_ ) {
			DateTime ref(transformation.getReferenceX());

			double shift = ref - xBase_;
			double x = (**point)["x"] - shift;
			(**point)["x"] = x;
		}
		(**point)["resolution"] = points_along_meridian_;
		out.push_back(*point);
	}
	points_.clear();

}
void WrepJSon::customisedPoints(const std::set<string>&, CustomisedPointsList& out)
{
	MagLog::dev() << "WrepJSon::customisedPoints" << std::endl;
	
	decode();

	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point) {
		(**point)["resolution"] = points_along_meridian_;
		out.push_back(*point);
			

	}
}
void WrepJSon::data()
{
	if ( points_.empty() ) {
		methods_[keyword_] = &WrepJSon::dig;
		file_ = path_;

		basic();
	}
}

void WrepJSon::basic()
{
	  ifstream is( file_.c_str());
	    	 
	     json_spirit::Value value;
	         try {
	        	  json_spirit::read_or_throw( is, value );
	        	  Object object = value.get_value< Object >();
	        		        
	        	  for (vector<Pair>::const_iterator entry = object.begin(); entry !=  object.end(); ++entry) {
	        		  map<string,  Method >::iterator method = methods_.find(entry->name_);
	        		    	    if ( method != methods_.end() ) {
	        		    	    	   ( (this->*method->second)(entry->value_) );
	        		    	    }  		
	        			    		
	        	  }
	         }
	         catch (std::exception e)
	         {
	        	 MagLog::error() << "Could not processed the file: " << file_ << ": " << e.what() << endl;
	        	 abort();
	         }	
}
void WrepJSon::print(ostream& out) const
{
	out << "WrepJSon[";
	out << "]";
}

void WrepJSon::metadata(const json_spirit::Value& value )
{
	metadata_ = value;
	dig(value);
}

void WrepJSon::station(const json_spirit::Value& value )
{
	ASSERT( value.type() == obj_type );
	Object location = value.get_value<Object>();
	const json_spirit::Value lat = find_value(location, "lat");
	const json_spirit::Value lon = find_value(location, "lon");
	station_latitude_=lat.get_value<double>();
	station_longitude_=lon.get_value<double>();

	MagLog::dev() << "found -> station_lat= " << station_latitude_ << endl;
	MagLog::dev() << "found -> station_lon= " << station_longitude_ << endl;

}

void WrepJSon::location(const json_spirit::Value& value )
{
	
	ASSERT( value.type() == obj_type );
	Object location = value.get_value<Object>();
	const json_spirit::Value lat = find_value(location, "lat");
	const json_spirit::Value lon = find_value(location, "lon");
	latitude_=lat.get_value<double>();
	longitude_=lon.get_value<double>();
	MagLog::dev() << "found -> lat= " << latitude_ << endl;
	MagLog::dev() << "found -> lon= " << longitude_ << endl;
	
}
void WrepJSon::station_name(const json_spirit::Value& value )
{
    string station;
	if  ( value.type() == str_type )
		station = value.get_value<string>();


    for ( string::iterator c = station.begin(); c != station.end(); ++c) {
        map<char, string>::iterator s = specials.find(*c);
        if ( s != specials.end()  ) {
            station_name_ += s->second;
        }
        else 
           station_name_ += *c;
    }
}
void WrepJSon::epsz(const json_spirit::Value& value)
{
	ASSERT( value.type() == real_type);
	epsz_ = value.get_value< double>();
	MagLog::dev() << "found -> epsz= " <<  epsz_ << endl;
}
void WrepJSon::height(const json_spirit::Value& value)
{
    if ( value.type() == real_type) {
     height_ = value.get_value< double>();
    }
    if ( value.type() == int_type) {
       height_ = value.get_value< int>(); 
    } 
	MagLog::dev() << "found -> height= " <<  height_ << endl;
}

void WrepJSon::detz(const json_spirit::Value& value )
{
	ASSERT( value.type() == real_type);
	detz_ = value.get_value< double>();
	MagLog::dev() << "found -> detz= " <<  detz_ << endl;
}
void WrepJSon::mask(const json_spirit::Value& value )
{
	ASSERT( value.type() == real_type);
	mask_ = value.get_value< double>();
	MagLog::dev() << "found -> mask= " <<  mask_ << endl;
}
void WrepJSon::missing(const json_spirit::Value& value )
{
	ASSERT( value.type() == str_type);
	MagLog::dev() << "found -> missing= " <<  value.get_value<string>() << endl;
	missing_ = tonumber(value.get_value<string>());
}
void WrepJSon::date(const json_spirit::Value& value)
{

	ASSERT( value.type() == str_type);
	MagLog::dev() << "found -> date= " <<  value.get_value<string>() << endl;
	date_ =  value.get_value<string>();
	
}
void WrepJSon::time(const json_spirit::Value& value)
{

	ASSERT( value.type() == str_type);
	MagLog::dev() << "found -> time= " <<  value.get_value<string>() << endl;
	time_ =  value.get_value<string>();
}

void WrepJSon::parameter(const json_spirit::Value& value)
{
	ASSERT( value.type() == obj_type );
	Object param = value.get_value< Object >();
	        		        
	for (vector<Pair>::const_iterator info = param.begin(); info !=  param.end(); ++info) {
				ASSERT (info->value_.type() == array_type);
	        	Array values = info->value_.get_value<Array>();
	        	if ( info->name_ == "steps" ) {
	        		for (unsigned int i = 0; i < values.size(); i++) {
	        				current_->steps_.push_back( tonumber(values[i].get_value<string>()));
	        			  
	        	     }
	        	}
	        	else if ( info->name_ == "levels" ) {
	        		for (unsigned int i = 0; i < values.size(); i++) {
	        				current_->levels_.push_back( tonumber(values[i].get_value<string>()));

	        	     }
	        	}
	        	else {
	        		bool add = true;
	        		for ( vector<string>::iterator i = ignore_keys_.begin(); i != ignore_keys_.end(); ++i)
	        			if ( *i == info->name_ )
	        				add = false;
	        		if ( !add ) {
	        			cout << " ignore -->" << info->name_ << endl;
	        			continue;
	        		}
	        		map<string, vector<double> >& xv = current_->values_;
	        		xv.insert(make_pair(info->name_, vector<double>()));
	        		vector<double>& vals =  xv[info->name_];
				
	        		for (unsigned int i = 0; i < values.size(); i++) {

	        			double val = values[i].get_value<double>();
	        			if ( same(val, 0 ))  {
                            val = 0;
                        }
	        			if ( val != missing_ ) {
	        				val = (val * scaling_factor_) + offset_factor_;
	        				if ( val < miny_) miny_ = val;
	        				if ( val > maxy_) maxy_ = val;
	        			}
	        			vals.push_back(val);
	        			
	        			
	        			
	        		}
	        	}
	}
	
}
void WrepJSon::dig(const json_spirit::Value& value)
{
	ASSERT( value.type() == obj_type );
	Object object = value.get_value< Object >();
	  for (vector<Pair>::const_iterator entry = object.begin(); entry !=  object.end(); ++entry) {
      		  map<string,  Method >::iterator method = methods_.find(entry->name_);
      		    	    if ( method != methods_.end() ) {
      		    	    	   ( (this->*method->second)(entry->value_) );
      		    	    }  		
      			    		
      	  }       		        
	
}

MatrixHandler& WrepJSon::matrix()
{
	decode();
	vector<double> values;
	vector<double> steps;
	// Find the min and the max ..
	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point)
	{


		 for ( int s = 0; s != 50; s++) {		    
				string key = tostring(s);
			    map<string, double>::const_iterator member = (*point)->find(key);
				if ( member != (*point)->end() ) 
					values.push_back(member->second );
		 }

		 steps.push_back((**point)["step"]);

	}


	double from = maground(*min_element(values.begin(), values.end()));
	double to = maground(*max_element(values.begin(), values.end()));

	IntervalMap<int> array;

	double plume = plumes_/2;

	for ( double a = from - plume; a <= to + plume; a = a + plume ) {
			array.insert(make_pair(Interval(a-plume, a+plume), 0));
	}


	for (IntervalMap<int>::iterator interval = array.begin(); interval!= array.end(); ++interval) {
		double row = (interval->first.max_ - interval->first.min_)/2 +interval->first.min_ ;
		matrix_.rowsAxis().push_back(row);
	}

	matrix_.columnsAxis().push_back(steps.front());
	vector<double>::iterator s = steps.begin();
	s++;
	for (; s != steps.end(); ++s) {

		matrix_.columnsAxis().push_back(*s);
	}

	matrix_.setMapsAxis();
	matrix_.resize(matrix_.columnsAxis().size() * matrix_.rowsAxis().size(), 0);
	int column = 0;

	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point)
	{
		for (IntervalMap<int>::iterator interval = array.begin(); interval!= array.end(); ++interval) {
			interval->second = 0;
		}
		for ( int s = 0; s != 50; s++) {		    
			string key = tostring(s);

			map<string, double>::const_iterator step = (*point)->find(key);
			if (step != (*point)->end() ) {
				for (IntervalMap<int>::iterator interval = array.begin(); interval!= array.end(); ++interval) {
						if ( interval->first.between(step->second) )
								interval->second++;
				}
			}
		}

		int row = 0;
		
		for (IntervalMap<int>::iterator interval = array.begin(); interval!= array.end(); ++interval) {
			matrix_[column+row*matrix_.columnsAxis().size()] = interval->second*2; // in percentage!
			row++;
		}
		++column;

	}

	matrix_.missing(std::numeric_limits<double>::max());
	matrixHandlers_.push_back(new  MatrixHandler(matrix_));
	return *(matrixHandlers_.back());
}

void WrepJSon::cdf() 
{
	if ( !clim_.empty() ) return;
	
	
	 file_ = path_;
	 methods_["clim"] =  &WrepJSon::clim;
	 methods_["eps"] = &WrepJSon::eps;
	 methods_["efi"] = &WrepJSon::efi;
	 methods_[param_] =  &WrepJSon::parameter;
	 scaling_factor_ = param_scaling_factor_;
	 	offset_factor_ = param_offset_factor_;
	 
	    
	basic();
	// setminmax ...
	minx_ = std::numeric_limits<double>::max();
		maxx_ = -std::numeric_limits<double>::max();
		for (map<string,  InputWrep>::iterator info = eps_.begin(); info !=  eps_.end(); ++info) {
							
							// find the index in the steps..
							InputWrep& data = info->second;
							int index = data.index(tonumber(info->first));
							map<string, vector<double>  >& values = data.values_;					
						
							for (map<string,  vector<double> >::iterator val = values.begin(); val !=  values.end(); ++val) {
								double value =  correctEpsz(val->second[index]);

								if ( minx_ > value ) minx_ = value;
								if ( maxx_ < value ) maxx_ = value;
									
							}
							
			}
			int index = clim_.index(36);
			for (map<string,  vector<double> >::iterator val = clim_.values_.begin(); val !=  clim_.values_.end(); ++val) {	
				double value =  correctEpsz(val->second[index]);
									if ( minx_ > value ) minx_ = value;
									if ( maxx_ < value ) maxx_ = value;
			}
			

			int steps = 1;
			CustomisedPoint* point = new CustomisedPoint();		
			points_.push_back(point);
			(*point)["resolution"] = points_along_meridian_;
			
			for ( vector<int>::iterator step = steps_.begin(); step != steps_.end(); ++step) {
				map<string,  InputWrep>::iterator info = eps_.find(tostring(*step));
				if ( info != eps_.end() ) {
						
							// find the index in the steps..
							InputWrep& data = info->second;
							int s = tonumber(info->first);
							int index = data.index(tonumber(info->first));
							map<string, vector<double>  >& values = data.values_;		
							ostringstream key;			
							key << steps << "_step";
							(*point)[key.str()] = s +12;
							for (map<string,  vector<double> >::iterator val = values.begin(); val !=  values.end(); ++val) {			
									if (isdigit(val->first[0]) == false ) continue;
									ostringstream key;
									int i = 2*tonumber(val->first);
									key << steps << "_" << i;
									(*point)[key.str()] = correctEpsz(val->second[index]);
							}
							steps++;
				}
			}
			

			
			index = clim_.index(36);
			for (map<string,  vector<double> >::iterator val = clim_.values_.begin(); val !=  clim_.values_.end(); ++val) {		
									ostringstream key;									
									key << "clim_" << val->first;
									(*point)[key.str()] = correctEpsz(val->second[index]);
			}
}
void WrepJSon::efi()
{
	if ( !efi_.empty() ) return;
	
	
	 file_ = path_;
	 methods_["clim"] =  &WrepJSon::ignore;
	 methods_["eps"] = &WrepJSon::ignore;
	 methods_["efi"] = &WrepJSon::efi;
	 methods_[param_] =  &WrepJSon::parameter;

	basic();
	CustomisedPoint* point = new CustomisedPoint();		
				points_.push_back(point);

			int steps = 1;
			

			(*point)["steps"] = steps_.size();
			
			miny_ = steps_.size()+1;
			maxy_ =  1;
			
			for ( vector<int>::iterator step = steps_.begin(); step != steps_.end(); ++step) {
					int x = *step +12;
					map<string,  InputWrep>::iterator info = efi_.find(tostring(x));
					if ( info != efi_.end() ) {
								
								// find the index in the steps..
								InputWrep& data = info->second;
								int index = data.index(tonumber(info->first));
								map<string, vector<double>  >& values = data.values_;		
								ostringstream key;			
								key << "efi" << steps << "_step";
								(*point)[key.str()] = (tonumber(info->first)) +12;
								for (map<string,  vector<double> >::iterator val = values.begin(); val !=  values.end(); ++val) {			
										ostringstream key;
										key << "efi" << steps << "_value";  
										(*point)[key.str()] = val->second[index];		
										
								}
								steps++;
					}
				}
			
}



void WrepJSon::clim(const json_spirit::Value& value)
{
	
	current_ = &clim_;
	scaling_factor_ = param_scaling_factor_;
	offset_factor_ = param_offset_factor_;


	dig(value);

	clim_.print();



}
void WrepJSon::efi(const json_spirit::Value& value)
{
	
	ASSERT( value.type() == obj_type );
	Object param = value.get_value< Object >();
	scaling_factor_ = 100;
	offset_factor_ = 0;
	        		        
	for (vector<Pair>::const_iterator info = param.begin(); info !=  param.end(); ++info) {				
	        	
	        	efi_.insert(make_pair(info->name_, InputWrep()));
	        	current_ = &(efi_[info->name_]);
	        	dig(info->value_);   
	  }
	        
	for (map<string,  InputWrep>::iterator info = efi_.begin(); info !=  efi_.end(); ++info) {
		
		info->second.print();
	}
	        
	
}

void WrepJSon::eps(const json_spirit::Value& value)
{
	
	scaling_factor_ = param_scaling_factor_;
		offset_factor_ = param_offset_factor_;
		ASSERT( value.type() == obj_type );
		Object param = value.get_value< Object >();
		        		        
		for (vector<Pair>::const_iterator info = param.begin(); info !=  param.end(); ++info) {				
			        	
			        	eps_.insert(make_pair(info->name_, InputWrep()));
			        	current_ = &(eps_[info->name_]);
			        	dig(info->value_);   
			  }
			        
			for (map<string,  InputWrep>::iterator info = eps_.begin(); info !=  eps_.end(); ++info) {
				
				info->second.print();
			}
}
	 
double WrepJSon::correctDetz(double value)
{

    if (correction_ && height_ != -9999) {
        double correction = ( height_ - detz_ ) * 0.0065;
        
        return value - correction;
    }
    
    return value;
  
}

double WrepJSon::correctEpsz(double value)
{
    if (correction_ && height_ != -9999) {
        double correction = ( height_ - epsz_ ) * 0.0065;
        
        return value - correction;
    }
    
    return value;
   
}

void WrepJSon::visit(MetaDataVisitor& visitor)
{


	if (metadata_.type() == null_type) return;


	// we visit the metadata to update it ..
	Object object = metadata_.get_value< Object >();
	for (vector<Pair>::iterator entry = object.begin(); entry !=  object.end(); ++entry) {

		map<string,  MetaMethod >::iterator method = metaMethods_.find(entry->name_);
		if ( method != metaMethods_.end() ) {
			entry->value_ = ( (this->*method->second)() );
		}
	}

	ostringstream json;
	json_spirit::write(object, json);

	visitor.add("metadata", json.str());



}

Value WrepJSon::temperature_correction()
{
	bool correction = (correction_ && height_ != -9999 );
	Value value(correction);
	return value;
}
Value  WrepJSon::temperature_adjustment()
{
	Value value;
	if (correction_ && height_ != -9999) {
		Object correction;;
		correction.push_back(Pair( "deterministic_adjustement", -( height_ - detz_ ) * 0.0065  ) );
		correction.push_back(Pair( "eps_adjustement", -( height_ - epsz_ ) * 0.0065  ) );
		value = correction;
	}
	return value;

}

void WrepJSon::points_along_meridian(const json_spirit::Value& value)
{
	ASSERT( value.type() == int_type);
	points_along_meridian_ = value.get_value< int>();
	MagLog::dev() << "found -> points_along_meridian_mask= " <<  mask_ << endl;
}

void WrepJSon::x_values(const json_spirit::Value& value)
{
	ASSERT (value.type() == array_type);
	Array values = value.get_value<Array>();
	bool newpoint = points_.empty();
	vector<double> minmax;

	for (unsigned int i = 0; i < values.size(); i++) {
		if ( newpoint ) {
			CustomisedPoint* point = new CustomisedPoint();
			(*point)["resolution"] = points_along_meridian_;
			points_.push_back(point);
		}
		double val = values[i].get_value<double>();
		if ( val == missing_ )  {
			points_.back()->missing(true);
		}
		else
			minmax.push_back(val);
		(*points_[i])["x"] = val;

	}
	minx_ = *std::min_element(minmax.begin(), minmax.end());
	maxx_ = *std::max_element(minmax.begin(), minmax.end());


}
void WrepJSon::values(const json_spirit::Value& value)
{
	ASSERT (value.type() == array_type);
	Array values = value.get_value<Array>();
	bool newpoint = points_.empty();


	for (unsigned int i = 0; i < values.size(); i++) {
		if ( newpoint ) {
			CustomisedPoint* point = new CustomisedPoint();
			(*point)["resolution"] = points_along_meridian_;
			points_.push_back(point);
		}
		double val = values[i].get_value<double>();
		if ( val == missing_ )  {
			points_.back()->missing(true);
		}

		(*points_[i])["value"] = val;

	}


}
void WrepJSon::y_values(const json_spirit::Value& value)
{
	ASSERT (value.type() == array_type);
	Array values = value.get_value<Array>();
	bool newpoint = points_.empty();
	vector<double> minmax;

	for (unsigned int i = 0; i < values.size(); i++) {
		if ( newpoint ) {
			CustomisedPoint* point = new CustomisedPoint();
			(*point)["resolution"] = points_along_meridian_;
			points_.push_back(point);
		}
		double val = values[i].get_value<double>();
		if ( val == missing_ )  {
			points_[i]->missing(true);
		}
		else
			minmax.push_back(val);
		(*points_[i])["y"] = val;
	}
	if (minmax.empty()) {
		miny_ = 0;
		maxy_ = 1;
		return;
	}

	miny_ = *std::min_element(minmax.begin(), minmax.end());
	maxy_ = *std::max_element(minmax.begin(), minmax.end());
}

void WrepJSon::x_date_values(const json_spirit::Value& value)
{

	ASSERT (value.type() == array_type);
	Array values = value.get_value<Array>();
	if ( !regular_) {
		xdate_ = true;
	}
	xBase_ = DateTime(values[0].get_value<string>());
	DateTime current;
	bool newpoint = points_.empty();
	vector<double> minmax;

	for (unsigned int i = 0; i < values.size(); i++) {
		current = DateTime(values[i].get_value<string>());

		if ( newpoint ) {
			CustomisedPoint* point = new CustomisedPoint();
			(*point)["resolution"] = points_along_meridian_;
			points_.push_back(point);
		}
		double val =  current - xBase_;
		if ( regular_ ) {
			val = val / (24.*3600);

		}
		(*points_[i])["x"] = val;
		minmax.push_back(val);
	}

	minx_ = *std::min_element(minmax.begin(), minmax.end());
	maxx_ = *std::max_element(minmax.begin(), minmax.end());

}

void WrepJSon::y_date_values(const json_spirit::Value& value)
{
	ASSERT (value.type() == array_type);
	Array values = value.get_value<Array>();
	ydate_ = true;
	yBase_ = DateTime(values[0].get_value<string>());
	DateTime current;
	bool newpoint = points_.empty();
	vector<double> minmax;

	for (unsigned int i = 0; i < values.size(); i++) {
		current = DateTime(values[i].get_value<string>());

		if ( newpoint ) {
			CustomisedPoint* point = new CustomisedPoint();
			(*point)["resolution"] = points_along_meridian_;
			points_.push_back(point);
		}
		double val =  current - yBase_;
		(*points_[i])["y"] = val;
		minmax.push_back(val);
	}
	miny_ = *std::min_element(minmax.begin(), minmax.end());
	maxy_ = *std::max_element(minmax.begin(), minmax.end());

}

json_spirit::Value WrepJSon::eps_resolution()
{
	Value value(tostring(points_along_meridian_));
	return value;
}

json_spirit::Value WrepJSon::deterministic_resolution()
{
	Value value(tostring(points_along_meridian_));
	return value;
}
json_spirit::Value WrepJSon::station_name()
{
	Value value(station_name_);
	return value;
}
json_spirit::Value WrepJSon::height()
{

	if ( height_ == -9999 )
		return Value("");
	return Value(tostring(height_));
}
void WrepJSon::visit(TextVisitor& text)
{
	if ( !title_ )
		return;
	DateTime base(date_, time_);

	if (param_info_ != "none")
		text.update("json", "date", base.tostring("%A %e %B %Y %H UTC"));
	ostringstream location;
	UserPoint point(longitude_, latitude_);
	location << " " << point.asLatitude() << " " << point.asLongitude();
	ostringstream height;

	height << height_ <<  " m";
	if ( position_info_) {
	if (height_ != -9999 ) 
        text.update("json", "height", height.str());
	text.update("json", "location", location.str());
	text.update("json", "grid_point", (mask_ < 0.5 ) ? " (EPS sea point) " : " (EPS land point) ");

	}

	ostringstream full_correction;
	ostringstream short_correction;

	int dett = (points_along_meridian_ * 2) -1;
	int epst = points_along_meridian_ -1;
	if ( (correction_ && height_ != -9999 && param_info_!= "none") ) {
		full_correction << " reduced to " << height_ <<  " m (station height) from " << maground(detz_) << " m (T" << dett << ") and " << maground(epsz_) <<  " m (T" << epst <<")";
		short_correction << " reduced to " << height_ <<  " m (station height) from " << maground(epsz_) <<  " m (T" << epst <<")";
	}

	text.update("json", "full_temperature_correction_info", full_correction.str());
	text.update("json", "short_temperature_correction_info", short_correction.str());
	text.update("json", "parameter_info", (param_info_ == "none") ? "": param_info_ );

	if (param_info_ != "none")
		text.update("json", "station_name", station_name_);

	//(**point)["y"]
	text.update("json", "product_info", product_info_);
	text.update("json", "plumes_interval", tostring(plumes_));

}

void WrepJSon::points(const Transformation& transformation, vector<UserPoint>& points)
{
	decode();

	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point) {
		// Here we need to check we have date!
		double x = (**point)["x"];
		if ( xdate_ ) {
			DateTime ref(transformation.getReferenceX());
			double shift = ref - xBase_;
			x -= shift;
		}
		//doubble v =  (*point)->find("value") != (*point)->end() ) : (**point)["value"] : 0;
		points.push_back(UserPoint(x, (**point)["y"], (**point)["value"]));
		if ( (*point)->missing() )
			points.back().flagMissing();
	}
}

PointsHandler& WrepJSon::points(const Transformation& transformation, bool)
{
	decode();
	for (vector<CustomisedPoint*>::const_iterator point = points_.begin(); point != points_.end(); ++point) {
	// Here we need to check we have date!
		double x = (**point)["x"];
		if ( xdate_ ) {
			DateTime ref(transformation.getReferenceX());
			double shift = ref - xBase_;
			x -= shift;
		}

		list_.push_back(new UserPoint(x, (**point)["y"], (**point)["value"]));
		if ( (*point)->missing() )
			list_.back()->flagMissing();
	}
   pointsHandlers_.push_back(new PointsHandler(list_));
   return *pointsHandlers_.back();
}
