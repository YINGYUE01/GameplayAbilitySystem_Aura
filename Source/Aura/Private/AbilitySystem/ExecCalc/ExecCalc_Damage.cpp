// Copyright YING


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	//声明一个属性捕获变量 如ArmorDef
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		//选取真正的值的来源 Source/Target   false表示不使用快照 把捕获放入捕获变量中  P##Def = FGameplayEffectAttributeCaptureDefinition(P##Property, EGameplayEffectAttributeCaptureSource::T, B); 
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPenetration,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitChance,Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitDamage,Source,false);
		
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CriticalHitResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightningResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance,Target,false);

	}
};

static const AuraDamageStatics& DamageStatics()
{
	//静态单例模式，减少性能消耗
	static AuraDamageStatics Statics;
	return Statics;
}
UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec,const FAggregatorEvaluateParameters& EvaluateParameters,
						 const TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition>& TagsToCaptureDefs) const
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	for (const TPair<FGameplayTag,FGameplayTag>& Pair : GameplayTags.DamageTypesToDebuff )
	{
		const FGameplayTag& DamageType = Pair.Key;
		//只有造成伤害才能有机会触发对应的Debuff
		if (Spec.GetSetByCallerMagnitude(DamageType,false,-1.f) > -.5f)
		{
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance,false,-1.f);
			float TargetDebuffResistance = 0.f;
			const FGameplayTag& ResistanceTag = GameplayTags.DamageTypesToResistances[DamageType];
			//获取捕获属性的值
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(TagsToCaptureDefs[ResistanceTag],EvaluateParameters,TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max(TargetDebuffResistance,0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance)/100;
			const bool bDebuff = FMath::RandRange(0.f,100.f) < EffectiveDebuffChance;
			if (bDebuff)
			{
				FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
				UAuraAbilitySystemLibrary::SetSuccessfulDebuff(EffectContextHandle,bDebuff);
				UAuraAbilitySystemLibrary::SetDamageType(EffectContextHandle,DamageType);
				UAuraAbilitySystemLibrary::SetDebuffDamage(EffectContextHandle,Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Debuff_Damage,false,-1.f));
				UAuraAbilitySystemLibrary::SetDebuffDuration(EffectContextHandle,Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Debuff_Duration,false,-1.f));
				UAuraAbilitySystemLibrary::SetDebuffFrequency(EffectContextHandle,Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Debuff_Frequency,false,-1.f));
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//标签与捕获属性定义的映射
	TMap<FGameplayTag,FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags& AuraGameplayTags = FAuraGameplayTags::Get();
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_Armor,DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_BlockChance,DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_ArmorPenetration,DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitChance,DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitResistance,DamageStatics().CriticalHitResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attribute_Secondary_CriticalHitDamage,DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attributes_Resistance_Fire,DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attributes_Resistance_Lightning,DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attributes_Resistance_Arcane,DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(AuraGameplayTags.Attributes_Resistance_Physical,DamageStatics().PhysicalResistanceDef);

	//获取 ASC 和 Avatar
	const UAbilitySystemComponent* SourceASC  = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* SourceAvatar =SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar =TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTag;
	EvaluateParameters.TargetTags = TargetTag;
	
	//Debuff
	DetermineDebuff(ExecutionParams, Spec, EvaluateParameters,TagsToCaptureDefs);
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	float Damage = 0.f;
	for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
	{
		const FGameplayTag DamageType = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		checkf(TagsToCaptureDefs.Contains(ResistanceTag),TEXT("TagsToCaptureDefs Does`t contain Tag [%s] in ExecCalc_Damage"),*ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = TagsToCaptureDefs.FindChecked(ResistanceTag);
		float Resistance = 0.f;
		//伤害抗性与护甲不一样
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters,Resistance);
		Resistance = FMath::Clamp<float>(Resistance,0.f,100.f);
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageType,false);
		DamageTypeValue*=(100.f - Resistance)/100.f;

		//范围伤害
		if (UAuraAbilitySystemLibrary::GetIsRadialDamage(EffectContextHandle))
		{
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageDelegate().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,
				0,
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),
				1.f,
				UDamageType::StaticClass(),
				TArray<AActor*>(),
				SourceAvatar,
				nullptr);
		}
		
		Damage+=DamageTypeValue;
	}
	
	//获取BlockChance  然后判断知否格挡成功
	// 成功 伤害减半
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef,EvaluateParameters,TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance,0.f);
	bool bBlock = FMath::RandRange(0.f,100.f) < TargetBlockChance;
	if (bBlock)
	{
		Damage *=0.5f;
	}
	UAuraAbilitySystemLibrary::SetBlockHit(EffectContextHandle,bBlock);
	
	/* Begin 护甲伤害削减*/
	//获取目标护甲
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor,0.f);
	//获取来源护甲穿透
	float SourceArmorPenetration= 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef,EvaluateParameters,SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration,0.f);
	//护甲对伤害的削减
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	if (CharacterClassInfo)
	{
		FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"),FString());
		const float ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
		
		FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"),FString());
		const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
		
		const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
		Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	}
	/* End 护甲伤害削减*/
	
	/* Begin 暴击伤害计算*/
	//获取暴击率
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef,EvaluateParameters,SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance,0.f);
	//获取目标暴击抗性
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef,EvaluateParameters,TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance,0.f);
	//获取暴击伤害
	float CriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef,EvaluateParameters,CriticalHitDamage);
	CriticalHitDamage = FMath::Max<float>(CriticalHitDamage,0.f);
	//获取暴击抗性因子
	FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHit"),FString());
	const float EffectiveCriticalHitCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel); //根据Level从曲线上获取相应数值
	//根据暴击抗性削弱暴击率
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * EffectiveCriticalHitCoefficient;
	bool bCriticalHit = FMath::RandRange(0.f,100.f) < EffectiveCriticalHitChance;
	if (bCriticalHit)
	{
		//造成暴击 加伤害
		Damage = Damage*2.f + Damage*(CriticalHitDamage/100);
	}
	/* End 暴击伤害计算*/
	UAuraAbilitySystemLibrary::SetCriticalHit(EffectContextHandle,bCriticalHit);
	//执行伤害 通过InComingDamageAttribute属性传递到AttributeSet进行真正的属性变化
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetInComingDamageAttribute(),EGameplayModOp::Additive,Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
