// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <string>
#include <vector>
#include <Memory>
#include <functional>
#include "../Message/Messages.h"
#include "../Message/ContentsStructure.h"
#include "../Play/ChatWindow.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Interfaces/IPv4/IPv4Endpoint.h"
#include "ClientGameInstance.generated.h"

class UnrealRecvThread : public FRunnable 
{
private:
	ISocketSubsystem* SocketSubSystem_;
	TAtomic<bool> IsAppClose_;
	FSocket* RecvSocket_;
	TQueue<std::shared_ptr<GameServerMessage>, EQueueMode::Spsc>* MessageQueue_;

public:
	UnrealRecvThread(ISocketSubsystem* SocketSubSystem_, FSocket* _RecvSocket, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue_);

	static bool FunctionTest();

	uint32 Run() override;

public:

	void Close();

};

class UnrealUDPRecvThread : public FRunnable
{
private:
	ISocketSubsystem* SocketSubSystem_;
	TAtomic<bool> IsAppClose_;
	FSocket* RecvSocket_;
	TQueue<std::shared_ptr<GameServerMessage>, EQueueMode::Spsc>* MessageQueue_;

public:
	UnrealUDPRecvThread(ISocketSubsystem* SocketSubSystem_, FSocket* _RecvSocket, TQueue<std::shared_ptr<GameServerMessage>>* MessageQueue_);

	static bool FunctionTest();

	uint32 Run() override;

public:

	void Close();

};


class AClientMonster;
UCLASS()
class UNREALCLIENT_API UClientGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TQueue<std::shared_ptr<GameServerMessage>> MessageQueue_;

	UnrealRecvThread* RecvThread_;
	FRunnableThread* ThreadRunalbe_;

	UnrealUDPRecvThread* UDPRecvThread_;
	FRunnableThread* UDPThreadRunalbe_;

	ISocketSubsystem* SocketSubSystem_;
	FSocket* TCPSocket_;
	FSocket* UDPSocket_;

	FIPv4Address ConnectAddress_;
	FIPv4Address UDPAddress_;
	int Port_;
	FIPv4Endpoint EndPoint_;

	int ClientRecvUDPPort_; 
	int ServerSendUDPPort_; 
	FIPv4Endpoint ClientRecvUDPEndPoint_;
	FIPv4Endpoint ServerSendUDPEndPoint_;

private:
	bool ClientMode_;

public:
	UFUNCTION(BlueprintCallable, Category = "Instance")
	FORCEINLINE int GetClientIndex()
	{
		return ObjectIndex;
	}

	FORCEINLINE bool GetClientMode() 
	{
		return ClientMode_;
	}

	FORCEINLINE void SetClientMode(bool _Value)
	{
		ClientMode_ = _Value;
	}

public:
	FString ID;
	FString NickName;
	std::string NickNameUTF8;
	UChatWindow* ChatWindow_;
	TAtomic<bool> LoginProcess_;
	std::vector<FCharacterInfo> Characters_;
	std::vector<FPostData> PostData_;
	class UListView* CharacterListView_;
	class UListView* TopRankListView_;
	class UListView* MyRankListView_;
	class UListView* PostListView_;
	FCharacterInfo SelectCharacter;
	FPostData PostData;
	uint64_t ObjectIndex;
	uint64_t ThreadIndex;
	uint64_t SectionIndex;
	ESlateVisibility RankWindowMode;
	ESlateVisibility PostWindowMode;
	class UWidget* RankWindow;
	class UWidget* PostWindow;
	class UPlayUIUserWidget* PlayUI;
	class UPostBox* PostBox_;

public:
	UClientGameInstance();
	~UClientGameInstance();

	void PushClientMessage(std::shared_ptr<GameServerMessage> _Message);

	std::shared_ptr<GameServerMessage> PopMessage();
	bool IsEmptyMessage();

	bool IsThreadCheck();

	bool ServerConnect(const FString& _IPString, const FString& _PORTString);

	bool ServerUDPConnect(const FString& _PORTString, uint64 _ServerSendUDP);
	void Close();

	bool Send(const std::vector<uint8>& _Data);

	bool SendTo(const std::vector<uint8>& _Data);

	void FinishDestroy() override;
};
