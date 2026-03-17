// Copyright YING


#include "AbilitySystem/Ability/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	FGameplayEffectSpecHandle DamageHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	const float ScaleDamage =Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageHandle,DamageType,ScaleDamage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageHandle.Data.Get(),UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefault(AActor* TargetActor, FVector InRadialDamageOrigin,bool bOverrideKnockbackDirection,FVector KnockbackDirectionOverride,bool bOverrideDeathDirection ,FVector OverrideDeathDirection,bool bOverridePitch,float PitchOverride ) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamageGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.DamageType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	DamageEffectParams.KnockbackChance = KnockbackChance;
	DamageEffectParams.KnockbackImpulse = KnockbackImpulse;
	DamageEffectParams.KnockbackImpulseMagnitude = KnockbackImpulseMagnitude;

	//设置击退和死亡击退参数
	if (IsValid(TargetActor))
	{
		FRotator Rotation =(TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		FVector Target = Rotation.Vector();
		if (bOverrideDeathDirection)
		{
			DamageEffectParams.DeathImpulse = Target * DeathImpulseMagnitude;
		}
		if (bOverrideKnockbackDirection)
		{
			DamageEffectParams.KnockbackImpulse = Target * KnockbackImpulseMagnitude;
		}
	}
	if (bOverrideKnockbackDirection)
	{
		KnockbackDirectionOverride.Normalize();
		DamageEffectParams.KnockbackImpulse = KnockbackDirectionOverride * KnockbackImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator KnockbackRotation = KnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = PitchOverride;
			DamageEffectParams.KnockbackImpulse = KnockbackRotation.Vector() * KnockbackImpulseMagnitude;
		}
	}
	if (bOverrideDeathDirection)
	{
		OverrideDeathDirection.Normalize();
		DamageEffectParams.DeathImpulse = OverrideDeathDirection * DeathImpulseMagnitude;
		if (bOverridePitch)
		{
			FRotator DeathRotation = OverrideDeathDirection.Rotation();
			DeathRotation.Pitch = PitchOverride;
			DamageEffectParams.DeathImpulse = DeathRotation.Vector() * DeathImpulseMagnitude;
		}
	}

	//判断是不是范围伤害类型
	if (bIsRadiaDamage)
	{
		DamageEffectParams.bIsRadiaDamage = bIsRadiaDamage;
		DamageEffectParams.RadiaDamageInnerRadius = RadiaDamageInnerRadius;
		DamageEffectParams.RadiaDamageOuterRadius = RadiaDamageOuterRadius;
		DamageEffectParams.RadiaDamageOrigin = InRadialDamageOrigin;
	}
	return DamageEffectParams;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel()
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.Num()>0)
	{
		const int32 Selection = FMath::RandRange(0,TaggedMontages.Num()-1);
		return TaggedMontages[Selection];
	}
	return FTaggedMontage();
}
