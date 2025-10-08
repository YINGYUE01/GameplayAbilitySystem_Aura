// Copyright YING


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
void FAuraGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Armor"),FString("Reduce damage taken,improves Block Chance"));
}
