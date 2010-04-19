// **********************************************************************
//
// Copyright (c) 2006-2007 QQTechnology, Inc. All rights reserved.
//
// Created by mxli@qqtechnology.com
//
// **********************************************************************


//////////////////////////////////////////////////////////////////////////
// 
// ������Ҫ�޸ġ���ӵĹ��ܣ�
//	1����StringDict�޸�Ϊvector
//	2��֧�������ļ��ı��湦��
//
//////////////////////////////////////////////////////////////////////////
#pragma once


#define MAX_LINE_SIZE	1024


typedef ::std::map< ::std::string, ::std::string> StringDict;

/**
 *
 * ���÷��ʽӿ�ʵ�ֶ���
 * ����"key:value"�Ե����÷���
 * �ɿ�ƽ̨ʹ��
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


