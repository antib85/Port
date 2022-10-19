#pragma once
#include "DBConnecter.h"

class DBTableBase
{
public:
	DBTableBase(DBConnecter* _Connecter);
	~DBTableBase();

	DBTableBase(const DBTableBase& _Other) = delete;
	DBTableBase(DBTableBase&& _Other) noexcept = delete;
	DBTableBase& operator=(const DBTableBase& _Other) = delete;
	DBTableBase& operator=(DBTableBase&& _Other) noexcept = delete;

protected:

private:
	DBConnecter* Connecter_;
};

