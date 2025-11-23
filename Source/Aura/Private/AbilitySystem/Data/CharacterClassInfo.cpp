// Copyright YING
#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass Class) const
{
	if (const FCharacterClassDefaultInfo* Info = CharacterClassInformation.Find(Class))
	{
		return *Info;
	}
	
	UE_LOG(LogTemp, Error, TEXT("CharacterClass %d not found in CharacterClassInformation map"), static_cast<int32>(Class));
	return FCharacterClassDefaultInfo();
}
