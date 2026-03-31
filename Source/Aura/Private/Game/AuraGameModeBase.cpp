// Copyright YING


#include "Game/AuraGameModeBase.h"

#include "EngineUtils.h"
#include "Game/AuraGameInstance.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Serialization/Archive.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->SaveSlotStatus = Taken;
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
	
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

ULoadScreenSaveGame*  AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	return GetSaveSlotData(LoadSlotName,LoadSlotIndex);

}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	const FString LoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 LoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	AuraGameInstance->PlayerStatTag = SaveObject->PlayerStartTag;
	UGameplayStatics::SaveGameToSlot(SaveObject,LoadSlotName,LoadSlotIndex);
	
}

void AAuraGameModeBase::SaveWorldState(UWorld* World,const FString& DestinationAssetMapName)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);
	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName,AuraGI->LoadSlotIndex))
	{
		if (DestinationAssetMapName!=FString(""))
		{
			SaveGame->MapAssetName = DestinationAssetMapName;
			SaveGame->MapName = GetMapNameFromAssetMapName(DestinationAssetMapName);
		}
		if (!SaveGame->HasMap(WorldName))
		{
			FSaveMap SaveMap;
			SaveMap.MapName = WorldName;
			SaveGame->SaveMaps.Add(SaveMap);
		}
		FSaveMap SaveMap = SaveGame->GetSaveMapWithMapName(WorldName);
		SaveMap.Actors.Empty();
		for (FActorIterator It(World); It ; ++It)
		{
			AActor* Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
			FSaveActor SaveActor;
			SaveActor.ActorName = Actor->GetFName();
			SaveActor.Transform = Actor->GetTransform();
			FMemoryWriter MemoryWriter(SaveActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter,true);
			Archive.GetArchiveState().ArIsSaveGame = true;
			Actor->Serialize(Archive);
			SaveMap.Actors.Add(SaveActor);
		}
		for (FSaveMap& MapToReplace : SaveGame->SaveMaps)
		{
			if (MapToReplace.MapName==WorldName)
			{
				MapToReplace = SaveMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World)
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName,AuraGI->LoadSlotIndex))
	{
		ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName,AuraGI->LoadSlotIndex);
		if (SaveGame==nullptr) return;
		for (FActorIterator It(World);It;++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USaveInterface>()) continue;
			for (FSaveActor SaveActor : SaveGame->GetSaveMapWithMapName(WorldName).Actors)
			{
				if (SaveActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
						Actor->SetActorTransform(SaveActor.Transform);
					FMemoryReader MemoryReader(SaveActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader,true);
					Archive.GetArchiveState().ArIsSaveGame = true;
					Actor->Serialize(Archive);
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}

}

FString AAuraGameModeBase::GetMapNameFromAssetMapName(const FString& InAssetMapName)
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName()==InAssetMapName)
		{
			return Map.Key;
		}
	}
	return FString();
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelMap(UMVVM_LoadSlot* LoadSlot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(LoadSlot,Maps.FindChecked(LoadSlot->GetMapName()));
}

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName,DefaultMap);
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* GameInstance  = Cast<UAuraGameInstance>(GetGameInstance());
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(),Actors);
	if (Actors.Num()>0)
	{
		AActor* SelectedActor = Actors[0];
		for (auto Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == GameInstance->PlayerStatTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	else
	{
		return nullptr;
	}

}
