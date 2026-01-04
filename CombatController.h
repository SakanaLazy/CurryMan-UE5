#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatController_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UCombatController_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	UCombatController_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	// === References ===
	UPROPERTY(EditAnywhere, Category = "References")
	USkeletalMeshComponent* MeshRef;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	UPROPERTY(EditAnywhere, Category = "References")
	class APlayerController_Prototype* PlayerControllerRef;

	// === Combo Settings ===
	UPROPERTY(EditAnywhere, Category = "Combo")
	float ComboResetTime = 0.9f;

	UPROPERTY(EditAnywhere, Category = "Combo")
	float ComboInputWindow = 0.45f;

	// === Animation Triggers ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FString> PunchTriggers;

	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FString> KickTriggers;

	// === Input Buffer ===
	UPROPERTY(EditAnywhere, Category = "Input")
	float BufferDuration = 0.4f;

	// === Anti-Spam ===
	UPROPERTY(EditAnywhere, Category = "Input")
	int32 MaxClicksPerSequence = 3;

	UPROPERTY(EditAnywhere, Category = "Input")
	float ClickWindow = 0.5f;

private:
	// internal state
	int32 ComboStep = 0;
	bool bCanQueueNext = false;
	bool bIsAttacking = false;

	bool bBufferedPunch = false;
	bool bBufferedKick = false;

	float PunchBufferTimer = 0.f;
	float KickBufferTimer = 0.f;

	int32 ClickCount = 0;
	float ClickTimer = 0.f;

	FTimerHandle ComboTimerHandle;

	void HandleBufferedInput(float DeltaTime);
	void StartCombo(const TArray<FString>& Triggers);
	void QueueNextAttack(const TArray<FString>& Triggers);
	void ExecuteComboStep(const TArray<FString>& Triggers);
	void ResetCombo();

	float GetAnimationLength(const FString& Trigger) const;
};
