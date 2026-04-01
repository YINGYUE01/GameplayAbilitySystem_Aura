// Copyright YING


#include "Actor/SpawnEnemyPoint.h"

#include "Character/AuraEnemy.h"

ASpawnEnemyPoint::ASpawnEnemyPoint()
{

}

void ASpawnEnemyPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass,GetActorTransform());
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(EnemyCharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
