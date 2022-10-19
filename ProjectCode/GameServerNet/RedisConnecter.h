#pragma once
#include "TCPSession.h"
#include <string>
#include <memory>

class RedisCommendBase
{
	friend class RedisConnecter;

public:
	void Process(class RedisConnecter& _Con);


protected:
	std::string Query;


	void Plus(const std::string& _Value)
	{
		Query += _Value;
	}

	void TrimPlus(const std::string& _Value)
	{
		Query += _Value + " ";
	}

	void End()
	{
		Query += "\r\n";
	}



	virtual void ConvertResult() = 0;

protected:
	std::string Result;

	std::string ReturnCut() 
	{
		size_t Index = Result.find("\r\n");
		std::string Return = Result.substr(0, Index);
		Result.replace(0, Index + 2, "");
		return Return;
	}

	std::string ReturnCutValue()
	{
		size_t Index = Result.find("\r\n");
		std::string Return = Result.substr(1, Index - 1);
		Result.replace(0, Index + 2, "");
		return Return;
	}

	int GetValueInt() 
	{
		return atoi(GetValueString().c_str());
	}

	char GetType()
	{
		return Result[0];
	}

	bool IsError() 
	{
		if (Result[0] == '-')
		{
			return true;
		}

		return false;
	}

	std::string GetValueString() 
	{
		std::string ValueResult;

		switch (Result[0])
		{
		case '*': // 배열
		{
			ValueResult = ReturnCutValue();
		}
			break; 
		case '+': // 단순문자열
		{
			int a = 0;
		}
			break;
		case '$': // 대량 문자열
		{
			int Count = std::atoi(ReturnCutValue().c_str());

			ValueResult = ReturnCut().c_str();
		}
			break;
		case ':': // 숫자
		{
			ValueResult = ReturnCutValue();
		}
			break;
		case '-':
		{
			int a = 0;
		}
			break;
		default:
			break;
		}

		return ValueResult;
	}


protected:
	RedisCommendBase()
	{
	}

	RedisCommendBase(const std::string& _BaseString)
	{
		TrimPlus(_BaseString);
	}



};


enum class ZADDINSERTOPTION
{
	NewMemberAdd, 
	OldMemberUpdate, 
};

enum class ZRANGEOPTION
{
	LESS,
	GREATER,
};

enum class ZRANGESCOREOPTION
{
	NONE,
	WITHSCORES,
};


class RedisCommend
{
public:
	class SET : public RedisCommendBase
	{
	public:
		SET(const std::string& _Key, const std::string& _Value)
			: RedisCommendBase("SET")
		{
			TrimPlus(_Key);
			TrimPlus(_Value);
			End();
		}

		void ConvertResult() {}
	};

	class GET : public RedisCommendBase
	{
	public:
		GET(const std::string& _Key)
			: RedisCommendBase("GET")
		{
			TrimPlus(_Key);
			End();
		}
		void ConvertResult() {}
	};

	class ZADD : public RedisCommendBase
	{
	public:
		ZADD(const std::string& _Key, int _Score, const std::string& _Member)
			: RedisCommendBase("ZADD")
		{
			TrimPlus(_Key);
			TrimPlus(std::to_string(_Score));
			TrimPlus(_Member);
			End();
		}

		ZADD(const std::string& _Key, int _Score, const std::string& _Member, ZADDINSERTOPTION _InsertOption/*, ZADDOPTION _Option*/)
			// : RedisCommendBase("ZADD")
		{
			TrimPlus(_Key);

			switch (_InsertOption)
			{
			case ZADDINSERTOPTION::NewMemberAdd:
				TrimPlus("NX");
				break;
			case ZADDINSERTOPTION::OldMemberUpdate:
				TrimPlus("XX");
				break;
			default:
				break;
			}

			TrimPlus(std::to_string(_Score));
			TrimPlus(_Member);
			End();
		}

		void ConvertResult() {}
	};


	class ZRANGE : public RedisCommendBase
	{
		ZRANGESCOREOPTION ResultScoreOption;

	public:
		std::vector<std::string> Members;
		std::vector<int> Scores;

	public:
		ZRANGE(const std::string _Key, int _Start, int _End, ZRANGEOPTION _Option, ZRANGESCOREOPTION _ScoreOption)
		{
			switch (_Option)
			{
			case ZRANGEOPTION::LESS:
				TrimPlus("ZRANGE");
				break;
			case ZRANGEOPTION::GREATER:
				TrimPlus("ZREVRANGE");
				break;
			default:
				break;
			}

			TrimPlus(_Key);
			TrimPlus(std::to_string(_Start));
			TrimPlus(std::to_string(_End));

			switch (_ScoreOption)
			{
			case ZRANGESCOREOPTION::NONE:
				break;
			case ZRANGESCOREOPTION::WITHSCORES:
				Plus("WITHSCORES");
				break;
			default:
				break;
			}

			ResultScoreOption = _ScoreOption;

			End();
		}

		void ConvertResult() 
		{
			Result;

			if (IsError())
			{
				return;
			}

			int Count = GetValueInt();

			switch (ResultScoreOption)
			{
			case ZRANGESCOREOPTION::NONE:
				break;
			case ZRANGESCOREOPTION::WITHSCORES:
				Count /= 2;
				break;
			default:
				break;
			}

			for (size_t i = 0; i < Count; i++)
			{

				Members.push_back(GetValueString());

				if (ResultScoreOption == ZRANGESCOREOPTION::WITHSCORES)
				{
					Scores.push_back(GetValueInt());
				}
			}

			int a = 0;
		}

	};



	class ZRANK : public RedisCommendBase
	{
	public:
		int Rank;

	public:
		ZRANK(const std::string _Key, const std::string _Member, ZRANGEOPTION _Option)
		{
			switch (_Option)
			{
			case ZRANGEOPTION::LESS:
				TrimPlus("ZRANK");
				break;
			case ZRANGEOPTION::GREATER:
				TrimPlus("ZREVRANK");
				break;
			default:
				break;
			}

			TrimPlus(_Key);
			TrimPlus(_Member);
			End();
		}

		void ConvertResult() 
		{
			Result;

			if (true == IsError() || '$' == GetType())
			{
				Rank = -1;
				return;
			}

			Rank = GetValueInt();
		}
	};



private:
	RedisCommend() {}
	~RedisCommend() {}
};

class RedisConnecter
{
private:
	static thread_local std::vector<std::shared_ptr<RedisConnecter>> AllConnector;

public:
	static void InitConntor(const std::string& _Host, unsigned int _Port, int _Index = 0);

	static std::shared_ptr<RedisConnecter> GetConnector(int _Index = 0);

public:
	RedisConnecter();
	~RedisConnecter();

	RedisConnecter(const RedisConnecter& _Other) = delete;
	RedisConnecter(RedisConnecter&& _Other) noexcept = delete;
	RedisConnecter& operator=(const RedisConnecter& _Other) = delete;
	RedisConnecter& operator=(RedisConnecter&& _Other) noexcept = delete;


	bool Connect(const std::string& _Host, unsigned int _Port);
	void Close();


	void Query(RedisCommendBase& _Query);
	std::string Query(const std::string& _Query);

protected:
	TCPSession Session_;

private:
};
