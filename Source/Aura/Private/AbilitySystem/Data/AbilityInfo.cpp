// Copyright YING


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, bool bLogNotFind) const
{
	for (const FAuraAbilityInfo& Info : AbilityInformation)
	{
		if (Info.AbilityTag == AbilityTag)
		{
			return Info;
		}
	}
	if (bLogNotFind)
	{
		UE_LOG(LogAura, Error,TEXT("Can`t Find Info for AbilityTag [%s] on AbilityInfo [%s]"),*AbilityTag.ToString(),*GetNameSafe(this));
	}
	
	return FAuraAbilityInfo();
}
