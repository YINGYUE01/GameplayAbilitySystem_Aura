// Copyright YING


#include "AbilitySystem/Ability/AuraBeamSpell.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& Hit)
{
	if (Hit.bBlockingHit)
	{
		MouseHitLocation = Hit.ImpactPoint;
		MouseHitActor = Hit.GetActor();
	}
	else
	{
		EndAbility(CurrentSpecHandle,CurrentActorInfo,CurrentActivationInfo,true,false);
	}
}

void UAuraBeamSpell::StoreOwnerPlayerController()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
	}
}
