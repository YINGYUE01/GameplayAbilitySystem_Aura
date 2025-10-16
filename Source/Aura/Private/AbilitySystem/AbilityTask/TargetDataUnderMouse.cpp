// Copyright YING


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle AbilitySpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredicationKey = GetActivationPredictionKey();
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(AbilitySpecHandle,ActivationPredicationKey).AddUObject(this,&UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(AbilitySpecHandle,ActivationPredicationKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
			
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow PredictionWindow(AbilitySystemComponent.Get(),GetActivationPredictionKey(),true);
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHit);
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey
		);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
	
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
