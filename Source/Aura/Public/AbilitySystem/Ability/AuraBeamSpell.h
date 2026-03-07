// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"
#include "AuraBeamSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void StoreMouseDataInfo(const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void StoreOwnerPlayerController();
protected:
	UPROPERTY(BlueprintReadWrite)
	FVector MouseHitLocation;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> MouseHitActor;
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerController> OwnerPlayerController;
};
