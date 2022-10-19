#pragma once


// OS 기본
#include <WS2tcpip.h> 
#include <WinSock2.h>
#include <Windows.h> 
#include <MSWSock.h>

// 쓰레드
#include <thread>
#include <mutex>
#include <memory>
#include <process.h>
#include <atomic>

// 수학
#include <math.h>
#include <random>

// 자료구조
#include <list>
#include <map>
#include <vector>
#include <stack>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <sstream>
#include <deque>

// 디버깅
#include <functional>
#include <assert.h>
#include <crtdbg.h>


#pragma comment (lib, "ws2_32") 
#pragma comment (lib, "mswsock.lib")

#include <GameServerBase/GameServerObjectBase.h>
#include <GameServerBase/GameServerNameBase.h>
#include <GameServerBase/GameServerQueue.h>
#include <GameServerBase/GameServerDebug.h>
#include <GameServerBase/GameServerUnique.h>
#include <GameServerBase/GameServerObjectPool.h>
#include <GameServerBase/GameServerObjectPool.h>

#include "Enums.h"
#include "TypeDefine.h"

#pragma comment (lib, "..\\Thirdparty\\Lib\\libmysql.lib")
