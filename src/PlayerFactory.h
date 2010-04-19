#ifndef __PLAYER_FACTORY__
#define __PLAYER_FACTORY__

#include "VideoPlayer.h"

//Player Types
enum{
	PT_WMP = 0, //Windows Media Player
	PT_RMP = 1, //Real Media Player
	PT_QTP = 2, //Quick Time Player
	PT_FMP = 3,	//Flash Media Player
};

//Object Factory of different players
class PlayerFactory
{
public:
	PlayerFactory();
	~PlayerFactory();

	CVideoPlayer* CreatePlayer(int type); //Factory Method

};

extern PlayerFactory _playerFactory;

#endif