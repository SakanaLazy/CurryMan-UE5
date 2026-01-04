#include "HealthSystem_Prototype.h"
#include "TimerManager.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "NavigationSystem.h"

UHealthSystem_Prototype::UHealthSystem_Prototype()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHealthSystem_Prototype::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	if (AActor* Owner = GetOwner())
	{
		MeshRef = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshRef)
		{
			AnimInstance = MeshRef->GetAnimInstance();
		}
	}

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UHealthSystem_Prototype::TakeDamage(int32 Damage)
{
	if (bIsDead) return;

	int32 Before = CurrentHealth;
	CurrentHealth = FMath::Max(0, CurrentHealth - Damage);

	UE_LOG(LogTemp, Log, TEXT("%s HP: %d ? %d"),
		*GetOwner()->GetName(), Before, CurrentHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	// Hit reaction (NPCs only)
	if (CurrentHealth > 0)
	{
		if (bEnableHitReactions && !bIsPlayer)
		{
			PlayHitReaction();
		}
	}
	else if (!bKOSequenceStarted)
	{
		BeginKO();
	}
}

void UHealthSystem_Prototype::PlayHitReaction()
{
	if (!AnimInstance) return;

	AnimInstance->ResetTrigger(*HitTrigger);
	AnimInstance->SetTrigger(*HitTrigger);

	// Unity version temporarily sped animator; UE equivalent is omitted
}

void UHealthSystem_Prototype::BeginKO()
{
	bKOSequenceStarted = true;
	bIsDead = true;

	FTimerHandle KOTimer;
	GetWorld()->GetTimerManager().SetTimer(
		KOTimer,
		this,
		&UHealthSystem_Prototype::Die,
		0.3f,
		false
	);
}

void UHealthSystem_Prototype::Die()
{
	UE_LOG(LogTemp, Log, TEXT("%s entering KO sequence"), *GetOwner()->GetName());

	if (AnimInstance && !DeathTrigger.IsEmpty())
	{
		AnimInstance->ResetTrigger(*DeathTrigger);
		AnimInstance->SetTrigger(*DeathTrigger);
	}

	// Disable known components (same intent as Unity)
	DisableIfExists<class UCombatController_Prototype>();
	DisableIfExists<class UActorComponent>(); // placeholder parity

	// Disable physics if present
	if (UPrimitiveComponent* Prim =
		GetOwner()->FindComponentByClass<UPrimitiveComponent>())
	{
		Prim->SetSimulatePhysics(false);
		Prim->SetPhysicsLinearVelocity(FVector::ZeroVector);
		Prim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	}

	// Notify restart system if present
	if (AActor* Owner = GetOwner())
	{
		UFunction* Fn = Owner->FindFunction(TEXT("OnCharacterDied"));
		if (Fn)
		{
			Owner->ProcessEvent(Fn, this);
		}
	}
}

template<typename T>
void UHealthSystem_Prototype::DisableIfExists()
{
	if (AActor* Owner = GetOwner())
	{
		if (T* Comp = Owner->FindComponentByClass<T>())
		{
			Comp->Deactivate();
		}
	}
}
