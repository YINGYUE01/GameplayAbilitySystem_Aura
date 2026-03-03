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
	bool IsBlockHit() const { return bIsBlockHit; }
	void SetCriticalHit(bool bCriticalHit) { bIsCriticalHit = bCriticalHit; }
	void SetBlockHit(bool bBlockHit) { bIsBlockHit = bBlockHit; }
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
