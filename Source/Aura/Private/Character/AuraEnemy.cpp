// Copyright YING


#include "Character/AuraEnemy.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Aura/Aura.h"
#include "Components/WidgetComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AAuraEnemy::AAuraEnemy()
{
	SetReplicates(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Full);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
	HealthBar->SetDrawSize(FVector2D(300.f, 50.f));
	HealthBar->SetVisibility(true);
	HealthBar->SetIsReplicated(true);
}

void AAuraEnemy::HighlightActor()
{
	bHighlight = true;
	GetMesh()->SetRenderCustomDepth(bHighlight);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	Weapon->SetRenderCustomDepth(bHighlight);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	bHighlight = false;
	GetMesh()->SetRenderCustomDepth(bHighlight);
	Weapon->SetRenderCustomDepth(bHighlight);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
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
        
		// 广播初始值
		OnHealthChanged.Broadcast(AuraSet->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraSet->GetMaxHealth());
	}
}

void AAuraEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttribute(this,CharacterClass,Level,AbilitySystemComponent);
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	BindUI();
}

void AAuraEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	InitializeDefaultAttributes();
}  
