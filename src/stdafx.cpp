// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// fwclient.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

int cmpOrder(const void *p1, const void *p2)
{
	mappair *order1 = *(mappair **)p1;
	mappair *order2 = *(mappair **)p2;
	
	if( order1->order>order2->order)
		return 1;
	else if( order1->order ==order2->order)
		return 0;
	else return -1;
}

void string_replace(std::string & strBig, const std::string & strsrc, const std::string &strdst) 
{
	std::string::size_type pos=0;
	std::string::size_type srclen=strsrc.size();
	std::string::size_type dstlen=strdst.size();
	while( (pos=strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.erase(pos, srclen);
		strBig.insert(pos, strdst);
		pos += dstlen;
	}
}