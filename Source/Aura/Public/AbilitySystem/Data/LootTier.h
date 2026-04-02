// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LootTier.generated.h"

USTRUCT(BlueprintType)
struct FLootItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AActor> LootItemClass;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float SpawnLootItemChance = 0;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	float MaxSpawnLootItemNums = 1;
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	bool bLootLevelOverride = false;
};
/**
 * 
 */
UCLASS()
class AURA_API ULootTier : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TArray<FLootItem> LootItems;

	UFUNCTION(BlueprintCallable)
	TArray<FLootItem> GetLootItems();
};
