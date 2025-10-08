// Copyright YING


#include "AbilitySystem/Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfo(const FGameplayTag& AttributeTag,bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag)
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("FAuraAttributeInfo::FindAttributeInfo: AttributeTag %s not found!"), *AttributeTag.GetTagName().ToString());
	}
	return FAuraAttributeInfo();
}
