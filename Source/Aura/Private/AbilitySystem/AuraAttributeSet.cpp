// Copyright YING


#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"
#include "Player/AuraPlayerController.h"

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Strength,GetStrengthAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Intelligence,GetIntelligenceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Resilience,GetResilienceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Primary_Vigor,GetVigorAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_Armor,GetArmorAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_ArmorPenetration,GetArmorPenetrationAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_BlockChance,GetBlockChanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_CriticalHitChance,GetCriticalHitChanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_CriticalHitDamage,GetCriticalHitDamageAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_CriticalHitResistance,GetCriticalHitResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_HealthRegeneration,GetHealthRegenerationAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_ManaRegeneration,GetManaRegenerationAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_MaxHealth,GetMaxHealthAttribute);
	TagsToAttribute.Add(GameplayTags.Attribute_Secondary_MaxMana,GetMaxManaAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Fire,GetFireResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Lightning,GetLightningResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Arcane,GetArcaneResistanceAttribute);
	TagsToAttribute.Add(GameplayTags.Attributes_Resistance_Physical,GetPhysicalResistanceAttribute);
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Strength,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Intelligence,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Resilience,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Vigor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,Armor,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArmorPenetration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,BlockChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitChance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitDamage,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,CriticalHitResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,HealthRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ManaRegeneration,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,FireResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,LightningResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,ArcaneResistance,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet,PhysicalResistance,COND_None,REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	// if (Attribute==GetHealthAttribute())
	// {
	// 	NewValue = FMath::Clamp(NewValue,0.f,GetMaxHealth());
	// }
	if (Attribute==GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue,0.f,GetMaxMana());
	}
}
void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	//
	SetEffectProperties(Data,Props);
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_bIsDead(Props.TargetCharacter))
		{
			return ;
		}
	}
	if (Data.EvaluatedData.Attribute==GetHealthAttribute())
	{
		//
		SetHealth(FMath::Clamp(GetHealth(),0.f,GetMaxHealth()));
		const bool bFatal = GetHealth()<=0.f;
		if (bFatal)
		{
			
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
	
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
		}
		//UE_LOG(LogTemp,Warning,TEXT("Change Health on %s Health:%f"),*Props.TargetAvatarActor->GetName(),GetHealth());
	}
	if (Data.EvaluatedData.Attribute==GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(),0.f,GetMaxMana()));
	}
	if (Data.EvaluatedData.Attribute == GetInComingDamageAttribute())
	{
		HandleInComingDamage(Props);
	}
	if (Data.EvaluatedData.Attribute==GetInComingXPAttribute())
	{
		HandleInComingXP(Props);
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetMaxHealthAttribute() && bChangedMaxHealth)
	{
		SetHealth(GetMaxHealth());
		bChangedMaxHealth = false;
	}
	if (Attribute == GetMaxManaAttribute() && bChangedMaxMana)
	{
		SetMana(GetMaxMana());
		bChangedMaxMana = false;
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController==nullptr && Props.SourceAvatarActor!=nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
		
}

void UAuraAttributeSet::SendXPEvent(FEffectProperties& Props) 
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		const int32 Level = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		ECharacterClass CharacterClass = ICombatInterface::Execute_GetCharacterCLass(Props.TargetCharacter);
		int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(Props.TargetCharacter,CharacterClass,Level);
		const FAuraGameplayTags& GameplayTag = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTag.Attributes_Meta_IncomingXP;
		Payload.EventMagnitude = XPReward;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,GameplayTag.Attributes_Meta_IncomingXP,Payload);
	}
	
}

void UAuraAttributeSet::Debuff(FEffectProperties& Props)
{
	const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContextHandle = Props.SourceASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(Props.SourceAvatarActor);
	
	const FGameplayTag& DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);
	 
	const FString DamageName = FString::Printf(TEXT("Debuff Name %s"),*DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(),FName(DamageName));
	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);
	//Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuff[DamageType]);
	UTargetTagsGameplayEffectComponent& TargetTagsComponent = Effect->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	// 2. 构造要添加的标签容器
	FGameplayTag DebuffTag = GameplayTags.DamageTypesToDebuff[DamageType];
	FInheritedTagContainer TagContainer;
	if (DebuffTag.MatchesTagExact(FAuraGameplayTags::Get().Debuff_Stun))
	{
		TagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_CursorTrace);
		TagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputHeld);
		TagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputPressed);
		TagContainer.AddTag(FAuraGameplayTags::Get().Player_Block_InputReleased);
	}
	TagContainer.AddTag(DebuffTag);
	// 3. 使用 SetAndApplyTargetTagChanges (这是 5.5 推荐的 API)
	TargetTagsComponent.SetAndApplyTargetTagChanges(TagContainer);
	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	FGameplayModifierInfo ModifierInfo = FGameplayModifierInfo();
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAuraAttributeSet::GetInComingDamageAttribute();
	Effect->Modifiers.Add(ModifierInfo);
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect,EffectContextHandle,1.0f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable<FGameplayTag>(new FGameplayTag(DamageType)); 
		AuraContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
	
}

