// Copyright YING

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */

struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeTags();
	FGameplayTag Attribute_Secondary_Armor;
protected:

private:
	static FAuraGameplayTags GameplayTags;
};