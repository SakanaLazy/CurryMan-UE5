#include "PlayerController_Prototype.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

APlayerController_Prototype::APlayerController_Prototype()
{
	PrimaryActorTick.bCanEverTick = true;

	// Default movement values similar to Unity prototype
	GetCharacterMovement()->GravityScale = 1.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->JumpZVelocity = JumpForce;
}

void APlayerController_Prototype::BeginPlay()
{
	Super::BeginPlay();

	MeshRef = GetMesh();
	if (MeshRef)
	{
		AnimInstance = MeshRef->GetAnimInstance();
	}

	// Default walk speed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void APlayerController_Prototype::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleMovement(DeltaTime);
	HandleJump();
}

void APlayerController_Prototype::HandleMovement(float DeltaTime)
{
	if (bMovementLocked)
	{
		if (AnimInstance)
		{
			AnimInstance->SetFloatParameter(TEXT("Speed"), 0.f);
		}
		return;
	}

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	float Forward = 0.f;
	float Right = 0.f;

	// === Keyboard input (raw, Unity-style polling) ===
	if (PC->IsInputKeyDown(EKeys::W)) Forward += 1.f;
	if (PC->IsInputKeyDown(EKeys::S)) Forward -= 1.f;
	if (PC->IsInputKeyDown(EKeys::D)) Right += 1.f;
	if (PC->IsInputKeyDown(EKeys::A)) Right -= 1.f;

	FVector InputDir = FVector(Forward, Right, 0.f);
	InputDir = InputDir.GetClampedToMaxSize(1.f);

	bool bRunning = PC->IsInputKeyDown(EKeys::LeftShift);
	float CurrentSpeed = bRunning ? RunSpeed : WalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = CurrentSpeed;

	if (!InputDir.IsNearlyZero())
	{
		AddMovementInput(GetActorForwardVector(), InputDir.X);
		AddMovementInput(GetActorRightVector(), InputDir.Y);

		// Rotate toward movement direction
		FRotator TargetRot = InputDir.Rotation();
		FRotator NewRot = FMath::RInterpConstantTo(
			GetActorRotation(),
			TargetRot,
			DeltaTime,
			RotationSpeed
		);

		SetActorRotation(NewRot);

		if (AnimInstance)
		{
			AnimInstance->SetFloatParameter(TEXT("Speed"), CurrentSpeed);
		}
	}
	else
	{
		if (AnimInstance)
		{
			AnimInstance->SetFloatParameter(TEXT("Speed"), 0.f);
		}
	}
}

void APlayerController_Prototype::HandleJump()
{
	if (bMovementLocked) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	bIsGrounded = GetCharacterMovement()->IsMovingOnGround();

	if (PC->WasInputKeyJustPressed(EKeys::SpaceBar) && bIsGrounded)
	{
		Jump();

		if (AnimInstance)
		{
			AnimInstance->Montage_Play(nullptr); // placeholder for Jump trigger
			AnimInstance->SetTriggerParameter(TEXT("Jump"));
		}
	}
}
