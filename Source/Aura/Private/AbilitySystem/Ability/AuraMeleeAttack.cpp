// Copyright YING


#include "AbilitySystem/Ability/AuraMeleeAttack.h"


AAuraWeapon* UAuraMeleeAttack::GetWeaponSkeletalMesh()
{
	AActor* PlayerActor = GetAvatarActorFromActorInfo();
	if (PlayerActor->Implements<UCombatInterface>())
	{
		return ICombatInterface::Execute_GetWeaponMesh(PlayerActor);
	}
	return nullptr;
}