void UAuraAttributeSet::HandleInComingDamage(FEffectProperties& Props)
{
	float ComingDamage = GetInComingDamage();
	SetInComingDamage(0.f);
	if (ComingDamage>0.f)
	{
		float NewHealth = GetHealth() - ComingDamage;
		SetHealth(FMath::Clamp(NewHealth,0.f,GetMaxHealth()));
		const bool bFatal = NewHealth<=0.f;
		if (bFatal)
		{
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				const FVector DeathImpulse = UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle);
				CombatInterface->Die(DeathImpulse);
			}
			SendXPEvent(Props);
		}
		else
		{
			if (Props.TargetCharacter->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsBeingShocked(Props.TargetCharacter))
			{
				FGameplayTagContainer TagContainer;
				TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
				Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);
			}		
			const FVector KnockbackImpulse = UAuraAbilitySystemLibrary::GetKnockbackImpulse(Props.EffectContextHandle);
			if (!KnockbackImpulse.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackImpulse,true,true);
			}
		}
		if (Props.SourceCharacter && Props.TargetCharacter && Props.SourceCharacter!=Props.TargetCharacter)
		{
			if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller))
			{
				const bool bBlock = UAuraAbilitySystemLibrary::IsBlockHit(Props.EffectContextHandle);
				const bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
				AuraPlayerController->ShowDamageNumber(ComingDamage,Props.TargetCharacter,bBlock,bCritical);
			}
			if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
			{
				const bool bBlock = UAuraAbilitySystemLibrary::IsBlockHit(Props.EffectContextHandle);
				const bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);
				AuraPlayerController->ShowDamageNumber(ComingDamage,Props.TargetCharacter,bBlock,bCritical);
			}
			if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
			{
				Debuff(Props);
			}
		}
	}
}

void UAuraAttributeSet::HandleInComingXP(FEffectProperties& Props)
{
	const float LocalIncomingXP = GetInComingXP();
	SetInComingXP(0.f);
	if (Props.SourceCharacter->Implements<UPlayerInterface>() && Props.SourceCharacter->Implements<UCombatInterface>())
	{
		int32 CurrentLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		int32 CurrentXP =IPlayerInterface::Execute_GetXP(Props.SourceCharacter);

		int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter,CurrentXP + LocalIncomingXP);
		int32 NumLevelUps = NewLevel - CurrentLevel;
		if (NumLevelUps>0)
		{
			int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(Props.SourceCharacter,CurrentLevel,NewLevel);
			int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(Props.SourceCharacter,CurrentLevel,NewLevel);
			IPlayerInterface::Execute_AddToLevel(Props.SourceCharacter,NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter,AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter,SpellPointsReward);
			bChangedMaxHealth = true;
			bChangedMaxMana = true;
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}
		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter,LocalIncomingXP);
	}
}


void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Health,OldHealth);
}

void UAuraAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxHealth,OldMaxHealth);
}

void UAuraAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Mana,OldMana);
}

void UAuraAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,MaxMana,OldMaxMana);
}

void UAuraAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Strength,OldStrength);
}

void UAuraAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Intelligence,OldIntelligence);
}

void UAuraAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Resilience,OldResilience);
}

void UAuraAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Vigor,OldVigor);
}

void UAuraAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldArmor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,Armor,OldArmor);
}

void UAuraAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArmorPenetration,OldArmorPenetration);
}

void UAuraAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,BlockChance,OldBlockChance);
}

void UAuraAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitChance,OldCriticalHitChance);
}

void UAuraAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitDamage,OldCriticalHitDamage);
}

void UAuraAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,CriticalHitResistance,OldCriticalHitResistance);
}

void UAuraAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,HealthRegeneration,OldHealthRegeneration);
}

void UAuraAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ManaRegeneration,OldManaRegeneration);
}

void UAuraAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,FireResistance,OldFireResistance);
}

void UAuraAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldLightningResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,LightningResistance,OldLightningResistance);
}

void UAuraAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,ArcaneResistance,OldArcaneResistance);
}

void UAuraAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet,PhysicalResistance,OldPhysicalResistance);
}


