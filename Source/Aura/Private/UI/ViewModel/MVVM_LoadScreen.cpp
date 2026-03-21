// Copyright YING


#include "UI/ViewModel/MVVM_LoadScreen.h"

#include "UI/ViewModel/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializedLoadSLot()
{
	LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_0->SetSlotName(FString("LoadSlot_0"));
	LoadSlots.Add(0,LoadSlot_0);
	LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_1->SetSlotName(FString("LoadSlot_1"));
	LoadSlots.Add(1,LoadSlot_1);
	LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this,LoadSlotClass);
	LoadSlot_2->SetSlotName(FString("LoadSlot_2"));
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
}

void UMVVM_LoadScreen::NewSlotButtonPressed(int32 Slot, const FString& EnterName)
{
}
