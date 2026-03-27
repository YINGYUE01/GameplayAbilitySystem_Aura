// Copyright YING


#include "Game/LoadScreenSaveGame.h"

FSaveMap ULoadScreenSaveGame::GetSaveMapWithMapName(const FString& InMapName)
{
	for (const FSaveMap& Map : SaveMaps)
	{
		if (Map.MapName == InMapName)
		{
			return Map;
		}
	}
	return FSaveMap();
}

bool ULoadScreenSaveGame::HarMap(const FString& InMapName)
{
	for (const FSaveMap& Map : SaveMaps)
	{
		if (Map.MapName == InMapName)
		{
			return true;
		}
	}
	return false;
}
