// Copyright YING


#include "Character/AuraEnemy.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"

AAuraEnemy::AAuraEnemy()
{
	SetReplicates(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetDrawSize(FVector2D(300.f, 50.f));
	HealthBar->SetVisibility(true);
	HealthBar->SetIsReplicated(true);
	BaseWalkSpeed = 250.f;
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	GetMesh()->MarkRenderStateDirty();
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->MarkRenderStateDirty();
}

void AAuraEnemy::OnRep_Burned()
{
	if (BurnDebuffNiagaraComponent==nullptr) return;
	if (Burned)
	{
		if (!BurnDebuffNiagaraComponent->IsActive())
			BurnDebuffNiagaraComponent->Activate();
	}
	else
	{
		BurnDebuffNiagaraComponent->Deactivate();
	}
}

void AAuraEnemy::OnRep_Stuned()
{
	if (Stunned)
	{
		StunDebuffNiagaraComponent->Activate();
	}
	else
	{
		StunDebuffNiagaraComponent->Deactivate();
	}
}

void AAuraEnemy::BurnTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::BurnTagChanged(CallbackTag, NewCount);
	
}
void AAuraEnemy::StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	Super::StunTagChanged(CallbackTag, NewCount);
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Stun"),Stunned);
	}
}


void AAuraEnemy::HighlightActor_Implementation()
{
	bHighlight = true;
	GetMesh()->SetRenderCustomDepth(bHighlight);
	Weapon->SetRenderCustomDepth(bHighlight);
}

void AAuraEnemy::UnHighlightActor_Implementation()
{
	bHighlight = false;
	GetMesh()->SetRenderCustomDepth(bHighlight);
	Weapon->SetRenderCustomDepth(bHighlight);
}

void AAuraEnemy::SetMoveToLocation_Implementation(FVector& OutLocation)
{
	
}

AActor* AAuraEnemy::GetCombatTarget_Implementation()
{
	return CombatTarget;
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}


void AAuraEnemy::Die(const FVector DeathImpulse)
{
	SpanLife = 2.5f;
	SetLifeSpan(SpanLife);
	if (AuraAIController)
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"),true);
	if (HealthBar)
		HealthBar->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Super::Die(DeathImpulse);
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority())
		return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),CharacterClass!=ECharacterClass::Warrior);
}
void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallBackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController && AuraAIController->GetBlackboardComponent())
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);
}  
void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	
	// 只在服务器端给予能力，客户端会通过复制接收
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this,Level,AbilitySystemComponent,CharacterClass);
	}
	OnAscRegister.Broadcast(AbilitySystemComponent);
	
	BindUI();
	
}
void AAuraEnemy::BindUI()
{
	if (UAuraUserWidget* AuraWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraWidget->SetWidgetController(this);
	}
    
	UAuraAttributeSet* AuraSet = Cast<UAuraAttributeSet>(AttributeSet);
	if (AuraSet)
	{
		// 绑定属性变化委托
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetHealthAttribute()).AddLambda(
					[this](const FOnAttributeChangeData& Data)
					{
						OnHealthChanged.Broadcast(Data.NewValue);
					});
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraSet->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			});
		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
			);
        
		// 广播初始值
		OnHealthChanged.Broadcast(AuraSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttribute(this,CharacterClass,Level,AbilitySystemComponent);
}


void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraEnemy::StunTagChanged);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Burn,EGameplayTagEventType::NewOrRemoved).AddUObject(this,&AAuraEnemy::BurnTagChanged);
	// 只在服务器端初始化属性，客户端会通过复制接收
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

