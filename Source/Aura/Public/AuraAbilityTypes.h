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

	UPROPERTY()
	float BaseDamage = 0;

	UPROPERTY()
	float AbilityLevel = 1.f;
	
	UPROPERTY()
	float DebuffChance = 0;

	UPROPERTY()
	float DebuffDamage = 0;

	UPROPERTY()
	float DebuffDuration = 0;

	UPROPERTY()
	float DebuffFrequency = 0;
	
	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();
	
	UPROPERTY()
	float DeathImpulseMagnitude = 0;
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
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	bool bIsBlockHit = false;
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	UPROPERTY()
	float DebuffDamage = 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	TSharedPtr<FGameplayTag> DamageType;
	
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
