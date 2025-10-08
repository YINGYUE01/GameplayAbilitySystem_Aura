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
	static const FGameplayTagContainer& Get() { return GameplayTags; }
	static void InitializeNativeTags();
protected:

private:
	static FGameplayTagContainer GameplayTags;
};