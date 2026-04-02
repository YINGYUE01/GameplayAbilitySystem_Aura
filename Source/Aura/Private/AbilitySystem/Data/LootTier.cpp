// Copyright YING


#include "AbilitySystem/Data/LootTier.h"

TArray<FLootItem> ULootTier::GetLootItems()
{
	TArray<FLootItem> ReturnLootItems;
	for (auto& LootItem : LootItems)
	{
		for (int i =0;i<LootItem.MaxSpawnLootItemNums;++i)
		{
			if (FMath::FRandRange(0.f,100.f)<=LootItem.SpawnLootItemChance)
			{
				FLootItem NewLootItem;
				NewLootItem.SpawnLootItemChance = LootItem.SpawnLootItemChance;
				NewLootItem.MaxSpawnLootItemNums = LootItem.MaxSpawnLootItemNums;
				NewLootItem.LootItemClass = LootItem.LootItemClass;
				NewLootItem.bLootLevelOverride = LootItem.bLootLevelOverride;
				ReturnLootItems.Add(NewLootItem);
			}
		}
	}
	return ReturnLootItems;
}
