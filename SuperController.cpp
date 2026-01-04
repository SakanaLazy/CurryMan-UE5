#include "SuperController_Prototype.h"
#include "HitDetector_Prototype.h"
#include "CombatController_Prototype.h"
#include "PlayerController_Prototype.h"
#include "SuperMeterSystem_Prototype.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

USuperController_Prototype::USuperController_Prototype()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USuperController_Prototype::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		MeshRef = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshRef)
			AnimInstance = MeshRef->GetAnimInstance();

		CombatController = Owner->FindComponentByClass<UCombatController_Prototype>();
		PlayerControllerRef = Cast<APlayerController_Prototype>(Owner);
	}
}

void USuperController_Prototype::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	if (!bIsSuper)
	{
		if (PC->WasInputKeyJustPressed(EKeys::Q))
			StartSuper(1);
		else if (PC->WasInputKeyJustPressed(EKeys::E))
			StartSuper(2);
		return;
	}

	SuperElapsed += DeltaTime;

	if (!IsInSuperState() && SuperElapsed > 0.1f)
	{
		EndSuper();
		return;
	}

	if (SuperElapsed >= SuperTimeout)
	{
		EndSuper();
		return;
	}

	ProcessSuperHits();
}

void USuperController_Prototype::StartSuper(int32 Index)
{
	if (bIsSuper || !AnimInstance) return;

	bIsSuper = true;
	ActiveSuperIndex = Index;
	SuperElapsed = 0.f;
	SuperTimeout = LockFallbackSeconds + 1.f;

	if (CombatController)
		CombatController->Deactivate();

	if (PlayerControllerRef)
		PlayerControllerRef->bMovementLocked = true;

	// Mirror randomization
	if (bUseMirrorParam)
	{
		bool bMirror = FMath::RandBool();
		AnimInstance->SetBoolParameter(*MirrorParam, bMirror);
	}

	const FString& Trigger =
		(Index == 1) ? Super1Trigger : Super2Trigger;

	// Store & scale damage
	OriginalDamages.Empty();
	for (UHitDetector_Prototype* HD : HitDetectors)
	{
		if (!HD) continue;
		OriginalDamages.Add(HD->Damage);
		HD->Damage = FMath::RoundToInt(HD->Damage * DamageMultiplier);
	}

	AnimInstance->ResetTrigger(*Trigger);
	AnimInstance->SetTrigger(*Trigger);

	SpawnSuperFX();
}

bool USuperController_Prototype::IsInSuperState() const
{
	if (!AnimInstance) return false;

	const FString A =
		(ActiveSuperIndex == 1) ? Super1State : Super2State;
	const FString B =
		(ActiveSuperIndex == 1) ? Super1MirrorState : Super2MirrorState;

	return AnimInstance->IsAnyMontagePlaying() ||
		AnimInstance->GetCurrentActiveMontage() != nullptr;
}

void USuperController_Prototype::ProcessSuperHits()
{
	for (UHitDetector_Prototype* HD : HitDetectors)
	{
		if (!HD) continue;

		FVector Center = HD->GetOwner()->GetActorLocation();
		TArray<AActor*> Hits;

		UKismetSystemLibrary::SphereOverlapActors(
			GetWorld(),
			Center,
			OverlapRadius,
			{},
			AActor::StaticClass(),
			{ GetOwner() },
			Hits
		);

		for (AActor* Other : Hits)
		{
			if (!Other) continue;

			UHealthSystem_Prototype* Health =
				Other->FindComponentByClass<UHealthSystem_Prototype>();

			if (Health && !Health->bIsDead)
			{
				Health->TakeDamage(HD->Damage);
				if (AAudioManager_Prototype::Instance)
					AAudioManager_Prototype::Instance->PlaySuper();
			}
		}
	}
}

void USuperController_Prototype::EndSuper()
{
	bIsSuper = false;

	// Restore damage
	for (int32 i = 0; i < HitDetectors.Num(); i++)
	{
		if (HitDetectors[i] && OriginalDamages.IsValidIndex(i))
			HitDetectors[i]->Damage = OriginalDamages[i];
	}

	if (bUseMirrorParam && AnimInstance)
		AnimInstance->SetBoolParameter(*MirrorParam, false);

	if (CombatController)
		CombatController->Activate();

	if (PlayerControllerRef)
		PlayerControllerRef->bMovementLocked = false;

	// Reset meter
	if (USuperMeterSystem_Prototype* Meter =
		GetOwner()->FindComponentByClass<USuperMeterSystem_Prototype>())
	{
		Meter->ResetMeter();
	}
}

void USuperController_Prototype::SpawnSuperFX()
{
	if (!SuperBurstFX) return;

	FVector SpawnPos = GetOwner()->GetActorLocation() + FXOffset;
	GetWorld()->SpawnActor<AActor>(SuperBurstFX, SpawnPos, FRotator::ZeroRotator);
}
