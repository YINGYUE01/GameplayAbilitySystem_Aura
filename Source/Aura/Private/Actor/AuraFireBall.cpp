// Copyright YING


#include "Actor/AuraFireBall.h"
#include "GameplayCueManager.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Net/UnrealNetwork.h"

void AAuraFireBall::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(AAuraFireBall,ReturnToActor,COND_None,REPNOTIFY_Always);
}

void AAuraFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters Parameters;
		Parameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(),FAuraGameplayTags::Get().GameplayCue_FireBlast,Parameters);
	}
	if (AudioComponent)
	{
		AudioComponent->Stop();
		AudioComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
	StatOutgoingTimeline();
}

void AAuraFireBall::OnSphereOverlay(UPrimitiveComponent* OverlapedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (HasAuthority())
	{
		if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemLibrary::ApplyDamageEffectFromDamageEffectParams(DamageEffectParams);
		}
	}
}
