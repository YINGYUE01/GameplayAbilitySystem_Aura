// Copyright YING


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::SetSlotName(FString Name)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotName,Name);
}

void UMVVM_LoadSlot::InitializeSlot()
{
	const int32 Index = SlotStatus.GetIntValue();
	SetWidgetSwitcherIndex.Broadcast(Index);
}

void UMVVM_LoadSlot::SetPlayerName(FString Name)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName,Name);
}

void UMVVM_LoadSlot::SetMapName(FString Name)
{
	UE_MVVM_SET_PROPERTY_VALUE(MapName,Name);
}
