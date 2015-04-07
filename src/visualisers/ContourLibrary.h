/*! \file ContourLibrary.h
    \brief Definition of the Template class ContourLibrary.
    
    Magics Team - ECMWF 2010
    
    Started: Fri 16-Jul-2010
    
    Changes:
    
*/

#ifndef ContourLibrary_H
#define ContourLibrary_H

#include "magics.h"

#include "MagTranslator.h"
#include "MagConfig.h"

namespace magics {

class MetaDataCollector;
class VisDefInfoBase;

class ContourLibrary {

public:
	ContourLibrary();
	virtual ~ContourLibrary();

	// set the meta data to be collected
	virtual void askId(MetaDataCollector&);

	virtual bool checkId(MetaDataCollector&,MetaDataCollector&);


	// se the map to set the contour!
	virtual void getAttributes(MetaDataCollector&, map<string, string>&);
	
protected:
     //! Method to print string about this class on to a stream of type ostream (virtual).
	 virtual void print(ostream&) const; 

private:
    //! Copy constructor - No copy allowed
	ContourLibrary(const ContourLibrary&);
    //! Overloaded << operator to copy - No copy allowed
	ContourLibrary& operator=(const ContourLibrary&);
	
	bool setInfoObject(string);

// -- Friends
    //! Overloaded << operator to call print().
	friend ostream& operator<<(ostream& s,const ContourLibrary& p)
		{ p.print(s); return s; }

	static VisDefInfoBase* info_;

};



class EcChartData : public MagConfig
{

public:
	EcChartData(const string& name) {
		path_ = getEnvVariable("MAGPLUS_HOME") + MAGPLUS_PATH_TO_SHARE_ + "/" + name + ".json";
		MagConfigHandler(path_, *this); }

	~EcChartData() {}

	void callback(const string&, const json_spirit::Value&);
	map<string, string> getMap(const int);
	void init();
	map<int, map<string, string> > data_;
	string path_;

	friend ostream& operator<<(ostream& s,const EcChartData& p)
	{
		for (map<int, map<string, string> >::const_iterator d = p.data_.begin(); d != p.data_.end(); ++d) {
			s << d->first << "[" << endl;
			for (map<string, string>::const_iterator i = d->second.begin(); i != d->second.end(); ++i) {
				s << "    " << i->first << " = " << i->second << endl;
			}
			s << "]" << endl;
		}
		s << "read from " << p.path_;
	}

};

class EcChartSetData : public MagConfig
{
public:
	EcChartSetData(const string& name) {
		path_ = getEnvVariable("MAGPLUS_HOME") + MAGPLUS_PATH_TO_SHARE_ + "/" + name + ".json";
		MagConfigHandler(path_, *this); }
	~EcChartSetData() {}

	void callback(const string&, const json_spirit::Value&);
	bool hasKey(const string&);
	vector<int> getSet(const string&);

	void init();
	map<string, vector<int> > data_;
	string path_;

	friend ostream& operator<<(ostream& s,const EcChartSetData& p)
	{
		for (map<string, vector<int> >::const_iterator d = p.data_.begin(); d != p.data_.end(); ++d) {
			s << d->first << "[" << endl;
			for (vector<int>::const_iterator i = d->second.begin(); i != d->second.end(); ++i) {
				s << "    " << *i << endl;
			}
			s << "]" << endl;
		}
		s << "read from " << p.path_;
	}

};

class EcChartLibrary : public ContourLibrary {


public:
	EcChartLibrary();
	virtual ~EcChartLibrary();

	// set the meta data to be collected
	void askId(MetaDataCollector&);

	bool checkId(MetaDataCollector&,MetaDataCollector&) { return true; }
	void setCriteria(MetaDataCollector&, const string&);

	// set the map to set the contour!
	void getAttributes(MetaDataCollector&, map<string, string>&);



protected:
     //! Method to print string about this class on to a stream of type ostream (virtual).
	 virtual void print(ostream&) const;

	EcChartData contours_;
	EcChartSetData default_set_;
	vector<string> keys_;
	map<string,EcChartSetData> index_;

};

class NoContourLibrary : public ContourLibrary {

public:
	NoContourLibrary() {}
	virtual ~NoContourLibrary() {}

	// sete the meata dat to be collected
	void askId(MetaDataCollector&) {}

	bool checkId(MetaDataCollector&,MetaDataCollector&) { return false; }

	// se the map to set the contour!
	void getAttributes(MetaDataCollector&, map<string, string>&) {}

protected:
     //! Method to print string about this class on to a stream of type ostream (virtual).
	 virtual void print(ostream&) const {}

};

template <>
class MagTranslator<string, ContourLibrary > {
public:
	ContourLibrary* operator()(const string& val )
	{
		 return SimpleObjectMaker<ContourLibrary >::create(val);
	}

};
} // namespace magics
#endif
