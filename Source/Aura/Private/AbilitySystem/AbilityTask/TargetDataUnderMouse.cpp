// Copyright YING


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	FHitResult HitResult;
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	PC->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
	//Cast<APlayerController>(GetOwnerActor())
	FVector MouseLocation = HitResult.Location;
	ValidData.Broadcast(MouseLocation);
}
