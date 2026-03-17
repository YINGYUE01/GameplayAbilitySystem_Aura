// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "ArcaneShardsAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShardsAbility : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ArcaneShards")
	int32 NumPoints = 1;
};
