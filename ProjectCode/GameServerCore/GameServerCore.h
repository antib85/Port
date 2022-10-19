#pragma once
#include <string>
#include <GameServerNet\TCPListener.h>

class TCPSession;
class UDPSession;
class GameServerCore
{
public:
	GameServerCore();
	~GameServerCore();

	GameServerCore(const GameServerCore& _Other) = delete;
	GameServerCore(GameServerCore&& _Other) noexcept = delete;
	GameServerCore& operator=(const GameServerCore& _Other) = delete;
	GameServerCore& operator=(GameServerCore&& _Other) noexcept = delete;

	static inline int GetServerPort()
	{
		return ServerPort;
	}
	static inline std::string GetDBHost()
	{
		return DBHost;
	}
	static inline int GetDBPort()
	{
		return DBPort;
	}
	static inline std::string GetDBName()
	{
		return DBName;
	}
	static inline std::string GetDBUser()
	{
		return DBUser;
	}
	static inline std::string GetDBPW()
	{
		return DBPW;
	}

	static std::shared_ptr<UDPSession> GetUDPPort(int& _UDPPort);

	template<typename UserGameType, typename ... Types>
	static void Start(Types ... args)
	{
		CoreInit();

		UserGameType NewUserServer = UserGameType(args...);
		NewUserServer.UserStart();

		CoreRun();

		CoreEnd();
	}

protected:
	static IPEndPoint ServerEndPoint;
	static int ServerPort;
	static std::string DBHost;
	static int DBPort;
	static std::string DBName;
	static std::string DBUser;
	static std::string DBPW;
	static std::atomic<int> UDPPortNumber_;
	static std::atomic<int> MaxUDPPortNumber_;

	void SetAcceptCallBack(const std::function<void(std::shared_ptr<TCPSession>)>& _CallBack);

	void InitUDP(int UDPCount, const std::function<void(std::shared_ptr<UDPSession>, const std::vector<unsigned char>&, IPEndPoint&)>& _CallBack);


private:

	static bool CoreInit();
	static bool CoreRun();
	static bool CoreEnd();

	static TCPListener Listener_;
	static std::function<void(std::shared_ptr<TCPSession>)> AcceptCallBack_;

	static std::vector<std::shared_ptr<UDPSession>> AllUDPSession_;

	static std::function<void(std::shared_ptr<UDPSession>, const std::vector<unsigned char>&, IPEndPoint&)>  UDPCallBack_;


};

