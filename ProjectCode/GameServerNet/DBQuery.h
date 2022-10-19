#pragma once
#include "DBConnecter.h"

class DBQuery
{


public:
	DBQuery(const char* _Query);
	~DBQuery();
	DBQuery(const DBQuery& _Other) = delete;
	DBQuery(DBQuery&& _Other) noexcept = delete;
	DBQuery& operator=(const DBQuery& _Other) = delete;
	DBQuery& operator=(DBQuery&& _Other) noexcept = delete;

	virtual bool DoQuery() = 0;

protected:
	std::shared_ptr<DBConnecter> DBConnecterPtr;
	const char* QueryString;

private:

	
};

