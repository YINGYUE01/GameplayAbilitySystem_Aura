// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"
enum ESaveSlotStatus : int;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndex, int32, WidgetSwitchIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButton, bool, bEnableButton);
/**
 * 
 */
UCLASS()
class AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()
public:
	void SetSlotName(FString Name);
	FString GetSlotName() const{return SlotName;};

	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndex SetWidgetSwitcherIndex;
	UPROPERTY(BlueprintAssignable)
	FEnableSelectSlotButton EnableSelectSlotButton;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter,meta=(AllowPrivateAccess = "true"))
	FString SlotName;

	void InitializeSlot();

	UPROPERTY()
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY()
	FString LoadSlotName;

	UPROPERTY()
	FString SlotIndex;

	//FieldNotify
	UPROPERTY(EditAnywhere,BlueprintReadWrite,FieldNotify,Setter,Getter)
	FString PlayerName;

	void SetPlayerName(FString Name);
	FString GetPlayerName() const { return PlayerName;}
};
