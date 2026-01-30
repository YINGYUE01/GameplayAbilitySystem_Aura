// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	int32 LevelUpRequiredXP = 0;
	UPROPERTY(EditAnywhere)
	int AttributePointAward = 1;
	UPROPERTY(EditAnywhere)
	int SpellPointAward = 1;
};
/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	TArray<FAuraLevelUpInfo> LevelUpInformation;
	UFUNCTION()
	int GetLevelForXP(const int32 XP);
};
