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
	}
	
}

void UAuraBeamSpell::StoreAdditionTargets(TArray<AActor*>& OutAdditionTargets)
{
	TArray<AActor*> ActorsIgnore;
	ActorsIgnore.Add(OwnerCharacter);
	ActorsIgnore.Add(MouseHitActor);
	TArray<AActor*> OverlappingTargets;
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(GetAvatarActorFromActorInfo(),OverlappingTargets,ActorsIgnore,850.f,MouseHitActor->GetActorLocation());
	UAuraAbilitySystemLibrary::GetClosestTargets(MaxNumShockingTargets,OverlappingTargets,OutAdditionTargets,MouseHitActor->GetActorLocation());
	
}
