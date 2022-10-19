#include "PreCompile.h"
#include "DBConnecter.h"
#include <GameServerBase\GameServerDebug.h>

DBStmtResult::DBStmtResult(DBConnecter* _ConncterPtr, MYSQL_STMT* _Stmt, std::string_view _Query)
	: DBConnecterPtr_(_ConncterPtr)
	, Query_(_Query)
	, Stmt_(_Stmt)
{
	ResultLengthBuffer_.reserve(20);
	ResultIsNullBuffer_.reserve(20);
	ResultBindBuffer_.reserve(1024);
}

bool DBStmtResult::Next() 
{
	return 0 == mysql_stmt_fetch(Stmt_);
}

std::string DBStmtResult::GetString(const int _Index)
{
	return std::string(reinterpret_cast<char*>(ResultBinds_[_Index].buffer));
}

int DBStmtResult::GetInt(const int _Index)
{
	return *reinterpret_cast<int32_t*>(ResultBinds_[_Index].buffer);
}


float DBStmtResult::GetFloat(const int _Index)
{
	return *reinterpret_cast<float*>(ResultBinds_[_Index].buffer);
}

DBStmt::DBStmt(DBConnecter* _ConncterPtr, MYSQL_STMT* _Stmt, std::string_view _Query)
	: DBConnecterPtr_(_ConncterPtr)
	, Query_(_Query)
	, Stmt_(_Stmt)
{
	ParamLengthBuffer_.reserve(20);
	ParamIsNullBuffer_.reserve(20);
	ParamBindBuffer_.reserve(1024);

}

void DBStmt::ParamBindString(std::string_view _Value)
{
	ParamLengthBuffer_.emplace_back();
	ParamIsNullBuffer_.emplace_back();
	size_t Size = ParamBindBuffer_.size();
	ParamBindBuffer_.resize(ParamBindBuffer_.size() + _Value.size());

	MYSQL_BIND& Bind = Parambinds_.emplace_back();
	Bind.buffer_type = MYSQL_TYPE_VARCHAR;
	Bind.buffer = &ParamBindBuffer_[Size];
	memset(Bind.buffer, 0x00, _Value.size() + 1);
	memcpy_s(Bind.buffer, _Value.length(), _Value.data(), _Value.length());
	Bind.buffer_length = static_cast<unsigned long>(_Value.length());
	Bind.is_null = reinterpret_cast<bool*>(&ParamIsNullBuffer_[ParamIsNullBuffer_.size() - 1]);
	*Bind.is_null = false;
	Bind.length = &ParamLengthBuffer_[ParamLengthBuffer_.size() - 1];
	*Bind.length = Bind.buffer_length;
}
void DBStmt::ParamBindInt(const int _Value)
{
	ParamLengthBuffer_.emplace_back();
	ParamIsNullBuffer_.emplace_back();
	size_t Size = ParamBindBuffer_.size();
	ParamBindBuffer_.resize(ParamBindBuffer_.size() + sizeof(int));

	MYSQL_BIND& Bind = Parambinds_.emplace_back();
	Bind.buffer_type = MYSQL_TYPE_LONG;
	Bind.buffer = &ParamBindBuffer_[Size];
	memset(Bind.buffer, 0x00, sizeof(int));
	memcpy_s(Bind.buffer, sizeof(int), &_Value, sizeof(int));
	Bind.buffer_length = static_cast<unsigned long>(sizeof(int));
	Bind.is_null = reinterpret_cast<bool*>(&ParamIsNullBuffer_[ParamIsNullBuffer_.size() - 1]);
	*Bind.is_null = false;
	Bind.length = &ParamLengthBuffer_[ParamLengthBuffer_.size() - 1];
	*Bind.length = Bind.buffer_length;
}
void DBStmt::ParamBindFloat(const float _Value)
{

}

void DBStmt::ParamBindDataTime(const std::string _Value)
{
	ParamLengthBuffer_.emplace_back();
	ParamIsNullBuffer_.emplace_back();
	size_t Size = ParamBindBuffer_.size();
	ParamBindBuffer_.resize(ParamBindBuffer_.size() + _Value.size());

	MYSQL_BIND& Bind = Parambinds_.emplace_back();
	Bind.buffer_type = MYSQL_TYPE_DATETIME;
	Bind.buffer = &ParamBindBuffer_[Size];
	memset(Bind.buffer, 0x00, _Value.size() + 1);
	memcpy_s(Bind.buffer, _Value.length(), _Value.data(), _Value.length());
	Bind.buffer_length = static_cast<unsigned long>(_Value.length());
	Bind.is_null = reinterpret_cast<bool*>(&ParamIsNullBuffer_[ParamIsNullBuffer_.size() - 1]);
	*Bind.is_null = false;
	Bind.length = &ParamLengthBuffer_[ParamLengthBuffer_.size() - 1];
	*Bind.length = Bind.buffer_length;
}


