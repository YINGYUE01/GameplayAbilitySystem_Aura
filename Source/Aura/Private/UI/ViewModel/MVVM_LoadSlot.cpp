// Copyright YING


#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::SetSlotName(FString Name)
{
	UE_MVVM_SET_PROPERTY_VALUE(SlotName,Name);
}

void UMVVM_LoadSlot::InitializeSlot()
{
	SetWidgetSwitcherIndex.Broadcast(1);
}
