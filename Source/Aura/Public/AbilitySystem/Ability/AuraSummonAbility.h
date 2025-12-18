// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();
	UFUNCTION(BlueprintPure)
	TSubclassOf<APawn> GetRandomMinionClass();
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Summoning")
	int32 NumMinions = 5;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Summoning")
	float SpawnSpread = 90.f;
	
	
};
