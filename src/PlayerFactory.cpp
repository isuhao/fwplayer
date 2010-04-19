#include "stdafx.h"
#include "PlayerFactory.h"
#include "WMPDlg.h"

PlayerFactory _playerFactory;

PlayerFactory::PlayerFactory()
{
}


PlayerFactory::~PlayerFactory()
{
}

CVideoPlayer* PlayerFactory::CreatePlayer(int type)
{
	switch(type)
	{
	case PT_WMP:
		return new CWMPDlg();
	case PT_RMP:
		//return new CRMPDlg();
		break;
	case PT_QTP:
		//return new CQTPDlg();
		break;
	case PT_FMP:
		//return new CFMPDlg();
		break;
	};

	return NULL;
}