// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UAbilityInfo;
class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,Category="Character Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;
	UPROPERTY(EditAnywhere,Category="Character Class Defaults")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot,int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);
	void SaveWorldState(UWorld* World,const FString& DestinationAssetMapName = FString(""));
	void LoadWorldState(UWorld* World);
	FString GetMapNameFromAssetMapName(const FString& InAssetMapName);
	void PlayerDied(ACharacter* DeadCharacter);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName,int32 SlotIndex) const;
	void DeleteSlot(const FString& SlotName,int32 SlotIndex);

	void TravelMap(UMVVM_LoadSlot* LoadSlot);

	UPROPERTY(EditDefaultsOnly)
	FName DefaultPlayStartName;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;
	UPROPERTY(EditDefaultsOnly)
	TMap<FString,TSoftObjectPtr<UWorld>> Maps;
protected:
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
