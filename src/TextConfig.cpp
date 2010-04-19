// **********************************************************************
//
// Copyright (c) 2006-2007 QQTechnology, Inc. All rights reserved.
//
// Created by mxli@qqtechnology.com
//
// **********************************************************************

#include "stdafx.h"
#include "TextConfig.h"

using namespace std;

TextConfig::TextConfig()
{

}

TextConfig::~TextConfig()
{
	_values.clear();
}

bool
TextConfig::initialize(const std::string& file)
{
	ifstream in(file.c_str());
	if(!in)
	{
		return false;
	}

	_file = file;
	char line[MAX_LINE_SIZE];
	while(in.getline(line, MAX_LINE_SIZE))
	{
		parseLine(line);
	}

	return true;
}


void 
TextConfig::release()
{
	delete this;
}

bool 
TextConfig::persist()
{
	ofstream out(_file.c_str());
	if(!out)
	{
		return false;
	}

	StringDict::iterator it;
	for(it = _values.begin(); it != _values.end(); ++it)
	{
		out << it->first << "=" << it->second << "\n";
	}
	
	return true;
}

void
TextConfig::parseLine(const string& line)
{
	const string delim = " \t\r\n";
	string s = line;

	string::size_type idx = s.find('#');
	if(idx != string::npos)
	{
		s.erase(idx);
	}

	idx = s.find_last_not_of(delim);
	if(idx != string::npos && idx + 1 < s.length())
	{
		s.erase(idx + 1);
	}

	string::size_type beg = s.find_first_not_of(delim);
	if(beg == string::npos)
	{
		return;
	}

	string::size_type end = s.find_first_of(delim + "=", beg);
	if(end == string::npos)
	{
		return;
	}

	string key = s.substr(beg, end - beg);

	end = s.find('=', end);
	if(end == string::npos)
	{
		return;
	}
	++end;

	string value;
	beg = s.find_first_not_of(delim, end);
	if(beg != string::npos)
	{
		end = s.length();
		value = s.substr(beg, end - beg);
	}

	setValue(key, value);
}

string
TextConfig::getValue(const string& key)
{
    StringDict::const_iterator p = _values.find(key);
    if(p != _values.end())
    {
		return p->second;
    }
    else
    {
		return string();
    }
}

string
TextConfig::getValueWithDefault(const string& key, const string& value)
{
    StringDict::const_iterator p = _values.find(key);
    if(p != _values.end())
    {
		return p->second;
    }
    else
    {
		return value;
    }
}

int
TextConfig::getIntValue(const string& key)
{
    return getIntValueWithDefault(key, 0);
}

int
TextConfig::getIntValueWithDefault(const string& key, int value)
{  
    StringDict::const_iterator p = _values.find(key);
    if(p != _values.end())
    {
		istringstream v(p->second);
		if(!(v >> value) || !v.eof())
		{
			return 0;
		}
    }

    return value;
}

StringDict 
TextConfig::getValuesForPrefix(const ::std::string& prefix)
{
	StringDict result;
	StringDict::const_iterator p;
	for(p = _values.begin(); p != _values.end(); ++p)
	{
		if(prefix.empty() || p->first.compare(0, prefix.size(), prefix) == 0)
		{
			result.insert(*p);
		}
	}

	return result;
}

void
TextConfig::setValue(const string& key, const string& value)
{
    if(key.empty())
    {
		return;
    }
 
    //
    // Set or clear the property.
    //
    if(!value.empty())
    {
		_values[key] = value;
    }
    else
    {
		_values.erase(key);
    }
}

void
TextConfig::setIntValue(const string& key, int value)
{
	if(key.empty())
	{
		return;
	}

	//
	// int to string
	//
	std::ostringstream ostr;
	ostr << value;

	setValue(key, ostr.str());
}