#pragma once

class ServerHelper
{
public: 
	static void StartEngineStartUp();

private:
	ServerHelper();
	~ServerHelper();

	ServerHelper(const ServerHelper& _Other) = delete;
	ServerHelper(ServerHelper&& _Other) noexcept;

protected:
	ServerHelper& operator=(const ServerHelper& _Other) = delete;
	ServerHelper& operator=(ServerHelper&& _Other) = delete;

private:

public: 
};

