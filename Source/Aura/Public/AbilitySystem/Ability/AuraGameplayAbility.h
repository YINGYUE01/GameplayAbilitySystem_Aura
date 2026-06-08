// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartupInputTag;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ComboAbilityMontage")
	TObjectPtr<UAnimMontage> ComboMontage;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 CurrentComboIndex = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 ComboCount = 3;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bNextAttack = false;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	virtual FString GetDescription(int32 Level);
	virtual FString GetNextLevelDescription(int32 Level);
	virtual float GetManaCost(float Level = 1.0f);
	virtual float GetCooldown(float Level = 1.0f);
	static FString GetLockedDescription(int32 LevelRequirement);

};
