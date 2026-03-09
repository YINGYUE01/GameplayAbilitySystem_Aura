// Copyright YING


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	BurnDebuffNiagaraComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffNiagaraComponent");
	BurnDebuffNiagaraComponent->SetupAttachment(GetRootComponent());
	BurnDebuffNiagaraComponent->DebuffTag = FAuraGameplayTags::Get().Debuff_Burn;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
}

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraCharacterBase,Stunned);
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag MontageTag)
{
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Weapon))
	{
		check(Weapon);
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_LeftHand))
		return GetMesh()->GetSocketLocation(LeftSocketName);
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_RightHand))
		return GetMesh()->GetSocketLocation(RightSocketName);
	if (MontageTag.MatchesTagExact(FAuraGameplayTags::Get().CombatSocket_Tail))
		return GetMesh()->GetSocketLocation(TailSocketName);
	return FVector();
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitMontage_Implementation()
{
	
	return HitReactMontage;
}


void AAuraCharacterBase::Die(const FVector DeathImpulse)
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld,true));
	MulticastHandleDeath(DeathImpulse);
}

bool AAuraCharacterBase::bIsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

bool AAuraCharacterBase::IsDead_Implementation()
{
	return bDead;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (auto TaggedMontage : AttackMontages)
	{
		if (TaggedMontage.MontageTag==MontageTag)
			return TaggedMontage;
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount+=Amount;
}

ECharacterClass AAuraCharacterBase::GetCharacterCLass_Implementation()
{
	return CharacterClass;
}

FOnASCRegister AAuraCharacterBase::GetOnASCRegisterDelegate()
{
	return OnAscRegister;
}

FOnDeath& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeath;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeaponMesh_Implementation()
{
	return Weapon;
}

void AAuraCharacterBase::OnRep_Stuned()
{
	
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Stunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = Stunned ? 0 : BaseWalkSpeed;
	
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation(const FVector DeathImpulse)
{
	UGameplayStatics::PlaySoundAtLocation(this,DeathSound,GetActorLocation(),GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	Weapon->AddImpulse(DeathImpulse*0.1,NAME_None,true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	GetMesh()->AddImpulse(DeathImpulse,NAME_None,true);
	Dissolve();
	bDead = true;
	OnDeath.Broadcast(this);
}
void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(DissolveMaterialInstance,this);
		GetMesh()->SetMaterial(0,DynamicMatIns);
		StartDissolveTimeline(DynamicMatIns);
	}
	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance,this);
		Weapon->SetMaterial(0,DynamicMatIns);
		WeaponStartDissolveTimeline(DynamicMatIns);
	}
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,Level,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryAttributes,1);
	ApplyEffectToSelf(DefaultVitalAttributes,1);
}

void AAuraCharacterBase::AddCharacterAbilities() 
{
	UAuraAbilitySystemComponent* AuraASC = CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return ;
	//StartupAbilities是每个角色的初始技能在这里转到ASC中去赋予初始技能
	AuraASC->AddCharacterAbilities(StartupAbilities);
	AuraASC->AddCharacterPassiveAbilities(StartupPassiveAbilities);
}







