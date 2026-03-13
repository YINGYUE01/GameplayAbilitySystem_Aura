// Copyright YING


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeTags()
{
	/*  AttributeTag  */
	//Primary
	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),FString("Increase physical damage"));
	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),FString("Increase magical damage"));
	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),FString("Increase Armor and Armor Penetration"));
	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),FString("Increase Health"));
	//Secondary
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"),FString("Reduce damage taken,improves Block Chance"));
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),FString("Max Health "));
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),FString("Max Mana "));
	GameplayTags.Attribute_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPenetration"),FString("Ignores Percentage of enemy Armor,increase Critical Hit Chance"));
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),FString("Chance to cut incoming damage in half"));
	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"),FString("Chance to duble plus critical hit bonus"));
	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"),FString("Bonus damage added when a critical hit is enemy"));
	GameplayTags.Attribute_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"),FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"),FString("Health Regenerdted every 1 second"));
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"),FString("Mana Regenerdted every 1 second"));
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),FString("Resistance Fire"));
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"),FString("Resistance Lightning"));
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),FString("Resistance Arcane"));
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),FString("Resistance Physical"));
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"),FString("Incoming XP Attribute"));
	/*  AttributeTag  */

	/*  InputTag  */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"),FString("Input Tag for Left Mouse Button"));
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"),FString("Input Tag for Right Mouse Button"));
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"),FString("Input Tag for 1 Key"));
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"),FString("Input Tag for 2 Key"));
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"),FString("Input Tag for 3 Key"));
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"),FString("Input Tag for 4 Key"));
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.1"),FString("Passive Tag for 1 Key"));
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive.2"),FString("Passive Tag for 2 Key"));
	/*  InputTag  */

	/* Damage */
	GameplayTags.Damage =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),FString("Damage"));
	GameplayTags.Damage_Fire =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),FString("Fire Damage Types"));
	GameplayTags.Damage_Lightning =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"),FString("Lightning Damage Types"));
	GameplayTags.Damage_Arcane =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),FString("Arcane Damage Types"));
	GameplayTags.Damage_Physical =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),FString("Physical Damage Types"));
	// Debuff Damage
	GameplayTags.Debuff_Burn =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"),FString("Debuff Burn Damage"));
	GameplayTags.Debuff_Stun =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"),FString("Debuff Stun Damage"));
	GameplayTags.Debuff_Arcane =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"),FString("Debuff Arcane Damage"));
	GameplayTags.Debuff_Physical =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"),FString("Debuff Physical Damage"));

	GameplayTags.Debuff_Chance =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"),FString("Debuff Chance"));
	GameplayTags.Debuff_Damage =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"),FString("Debuff Damage"));
	GameplayTags.Debuff_Duration =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"),FString("Debuff Duration"));
	GameplayTags.Debuff_Frequency =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Frequency"),FString("Debuff Frequency"));
	//Maps DamageType To Damage
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire,GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Arcane,GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical,GameplayTags.Attributes_Resistance_Physical);

	//Maps DamageType To Debuff Damage
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Arcane,GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Fire,GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Lightning,GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuff.Add(GameplayTags.Damage_Physical,GameplayTags.Debuff_Physical);

	
	/*End Damage */
	
	/* Ability */
	GameplayTags.Effects_HitReact =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"),FString("HitReact"));
	
	GameplayTags.Abilities_Attack =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"),FString("Attack Ability Tag"));
	GameplayTags.Abilities_None =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"),FString("None Ability"));
	GameplayTags.Abilities_Summon =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"),FString("Summon Ability Tag"));
	
	GameplayTags.Abilities_Fire_FireBolt =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"),FString("FireBolt Ability Tag"));
	GameplayTags.Abilities_Lightning_Electrocute=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lightning.Electrocute"),FString("Electrocute Ability"));
	GameplayTags.Abilities_Arcane_ArcaneShards=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Arcane.ArcaneShards"),FString("ArcaneShards Ability"));
	
	GameplayTags.Cooldown_Fire_FireBolt=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"),FString("FireBolt Cooldown Tag"));
	GameplayTags.Cooldown_None=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.None"),FString("None Cooldown Tag"));
	
	GameplayTags.Abilities_HitReact=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"),FString("HitReact Ability"));
	GameplayTags.Abilities_Status_Eligible=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"),FString("Eligible Ability"));
	GameplayTags.Abilities_Status_Equipped=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"),FString("Equipped Ability"));
	GameplayTags.Abilities_Status_Locked=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Locked"),FString("Locked Ability"));
	GameplayTags.Abilities_Status_UnLocked=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.UnLocked"),FString("UnLocked Ability"));
	
	GameplayTags.Abilities_Type_Offensive=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"),FString("Offensive Type"));
	GameplayTags.Abilities_Type_Passive=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"),FString("Passive Type"));
	
	GameplayTags.Abilities_Passive_HaloOfProtection=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.HaloOfProtection"),FString("Passive HaloOfProtection"));
	GameplayTags.Abilities_Passive_LifeSiphon=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.LifeSiphon"),FString("Passive LifeSiphon"));
	GameplayTags.Abilities_Passive_ManaSiphon=  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Passive.ManaSiphon"),FString("Passive ManaSiphon"));
	
	/*End Ability */
	
	/* CombatSocket */
	GameplayTags.CombatSocket_Weapon =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"),FString("Weapon"));
	GameplayTags.CombatSocket_RightHand =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"),FString("RightHand"));
	GameplayTags.CombatSocket_LeftHand =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"),FString("LeftHand"));
	GameplayTags.CombatSocket_Tail =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"),FString("Tail"));
	/*End CombatSocket */
	
	/* MontageAttack */
	GameplayTags.Montage_Attack_1 =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"),FString("Attack_1"));
	GameplayTags.Montage_Attack_2 =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"),FString("Attack_2"));
	GameplayTags.Montage_Attack_3 =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"),FString("Attack_3"));
	GameplayTags.Montage_Attack_4 =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"),FString("Attack_4"));
	/*End MontageAttack */
	/* BlockTag */
	GameplayTags.Player_Block_InputPressed =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputPressed"),FString("Block Input Pressed callback for Input"));
	GameplayTags.Player_Block_InputHeld =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputHeld"),FString("Block Input Held callback for Input"));
	GameplayTags.Player_Block_InputReleased =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.InputReleased"),FString("Block Input Released callback for Input"));
	GameplayTags.Player_Block_CursorTrace =  UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Player.Block.CursorTrace"),FString("Block CursorTrace"));
	/*End BlockTag */
	
}	
