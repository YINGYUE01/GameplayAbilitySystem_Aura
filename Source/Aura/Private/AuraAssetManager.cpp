// Copyright YING


#include "AuraAssetManager.h"
#include "AuraGameplayTags.h"
#include "AbilitySystemGlobals.h"
UAuraAssetManager& UAuraAssetManager::Get()
{
	check(GEngine);
	UAuraAssetManager* AuraAssetManager = CastChecked<UAuraAssetManager>(GEngine->AssetManager);
	return *AuraAssetManager;
}

void UAuraAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FAuraGameplayTags::InitializeNativeTags();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
