#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerController_Prototype.generated.h"

UCLASS()
class APlayerController_Prototype : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerController_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// === Movement Settings ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed = 600.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 720.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float JumpForce = 420.f;

	// === References ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	USkeletalMeshComponent* MeshRef;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	// ?? Combat can toggle this
	UPROPERTY(BlueprintReadWrite, Category = "State")
	bool bMovementLocked = false;

private:
	void HandleMovement(float DeltaTime);
	void HandleJump();

	bool bIsGrounded = false;
};
