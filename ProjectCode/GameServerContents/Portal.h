#pragma once
#include <GameServerCore\GameServerActor.h>

class Portal : public GameServerActor
{
public:
	Portal(FVector4 _Pos, FVector4 _Scale);
	~Portal();

	Portal(const Portal& _Other) = delete;
	Portal(Portal&& _Other) noexcept = delete;
	Portal& operator=(const Portal& _Other) = delete;
	Portal& operator=(Portal&& _Other) noexcept = delete;

	GameServerSection* LinkSection;

protected:

private:
	void SectionInitialize() override;
	void TCPSessionInitialize() override {}
	void UDPSessionInitialize() override {}

	void Update(float _Time) {}
	bool InsertSection() { return true; }
	void DeathEvent() {}


};

