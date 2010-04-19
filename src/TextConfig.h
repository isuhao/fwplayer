// **********************************************************************
//
// Copyright (c) 2006-2007 QQTechnology, Inc. All rights reserved.
//
// Created by mxli@qqtechnology.com
//
// **********************************************************************


//////////////////////////////////////////////////////////////////////////
// 
// 后续需要修改、添加的功能：
//	1、将StringDict修改为vector
//	2、支持配置文件的保存功能
//
//////////////////////////////////////////////////////////////////////////
#pragma once


#define MAX_LINE_SIZE	1024


typedef ::std::map< ::std::string, ::std::string> StringDict;

/**
 *
 * 配置访问接口实现对象
 * 基于"key:value"对的配置访问
 * 可跨平台使用
 *
 **/
class TextConfig
{
public:
	TextConfig();
	virtual ~TextConfig();

public:
	//IConfig

	virtual bool initialize(const ::std::string& file);

	virtual void release();

	virtual bool persist();

	virtual ::std::string getValue(const ::std::string& key);

	virtual ::std::string getValueWithDefault(const ::std::string& key, const ::std::string& value);

	virtual int getIntValue(const ::std::string& key);

	virtual int getIntValueWithDefault(const ::std::string& key, int value);

	virtual StringDict getValuesForPrefix(const ::std::string&);

	virtual void setValue(const ::std::string& key, const ::std::string& value);

	virtual void setIntValue(const ::std::string& key, int value);

private:
	//config file
	std::string _file;

	//value map
    StringDict _values;

	//Parse a line
    void parseLine(const std::string&);
};


