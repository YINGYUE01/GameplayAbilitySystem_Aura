#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass=nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0;
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackImpulseMagnitude = 0;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRadiaDamage= false;
	
	UPROPERTY(BlueprintReadWrite)
	float RadiaDamageInnerRadius= 0;

	UPROPERTY(BlueprintReadWrite)
	float RadiaDamageOuterRadius= 0;

	UPROPERTY(BlueprintReadWrite)
	FVector RadiaDamageOrigin= FVector::Zero();
	
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()
public:
	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	
	bool IsCriticalHit() const { return bIsCriticalHit; }
	void SetCriticalHit(bool bCriticalHit) { bIsCriticalHit = bCriticalHit; }
	
	bool IsBlockHit() const { return bIsBlockHit; }
	void SetBlockHit(bool bBlockHit) { bIsBlockHit = bBlockHit; }
	
	void SetDebuffDuration(float Duration) { this->DebuffDuration = Duration;}
	float GetDebuffDuration() const { return DebuffDuration;}
	
	void SetDebuffDamage(float Damage) { this->DebuffDamage = Damage;}
	float GetDebuffDamage() const { return DebuffDamage;}
	
	void SetDebuffFrequency(float Frequency) { this->DebuffFrequency = Frequency;}
	float GetDebuffFrequency() const { return DebuffFrequency;}
	
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff;}
	void SetIsSuccessfulDebuff(bool bSuccessfulDebuff) { bIsSuccessfulDebuff = bSuccessfulDebuff; }

	TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }
	void SetDamageType(TSharedPtr<FGameplayTag> Type) { this->DamageType = Type;}

	FVector GetDeathImpulse() const { return DeathImpulse;}
	void SetDeathImpulse(FVector Impulse) { this->DeathImpulse = Impulse;}

	FVector GetKnockbackImpulse() const { return KnockbackImpulse;}
	void SetKnockbackImpulse(FVector Impulse) { this->KnockbackImpulse = Impulse;}

	bool IsRadiaDamage() const {return bIsRadiaDamage;}
	void SetIsRadiaDamage(const bool bRadiaDamage) { this->bIsRadiaDamage = bRadiaDamage;}

	float GetRadiaDamageInnerRadius() const { return RadiaDamageInnerRadius;}
	void SetRadiaDamageInnerRadius(const float Radius) { this->RadiaDamageInnerRadius = Radius;}

	float GetRadiaDamageOuterRadius() const { return RadiaDamageOuterRadius;}
	void SetRadiaDamageOuterRadius(const float Radius) { this->RadiaDamageOuterRadius = Radius;}

	FVector GetRadiaDamageOrigin() const { return RadiaDamageOrigin; }
	void SetRadiaDamageOrigin(const FVector Origin) { this->RadiaDamageOrigin = Origin;}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FAuraGameplayEffectContext* Duplicate() const
	{
		FAuraGameplayEffectContext* NewContext = new FAuraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
private:
	//伤害显示相关参数
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsBlockHit = false;

	//Debuff 相关参数
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;

	//范围伤害相关参数
	UPROPERTY()
	bool bIsRadiaDamage= false;
	UPROPERTY()
	float RadiaDamageInnerRadius= 0;
	UPROPERTY()
	float RadiaDamageOuterRadius= 0;
	UPROPERTY()
	FVector RadiaDamageOrigin= FVector::Zero();
	
	TSharedPtr<FGameplayTag> DamageType;

	//击退相关参数
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY()
	FVector KnockbackImpulse = FVector::ZeroVector;
	
};
template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};
