#pragma once
#include <GameServerBase\GameServerMath.h>
#include "GameServerActor.h"
#include <DirectXCollision.h>
#include <DirectXCollision.inl>

enum CollisionCheckType
{
	SPHERE,	 
	AABB, 
	OBB,
	MAX,
};

class CollisionData 
{
public:
	union 
	{
		DirectX::BoundingSphere Sphere;
		DirectX::BoundingBox AABB;
		DirectX::BoundingOrientedBox OBB;
	};

	CollisionData() 
	{

	}
};

class GameServerCollision : public GameServerObjectBase
{
	friend class GameServerActor;
	friend class GameServerSection;

public:
	static bool CollisionCheck(GameServerCollision* _Left, CollisionCheckType _LeftType, GameServerCollision* _Right, CollisionCheckType _RightType);

	static std::function<bool(GameServerCollision*, GameServerCollision*)> CollisionCheckFunction[static_cast<int>(CollisionCheckType::MAX)][static_cast<int>(CollisionCheckType::MAX)];

	static bool SphereToSphere(GameServerCollision* _Left, GameServerCollision* _Right);
	static bool OBBToOBB(GameServerCollision* _Left, GameServerCollision* _Right);
	static bool SphereToOBB(GameServerCollision* _Left, GameServerCollision* _Right);

public:
	GameServerCollision();
	~GameServerCollision();

	GameServerCollision(const GameServerCollision& _Other) = delete;
	GameServerCollision(GameServerCollision&& _Other) noexcept = delete;
	GameServerCollision& operator=(const GameServerCollision& _Other) = delete;
	GameServerCollision& operator=(GameServerCollision&& _Other) noexcept = delete;

	GameServerSection* GetOwnerSection()
	{
		return OwnerSection;
	}

	GameServerActor* GetOwnerActor()
	{
		return OwnerActor;
	}

	template<typename OwnerType>
	OwnerType* GetOwnerActorConvert()
	{
		return dynamic_cast<OwnerType*>(OwnerActor);
	}


	FVector4 GetScale() 
	{
		return Scale;
	}

	FVector4 GetPivot() 
	{
		return Pivot;
	}

	void SetScale(const FVector4& _Value)
	{
		Scale = _Value;
	}

	void SetPivot(const FVector4& _Value)
	{
		Pivot = _Value;
	}

	void SetGroupIndex(int _Index)
	{
		Index_ = _Index;
	}


	template<typename EnumType>
	bool CollisionCheck(CollisionCheckType _ThisType, EnumType _Order, CollisionCheckType _OtherType)
	{
		return CollisionCheck(_ThisType, static_cast<int>(_Order), _OtherType);
	}

	bool CollisionCheck(CollisionCheckType _ThisType, int _Order, CollisionCheckType _OtherType);


	template<typename EnumType>
	bool CollisionCheckResult(CollisionCheckType _ThisType, EnumType _Order
		, CollisionCheckType _OtherType, std::vector<GameServerCollision*>& _HitResults)
	{
		return CollisionCheckResult(
			_ThisType, static_cast<int>(_Order)
			, _OtherType, _HitResults
		);
	}

	bool CollisionCheckResult(CollisionCheckType _ThisType, int _Order
		, CollisionCheckType _OtherType, std::vector<GameServerCollision*>& _HitResults);

	void CollisionDataUpdate();

	void Death() override;


protected:

private:
	GameServerSection* OwnerSection;
	GameServerActor* OwnerActor;
	FVector4 Scale;
	FVector4 Pivot;

	FVector4 Rot;
	int Index_;

	CollisionData CollisionData_;


	inline void SetSection(GameServerSection* _OwnerSection)
	{
		OwnerSection = _OwnerSection;
	}

	inline void SetActor(GameServerActor* _Actor) 
	{
		OwnerActor = _Actor;
	}
};

