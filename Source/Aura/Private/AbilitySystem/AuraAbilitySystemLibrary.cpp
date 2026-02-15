// Copyright YING


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Character/AuraEnemy.h"
#include "Interaction/CombatInterface.h"


bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject,
	FWidgetControllerParams& OutWCParams, AAuraHUD*& AuraHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		AuraHUD =  Cast<AAuraHUD>(PC->GetHUD());
		if (AuraHUD)
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			OutWCParams.PlayerState = PS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.AttributeSet = AS;
			OutWCParams.PlayerController = PC;
			return true;
		}
	}
	return false;
}


UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetAttributeMenuWidgetController(WCParams);
	}
	return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	AAuraHUD* AuraHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject,WCParams,AuraHUD))
	{
		return AuraHUD->GetSpellMenuWidgetController(WCParams);
	}
	return nullptr;
	
}


void UAuraAbilitySystemLibrary::InitializeDefaultAttribute(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level,UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterClassInfo is null in InitializeDefaultAttribute"));
		return;
	}

	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	if (ClassDefaultInfo.PrimaryAttributes == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("PrimaryAttributes is null for CharacterClass %d"), static_cast<int32>(CharacterClass));
		return;
	}

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	FGameplayEffectContextHandle PrimaryContextHandle = ASC->MakeEffectContext();
	PrimaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryEffectSpecHandle.Data.Get());

	if (CharacterClassInfo->SecondaryAttributes == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SecondaryAttributes is null in CharacterClassInfo"));
		return;
	}

	FGameplayEffectContextHandle  SecondaryContextHandle = ASC->MakeEffectContext();
	SecondaryContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,SecondaryContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryEffectSpecHandle.Data.Get());

	if (CharacterClassInfo->VitalAttributes == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("VitalAttributes is null in CharacterClassInfo"));
		return;
	}

	FGameplayEffectContextHandle  VitalContextHandle = ASC->MakeEffectContext();
	VitalContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalEffectSpecHandle.Data.Get());
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, float Level,
	UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterClassInfo is null in GiveStartupAbilities"));
		return;
	}
	
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		if (AbilityClass != nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,Level);
			ASC->GiveAbility(AbilitySpec);
		}
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
	
}
int32 UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass, const int32 Level)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("CharacterClassInfo is null in GiveStartupAbilities"));
		return 0;
	}
	FCharacterClassDefaultInfo Info = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	float XPReward = Info.XPReward.GetValueAtLevel(Level);
	return static_cast<int32>(XPReward);
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr ;

	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return AuraEffectContext->IsBlockHit();
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return AuraEffectContext->IsCriticalHit();
	return false;
}

void UAuraAbilitySystemLibrary::SetBlockHit(FGameplayEffectContextHandle& EffectContextHandle, bool bBlockHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraEffectContext->SetBlockHit(bBlockHit);
}

void UAuraAbilitySystemLibrary::SetCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bCriticalHit)
{
	if (FAuraGameplayEffectContext* AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraEffectContext->SetCriticalHit(bCriticalHit);
}

void UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps,SphereOrigin,FQuat::Identity,FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),FCollisionShape::MakeSphere(Radius),SphereParams);
		for (FOverlapResult& OverlapResult : Overlaps)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_bIsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(OverlapResult.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool BothArePlayers = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool BothAreEnemies = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool AreFriends = BothArePlayers || BothAreEnemies;
	return !AreFriends;
}

