// Copyright YING


#include "AbilitySystem/Ability/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

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

void UAuraBeamSpell::StoreOwnerVariable()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter);
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter);
		if (Weapon)
		{
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			UKismetSystemLibrary::SphereTraceSingle(
				OwnerCharacter,
				SocketLocation,
				BeamTargetLocation,
				10.f,
				ETraceTypeQuery::TraceTypeQuery1,
				false,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				HitResult,
				true);
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
		}
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(MouseHitActor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this,&UAuraBeamSpell::PrimaryTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UAuraBeamSpell::PrimaryTargetDied);
			}
		}
	}
	
}

void UAuraBeamSpell::StoreAdditionTargets(TArray<AActor*>& OutAdditionTargets)
{
	TArray<AActor*> ActorsIgnore;
	ActorsIgnore.Add(OwnerCharacter);
	ActorsIgnore.Add(MouseHitActor);
	TArray<AActor*> OverlappingTargets;
	int32 NumBeamShocks = FMath::Min(MaxNumShockingTargets,GetAbilityLevel());
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(),OverlappingTargets,ActorsIgnore,850.f,MouseHitActor->GetActorLocation());
	UAuraAbilitySystemLibrary::GetClosestTargets(NumBeamShocks,OverlappingTargets,OutAdditionTargets,MouseHitActor->GetActorLocation());
	for (AActor* Actor : OutAdditionTargets)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Actor))
		{
			if (!CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this,&UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}
}
