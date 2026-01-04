#include "NPCChase_Prototype.h"
#include "NPCCombat_Prototype.h"
#include "HealthSystem_Prototype.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"

UNPCChase_Prototype::UNPCChase_Prototype()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UNPCChase_Prototype::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	CombatComp = Owner->FindComponentByClass<UNPCCombat_Prototype>();
	HealthComp = Owner->FindComponentByClass<UHealthSystem_Prototype>();

	if (USkeletalMeshComponent* Mesh = Owner->FindComponentByClass<USkeletalMeshComponent>())
	{
		AnimInstance = Mesh->GetAnimInstance();
	}

	// Auto-find player by tag
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Players);
	if (Players.Num() > 0)
	{
		PlayerActor = Players[0];
	}

	// Unity InvokeRepeating equivalent
	GetWorld()->GetTimerManager().SetTimer(
		ChaseTimerHandle,
		this,
		&UNPCChase_Prototype::UpdateChase,
		UpdateRate,
		true
	);
}

void UNPCChase_Prototype::UpdateChase()
{
	AActor* Owner = GetOwner();
	if (!Owner || !PlayerActor) return;

	// Stop logic if dead
	if (HealthComp && HealthComp->bIsDead)
	{
		if (AAIController* AI = Cast<AAIController>(Owner->GetInstigatorController()))
		{
			AI->StopMovement();
		}

		if (AnimInstance)
		{
			AnimInstance->SetBoolParameter(TEXT("isWalking"), false);
		}

		if (CombatComp)
			CombatComp->Deactivate();

		return;
	}

	float Dist = FVector::Dist(
		Owner->GetActorLocation(),
		PlayerActor->GetActorLocation()
	);

	AAIController* AI = Cast<AAIController>(Owner->GetInstigatorController());
	if (!AI) return;

	// === CHASE ===
	if (Dist <= ChaseRange && Dist > StopRange)
	{
		AI->MoveToActor(PlayerActor);

		if (AnimInstance)
			AnimInstance->SetBoolParameter(TEXT("isWalking"), true);

		if (CombatComp)
			CombatComp->Deactivate();
	}
	// === STOP & PUNCH ===
	else if (Dist <= StopRange)
	{
		AI->StopMovement();

		if (AnimInstance)
			AnimInstance->SetBoolParameter(TEXT("isWalking"), false);

		// Face player
		FVector Dir = PlayerActor->GetActorLocation() - Owner->GetActorLocation();
		Dir.Z = 0.f;
		if (!Dir.IsNearlyZero())
		{
			FRotator TargetRot = Dir.Rotation();
			Owner->SetActorRotation(
				FMath::RInterpTo(
					Owner->GetActorRotation(),
					TargetRot,
					UpdateRate,
					10.f
				)
			);
		}

		if (CombatComp && !CombatComp->IsActive())
			CombatComp->Activate();
	}
	// === TOO FAR ===
	else
	{
		AI->StopMovement();

		if (AnimInstance)
			AnimInstance->SetBoolParameter(TEXT("isWalking"), false);

		if (CombatComp)
			CombatComp->Deactivate();
	}
}
