// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelected);
class UMVVM_LoadSlot;
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSlot> LoadSlotClass;

	UPROPERTY(BlueprintAssignable)
	FSlotSelected SlotSelected;
	
	void InitializedLoadSLot();

	UFUNCTION(BlueprintPure)
	UMVVM_LoadSlot* GetLoadSlotByIndex(int32 index);

	void SetSlotNums(int32 nums);
	int32 GetSlotNums() const { return SlotNums;}

	UFUNCTION(BlueprintCallable)
	void NewGameButtonPressed(int32 Slot);

	UFUNCTION(BlueprintCallable)
	void SelectSLotButtonPressed(int32 Slot);
	
	UFUNCTION(BlueprintCallable)
	void NewSlotButtonPressed(int32 Slot,const FString& EnterName);

	UFUNCTION(BlueprintCallable)
	void DeleteButtonPressed();

	void LoadData();
	
private:
	
	UPROPERTY()
	TMap<int32,UMVVM_LoadSlot*> LoadSlots;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;

	UPROPERTY()
	int32 SelectSlot;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess = "true"))
	int32 SlotNums;
};
