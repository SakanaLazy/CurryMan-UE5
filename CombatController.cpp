#include "CombatController_Prototype.h"
#include "PlayerController_Prototype.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

UCombatController_Prototype::UCombatController_Prototype()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatController_Prototype::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner = GetOwner())
	{
		MeshRef = Owner->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshRef)
		{
			AnimInstance = MeshRef->GetAnimInstance();
		}

		PlayerControllerRef = Cast<APlayerController_Prototype>(Owner);
	}
}

void UCombatController_Prototype::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	HandleBufferedInput(DeltaTime);

	// click window timing
	if (ClickTimer > 0.f)
	{
		ClickTimer -= DeltaTime;
		if (ClickTimer <= 0.f)
			ClickCount = 0;
	}
}

void UCombatController_Prototype::HandleBufferedInput(float DeltaTime)
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (!PC) return;

	// count clicks
	if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton) ||
		PC->WasInputKeyJustPressed(EKeys::RightMouseButton))
	{
		ClickCount++;
		ClickTimer = ClickWindow;

		if (ClickCount > MaxClicksPerSequence)
			return;
	}

	// buffer inputs
	if (PC->WasInputKeyJustPressed(EKeys::LeftMouseButton))
	{
		bBufferedPunch = true;
		PunchBufferTimer = BufferDuration;
	}

	if (PC->WasInputKeyJustPressed(EKeys::RightMouseButton))
	{
		bBufferedKick = true;
		KickBufferTimer = BufferDuration;
	}

	// consume buffers
	if (!bIsAttacking)
	{
		if (bBufferedPunch)
		{
			bBufferedPunch = false;
			StartCombo(PunchTriggers);
		}
		else if (bBufferedKick)
		{
			bBufferedKick = false;
			StartCombo(KickTriggers);
		}
	}
	else if (bCanQueueNext)
	{
		if (bBufferedPunch)
		{
			bBufferedPunch = false;
			QueueNextAttack(PunchTriggers);
		}
		else if (bBufferedKick)
		{
			bBufferedKick = false;
			QueueNextAttack(KickTriggers);
		}
	}

	// buffer timers
	if (PunchBufferTimer > 0.f)
	{
		PunchBufferTimer -= DeltaTime;
		if (PunchBufferTimer <= 0.f)
			bBufferedPunch = false;
	}

	if (KickBufferTimer > 0.f)
	{
		KickBufferTimer -= DeltaTime;
		if (KickBufferTimer <= 0.f)
			bBufferedKick = false;
	}
}

void UCombatController_Prototype::StartCombo(const TArray<FString>& Triggers)
{
	ComboStep = 0;
	ExecuteComboStep(Triggers);
}

void UCombatController_Prototype::QueueNextAttack(const TArray<FString>& Triggers)
{
	ComboStep++;
	ExecuteComboStep(Triggers);
}

void UCombatController_Prototype::ExecuteComboStep(const TArray<FString>& Triggers)
{
	if (!AnimInstance || Triggers.Num() == 0) return;

	bIsAttacking = true;
	bCanQueueNext = false;

	if (PlayerControllerRef)
		PlayerControllerRef->bMovementLocked = true;

	ComboStep = FMath::Clamp(ComboStep, 0, Triggers.Num() - 1);
	const FString& Trigger = Triggers[ComboStep];

	AnimInstance->ResetTrigger(*Trigger);
	AnimInstance->SetTrigger(*Trigger);

	float ClipLength = GetAnimationLength(Trigger);

	GetWorld()->GetTimerManager().SetTimer(
		ComboTimerHandle,
		this,
		&UCombatController_Prototype::ResetCombo,
		ClipLength + ComboResetTime,
		false
	);

	// open queue window near the end
	FTimerHandle QueueWindowHandle;
	GetWorld()->GetTimerManager().SetTimer(
		QueueWindowHandle,
		[this]()
		{
			bCanQueueNext = true;
		},
		FMath::Max(0.f, ClipLength - ComboInputWindow),
		false
	);
}

void UCombatController_Prototype::ResetCombo()
{
	bIsAttacking = false;
	bCanQueueNext = false;
	ComboStep = 0;

	if (PlayerControllerRef)
		PlayerControllerRef->bMovementLocked = false;

	ClickCount = 0;
	ClickTimer = 0.f;
}

float UCombatController_Prototype::GetAnimationLength(const FString& Trigger) const
{
	if (!MeshRef || !MeshRef->GetAnimInstance()) return 0.6f;

	const UAnimInstance* Anim = MeshRef->GetAnimInstance();
	const UAnimBlueprintGeneratedClass* BPClass =
		Cast<UAnimBlueprintGeneratedClass>(Anim->GetClass());

	if (!BPClass) return 0.6f;

	for (const FAnimBlueprintDebugData::FAnimBlueprintAnimationData& Data :
		BPClass->GetAnimBlueprintDebugData().GetAnimationData())
	{
		if (Data.Name.ToString().Contains(Trigger))
		{
			if (Data.Animation)
				return Data.Animation->GetPlayLength();
		}
	}

	return 0.6f;
}
