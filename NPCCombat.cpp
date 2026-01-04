#include "NPCCombat_Prototype.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"

UNPCCombat_Prototype::UNPCCombat_Prototype()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNPCCombat_Prototype::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		MeshRef = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshRef)
		{
			AnimInstance = MeshRef->GetAnimInstance();
		}
	}

	// Auto-find player by tag
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Player"), Players);
	if (Players.Num() > 0)
	{
		PlayerActor = Players[0];
	}
}

void UNPCCombat_Prototype::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerActor || !GetOwner()) return;

	float Dist = FVector::Dist(
		GetOwner()->GetActorLocation(),
		PlayerActor->GetActorLocation()
	);

	if (Dist <= AttackRange && !bIsAttacking)
	{
		TryStartAttack();
	}
}

void UNPCCombat_Prototype::TryStartAttack()
{
	if (bIsAttacking) return;

	bIsAttacking = true;

	float Delay = bUseRandomDelay
		? FMath::FRandRange(MinAttackDelay, MaxAttackDelay)
		: FixedDelay;

	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&UNPCCombat_Prototype::PerformAttack,
		Delay,
		false
	);
}

void UNPCCombat_Prototype::PerformAttack()
{
	if (!AnimInstance || !PlayerActor)
	{
		bIsAttacking = false;
		return;
	}

	// Face player instantly (Unity coroutine equivalent)
	FVector Dir = PlayerActor->GetActorLocation() - GetOwner()->GetActorLocation();
	Dir.Z = 0.f;

	if (!Dir.IsNearlyZero())
	{
		FRotator TargetRot = Dir.Rotation();
		GetOwner()->SetActorRotation(TargetRot);
	}

	// Choose punch type (1 or 2)
	int32 Chosen = FMath::RandRange(1, 2);
	AnimInstance->SetIntegerParameter(*PunchTypeName, Chosen);

	AnimInstance->ResetTrigger(*PunchTriggerName);
	AnimInstance->SetTrigger(*PunchTriggerName);

	// Fail-safe reset
	GetWorld()->GetTimerManager().SetTimer(
		AttackTimerHandle,
		[this]()
		{
			bIsAttacking = false;
		},
		TriggerFailSafeTime,
		false
	);
}
