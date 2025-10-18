// Copyright YING


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(ECharacterClass Class) const
{
	return CharacterClassInformation.FindChecked(Class);
}
