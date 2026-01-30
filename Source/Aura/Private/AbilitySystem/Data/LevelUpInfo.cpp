// Copyright YING


#include "AbilitySystem/Data/LevelUpInfo.h"

int ULevelUpInfo::GetLevelForXP(const int32 XP)
{
	int Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		if (LevelUpInformation.Num()-1<=Level) return Level;
		if (XP > LevelUpInformation[Level].LevelUpRequiredXP)
		{
			Level++;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}
