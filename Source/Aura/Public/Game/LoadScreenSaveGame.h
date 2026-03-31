// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};
USTRUCT(BlueprintType)
struct FSaveActor
{
	GENERATED_BODY()
	UPROPERTY()
	FName ActorName = FName();
	UPROPERTY()
	FTransform Transform = FTransform();

	UPROPERTY()
	TArray<uint8> Bytes; 
};
inline bool operator==(const FSaveActor& Left,const FSaveActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSaveMap
{
	GENERATED_BODY()
	UPROPERTY()
	FString MapName = FString();
	UPROPERTY()
	TArray<FSaveActor> Actors;
};


USTRUCT(BlueprintType)
struct FSaveAbility
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite,Category="Ability Class")
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(BlueprintReadWrite,Category="Ability Tag")
	FGameplayTag AbilityTag;

	UPROPERTY(BlueprintReadWrite,Category="Ability Slot")
	FGameplayTag AbilitySlot;

	UPROPERTY(BlueprintReadWrite,Category="Ability Type")
	FGameplayTag AbilityType;

	UPROPERTY(BlueprintReadWrite,Category="Ability Status")
	FGameplayTag AbilityStatus;

	UPROPERTY(BlueprintReadWrite,Category="Ability Level")
	int32 AbilityLevel;
	
};

inline bool operator==(const FSaveAbility& Left,const FSaveAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}
/**
 * 
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString SlotName = FString();
	
	UPROPERTY()
	int32 SlotIndex = 0;
	
	UPROPERTY()
	FString PlayerName = FString("Default Name");
	
	UPROPERTY()
	FString MapName = FString("Default Map Name");
		
	UPROPERTY()
	FString MapAssetName = FString("Default Map Asset Name");

	UPROPERTY()
	FName PlayerStartTag;
	
	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
	bool bFirstSave = true;
	/* Player */
	UPROPERTY()
	int32 PlayerLevel = 1;
	UPROPERTY()
	int32 XP = 0;
	UPROPERTY()
	int32 AttributePoints = 0;
	UPROPERTY()
	int32 SpellPoints = 0;
	/* Attribute */
	UPROPERTY()
	float Strength = 0;
	UPROPERTY()
	float Vigor = 0;
	UPROPERTY()
	float Intelligence = 0;
	UPROPERTY()
	float Resilience = 0;
	/* Abilities */
	UPROPERTY()
	TArray<FSaveAbility> SaveAbilities;

	/* Map And MapActors */
	UPROPERTY()
	TArray<FSaveMap> SaveMaps;

	FSaveMap GetSaveMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
