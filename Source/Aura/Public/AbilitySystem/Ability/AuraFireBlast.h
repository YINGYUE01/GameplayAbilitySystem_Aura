// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:

	UPROPERTY(EditDefaultsOnly,Category="FireBlast")
	int32 NumFireBalls = 5;
	
};