std::unique_ptr<DBStmtResult> DBStmt::Execute()
{
	int ParamCount = mysql_stmt_param_count(Stmt_);

	if (ParamCount != Parambinds_.size())
	{
		GameServerDebug::AssertDebugMsg("query bind count not Equal");
		return nullptr;
	}

	if (0 != ParamCount)
	{
		if (0 != mysql_stmt_bind_param(Stmt_, &Parambinds_[0]))
		{
			GameServerDebug::AssertDebugMsg("mysql_stmt_bind_param Error" + DBConnecterPtr_->GetLastError());
			return nullptr;
		}
	}

	MYSQL_RES* ResultMetaData = mysql_stmt_result_metadata(Stmt_);

	std::unique_ptr<DBStmtResult> Result = nullptr;

	if (nullptr != ResultMetaData)
	{
		Result = std::make_unique<DBStmtResult>(DBConnecterPtr_, Stmt_, Query_);

		int ColumnCount = mysql_num_fields(ResultMetaData);
		for (size_t i = 0; i < ColumnCount; i++)
		{
			MYSQL_BIND& ResultBind = Result->ResultBinds_.emplace_back();
			Result->ResultIsNullBuffer_.emplace_back();
			Result->ResultLengthBuffer_.emplace_back();

			MYSQL_FIELD& field = ResultMetaData->fields[i];

			size_t BufferStart = Result->ResultBindBuffer_.size();

			switch (field.type)
			{
			case enum_field_types::MYSQL_TYPE_VAR_STRING:
			{
				Result->ResultBindBuffer_.resize(Result->ResultBindBuffer_.size() + field.length + 1);
				ResultBind.buffer_type = enum_field_types::MYSQL_TYPE_VAR_STRING;
				ResultBind.buffer = &Result->ResultBindBuffer_[BufferStart];
				// ResultBind.buffer = new char[field.length + 1];
				ResultBind.buffer_length = field.length + 1;
				memset(ResultBind.buffer, 0x00, field.length + 1);
				ResultBind.is_null = reinterpret_cast<bool*>(&Result->ResultIsNullBuffer_[Result->ResultIsNullBuffer_.size() - 1]);
				*ResultBind.is_null = false;
				ResultBind.length = &Result->ResultLengthBuffer_[Result->ResultLengthBuffer_.size() - 1];
				break;
			}
			case enum_field_types::MYSQL_TYPE_LONG:
			{
				Result->ResultBindBuffer_.resize(Result->ResultBindBuffer_.size() + sizeof(int));
				ResultBind.buffer_type = enum_field_types::MYSQL_TYPE_LONG;
				ResultBind.buffer = &Result->ResultBindBuffer_[BufferStart];
				// ResultBind.buffer = new char[sizeof(int)];
				ResultBind.buffer_length = sizeof(int);
				memset(ResultBind.buffer, 0x00, sizeof(int));
				ResultBind.is_null = reinterpret_cast<bool*>(&Result->ResultIsNullBuffer_[Result->ResultIsNullBuffer_.size() - 1]);
				*ResultBind.is_null = false;
				ResultBind.length = &Result->ResultLengthBuffer_[Result->ResultLengthBuffer_.size() - 1];
				break;
			}
			case enum_field_types::MYSQL_TYPE_FLOAT:
			{
				Result->ResultBindBuffer_.resize(Result->ResultBindBuffer_.size() + sizeof(float));
				ResultBind.buffer_type = enum_field_types::MYSQL_TYPE_FLOAT;
				ResultBind.buffer = &Result->ResultBindBuffer_[BufferStart];
				// ResultBind.buffer = new char[sizeof(int)];
				ResultBind.buffer_length = sizeof(float);
				memset(ResultBind.buffer, 0x00, sizeof(float));
				ResultBind.is_null = reinterpret_cast<bool*>(&Result->ResultIsNullBuffer_[Result->ResultIsNullBuffer_.size() - 1]);
				*ResultBind.is_null = false;
				ResultBind.length = &Result->ResultLengthBuffer_[Result->ResultLengthBuffer_.size() - 1];
				break;
			}
			case enum_field_types::MYSQL_TYPE_DATETIME:
			{
				Result->ResultBindBuffer_.resize(Result->ResultBindBuffer_.size() + field.length + 1);
				ResultBind.buffer_type = enum_field_types::MYSQL_TYPE_DATETIME;
				ResultBind.buffer = &Result->ResultBindBuffer_[BufferStart];
				// ResultBind.buffer = new char[field.length + 1];
				ResultBind.buffer_length = field.length + 1;
				memset(ResultBind.buffer, 0x00, field.length + 1);
				ResultBind.is_null = reinterpret_cast<bool*>(&Result->ResultIsNullBuffer_[Result->ResultIsNullBuffer_.size() - 1]);
				*ResultBind.is_null = false;
				ResultBind.length = &Result->ResultLengthBuffer_[Result->ResultLengthBuffer_.size() - 1];
				break;
			}
			default:
				GameServerDebug::AssertDebugMsg("mysql result type switch error");
				break;
			}
		}

		if (0 != mysql_stmt_bind_result(Stmt_, &Result->ResultBinds_[0]))
		{
			GameServerDebug::AssertDebugMsg("mysql_stmt_bind_result Error" + DBConnecterPtr_->GetLastError());
			return nullptr;
		}

	}

	if (0 != mysql_stmt_execute(Stmt_))
	{
		DBConnecterPtr_->GetLastError();

		return nullptr;
	}

	if (0 != mysql_stmt_store_result(Stmt_))
	{
		GameServerDebug::AssertDebugMsg("mysql_stmt_store_result Error" + DBConnecterPtr_->GetLastError());
		return nullptr;
	}

	if (nullptr != ResultMetaData)
	{
		mysql_free_result(ResultMetaData);
	}

	return Result;
}

