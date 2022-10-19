#include "PreCompile.h"
#include <GameServerContents\StudyGameServerCore.h>
#include <GameServerNet\RedisConnecter.h>
#include <GameServerBase\GameServerString.h>
#include <GameServerBase\GameServerSerializer.h>

int main() 
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameServerCore::Start<StudyGameServerCore>();
}