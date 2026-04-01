// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Engine/TargetPoint.h"
#include "SpawnEnemyPoint.generated.h"
enum class ECharacterClass : uint8;
class AAuraEnemy;
/**
 * 
 */
UCLASS()
class AURA_API ASpawnEnemyPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	ASpawnEnemyPoint();
	UPROPERTY(EditAnywhere)
	TSubclassOf<AAuraEnemy> EnemyClass;
	UPROPERTY(EditAnywhere)
	int32 EnemyLevel = 1;
	UPROPERTY(EditAnywhere)
	ECharacterClass EnemyCharacterClass = ECharacterClass::Warrior;
	void SpawnEnemy();
};
