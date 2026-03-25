// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(int32 InLevel);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 CurrentLevel,int32 NewLevel);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 CurrentLevel,int32 NewLevel);

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints();

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints();

	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);
	
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void ShowMagicCircle(UMaterialInterface* Material);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void HideMagicCircle();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SetMagicCircleMaterial(UMaterialInterface* Material);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void SaveGameProgress(const FName& CheckPointTag);
	
};
