#pragma once
#include <..\ProjectCode\Thirdparty\include\mysql.h>
#include <string>
#include <string_view>
#include <vector>

class DBStmt;
class DBConnecter;
class DBStmtResult : public std::enable_shared_from_this<DBStmt> 
{
	friend DBStmt;

public:
	DBStmtResult(DBConnecter* _Ptr, MYSQL_STMT* _stmt, std::string_view _Query);

private:
	std::vector<unsigned long> ResultLengthBuffer_;
	std::vector<char> ResultIsNullBuffer_;
	std::vector<char> ResultBindBuffer_;

	std::vector<MYSQL_BIND> ResultBinds_;

	DBConnecter* DBConnecterPtr_;
	std::string_view Query_;
	MYSQL_STMT* Stmt_;


public:

	inline uint64_t AffectedRows() { return mysql_stmt_affected_rows(Stmt_); };

	bool Next();

	std::string GetString(const int _Value);
	int GetInt(const int _Value);
	float GetFloat(const int _Value);
};

class DBConnecter;
class DBStmt : public std::enable_shared_from_this<DBStmt>
{
public:
	DBStmt(DBConnecter* _Ptr, MYSQL_STMT* _stmt, std::string_view _Query);

	std::vector<unsigned long> ParamLengthBuffer_;
	std::vector<char> ParamIsNullBuffer_;
	std::vector<char> ParamBindBuffer_;

	std::vector<MYSQL_BIND> Parambinds_;

	DBConnecter* DBConnecterPtr_;
	std::string_view Query_;
	MYSQL_STMT* Stmt_;

public:
	void ParamBindString(std::string_view _Value);
	void ParamBindInt(const int _Value);
	void ParamBindFloat(const float _Value);
	void ParamBindDataTime(const std::string _Value);

public:
	std::unique_ptr<DBStmtResult> Execute();

	inline uint64_t InsertId() { return mysql_stmt_insert_id(Stmt_); };

	inline uint64_t AffectedRows() { return mysql_stmt_affected_rows(Stmt_); };
};

class DBConnecter
{
private:
	static thread_local std::vector<std::shared_ptr<DBConnecter>> AllConnector;

public:
	static bool InitConntor(const std::string& _Host, const std::string& _Id, const std::string& _Pw, const std::string& _Schema, unsigned int _Port, int _Index = 0);

	static std::shared_ptr<DBConnecter> GetConnector(int _Index = 0);


public:
	DBConnecter();
	~DBConnecter();

	DBConnecter(const DBConnecter& _Other) = delete;
	DBConnecter(DBConnecter&& _Other) noexcept = delete;
	DBConnecter& operator=(const DBConnecter& _Other) = delete;
	DBConnecter& operator=(DBConnecter&& _Other) noexcept = delete;

	std::unique_ptr<DBStmt> CreateStmt(std::string_view _Query);

	void ConnectInfoSetting(
		const std::string& _Host,
		const std::string& _Id,
		const std::string& _Pw,
		const std::string& _Schema,
		unsigned int _Port
	);

	bool Connect();

	bool Connect(
		const std::string& _Host,
		const std::string& _Id,
		const std::string& _Pw,
		const std::string& _Schema,
		unsigned int _Port
	);

	inline bool IsConnect() const
	{


		return mysql_ != nullptr;
	}

	std::string GetLastError();

	void Reset();

protected:

private:
	MYSQL* mysql_;
	std::string Host_;
	std::string Id_;
	std::string Pw_;
	std::string Schema_;
	unsigned int Port_;
};

