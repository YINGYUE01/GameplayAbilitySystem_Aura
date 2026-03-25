// Copyright YING


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializedLoadSLot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_0->LoadSlotName = FString("LoadSlot_0");
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_1->LoadSlotName = FString("LoadSlot_1");
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_2->LoadSlotName = FString("LoadSlot_2");
	LoadSlots.Add(2,LoadSlot_2);
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetLoadSlotByIndex(int32 index)
{
	return LoadSlots.FindChecked(index);
}

void UMVVM_LoadScreen::SetSlotNums(int32 nums)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotNums,nums);
}

void UMVVM_LoadScreen::NewGameButtonPressed(int32 Slot)
{
	LoadSlots[Slot]->SetWidgetSwitcherIndex.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSLotButtonPressed(int32 Slot)
{
	SlotSelected.Broadcast();
	for (TTuple<int32,UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		if (LoadSlot.Key == Slot)
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(false);
			SelectSlot = Slot;
		}
		else
		{
			LoadSlot.Value->EnableSelectSlotButton.Broadcast(true);
		}
	}
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnterName)
{
	AAuraGameModeBase* GameModeBase = Cast<AAuraGameModeBase>(GetWorld()->GetAuthGameMode());
	LoadSlots[Slot]->SetMapName(GameModeBase->DefaultMapName);
	LoadSlots[Slot]->SetPlayerName(EnterName);
	LoadSlots[Slot]->SlotStatus = Taken;
	GameModeBase->SaveSlotData(LoadSlots[Slot],Slot);
	LoadSlots[Slot]->InitializeSlot();
	UAuraGameInstance* GameInstance  = Cast<UAuraGameInstance>(GameModeBase->GetGameInstance());
	GameInstance->LoadSlotName = LoadSlots[Slot]->LoadSlotName;
	GameInstance->LoadSlotIndex = LoadSlots[Slot]->SlotIndex;
	GameInstance->PlayerStatTag = GameModeBase->DefaultPlayStartName;
}

void UMVVM_LoadScreen::DeleteButtonPressed()
{
	AAuraGameModeBase* AuraGameModeBase = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameModeBase)
	{
		AuraGameModeBase->DeleteSlot(LoadSlots[SelectSlot]->LoadSlotName,SelectSlot);
		LoadSlots[SelectSlot]->SlotStatus = Vacant;
		LoadSlots[SelectSlot]->InitializeSlot();
		LoadSlots[SelectSlot]->EnableSelectSlotButton.Broadcast(true);
	}
}

void UMVVM_LoadScreen::PlayButtonPressed()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	AuraGameMode->TravelMap(LoadSlots[SelectSlot]);
}

void UMVVM_LoadScreen::LoadData()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	for (TTuple<int32,UMVVM_LoadSlot*> LoadSlot : LoadSlots)
	{
		ULoadScreenSaveGame* LoadScreenSaveGame = AuraGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName,LoadSlot.Key);
		const FString PlayerName = LoadScreenSaveGame->PlayerName;
		LoadSlot.Value->SetMapName(LoadScreenSaveGame->MapName);
		LoadSlot.Value->SetPlayerName(PlayerName);
		LoadSlot.Value->SlotStatus = LoadScreenSaveGame->SaveSlotStatus;
		LoadSlot.Value->InitializeSlot();
	}
}