/////////////////////////////////////////////// CONNECTER

thread_local std::vector<std::shared_ptr<DBConnecter>> DBConnecter::AllConnector;

bool DBConnecter::InitConntor(const std::string& _Host, const std::string& _Id, const std::string& _Pw, const std::string& _Schema, unsigned int _Port, int _Index /*= 0*/)
{
	AllConnector.resize(_Index + 1);
	AllConnector[_Index] = std::make_shared<DBConnecter>();
	return AllConnector[_Index]->Connect(_Host, _Id, _Pw, _Schema, _Port);
}

std::shared_ptr<DBConnecter> DBConnecter::GetConnector(int _Index /*= 0*/) 
{
	return AllConnector[_Index];
}

DBConnecter::DBConnecter()
	: mysql_(nullptr)
{
}

DBConnecter::~DBConnecter()
{
}

void DBConnecter::Reset() 
{
	mysql_ = nullptr;
	Host_ = std::string();
	Id_ = std::string();
	Pw_ = std::string();
	Schema_ = std::string();

}

std::unique_ptr<DBStmt> DBConnecter::CreateStmt(std::string_view _Query)
{
	MYSQL_STMT* stmt = mysql_stmt_init(mysql_);

	if (nullptr == stmt)
	{
		GameServerDebug::AssertDebugMsg("mysql Statememnt Create Error");
		return nullptr;
	}

	if (0 != mysql_stmt_prepare(stmt, _Query.data(), static_cast<unsigned long>(_Query.length())))
	{
		mysql_stmt_close(stmt);
		GameServerDebug::AssertDebugMsg("mysql Statememnt mysql_stmt_prepare Error");
		return nullptr;
	}

	return std::make_unique<DBStmt>(this, stmt, _Query);
}

void DBConnecter::ConnectInfoSetting(
	const std::string& _Host,
	const std::string& _Id,
	const std::string& _Pw,
	const std::string& _Schema,
	unsigned int _Port
) 
{
	Host_ = _Host;
	Id_ = _Id;
	Pw_ = _Pw;
	Schema_ = _Schema;
	Port_ = _Port;
}


bool DBConnecter::Connect()
{
	if (nullptr != mysql_)
	{
		return true;
	}

	mysql_ = mysql_init(nullptr);

	if (nullptr == mysql_)
	{
		GameServerDebug::AssertDebugMsg("Mysql Init Error");
		return false;
	}

	mysql_options(mysql_, MYSQL_SET_CHARSET_NAME, "utf8");

	MYSQL* Handle = mysql_real_connect(mysql_, Host_.c_str(), Id_.c_str(), Pw_.c_str(), Schema_.c_str(), Port_, nullptr, CLIENT_MULTI_RESULTS);

	if (mysql_ != Handle)
	{
		GameServerDebug::AssertDebugMsg(std::string("Mysql connect Error =>") + GetLastError());
		return false;
	}

	bool reconnect = true;
	mysql_options(mysql_, MYSQL_OPT_RECONNECT, &reconnect);

	return true;
}

std::string DBConnecter::GetLastError() 
{
	return mysql_error(mysql_);
}



bool DBConnecter::Connect(const std::string& _Host, const std::string& _Id, const std::string& _Pw, const std::string& _Schema, unsigned int _Port)
{
	ConnectInfoSetting(_Host, _Id, _Pw, _Schema, _Port);
	return Connect();
}
