#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SuperController_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USuperController_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	USuperController_Prototype();

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
	TArray<class UHitDetector_Prototype*> HitDetectors;

	UPROPERTY(EditAnywhere, Category = "References")
	class UCombatController_Prototype* CombatController;

	UPROPERTY(EditAnywhere, Category = "References")
	class APlayerController_Prototype* PlayerControllerRef;

	// === Super Triggers ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super1Trigger = "Super1";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super2Trigger = "Super2";

	// === Animator States ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super1State = "Super1";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super1MirrorState = "Super1_Mirror";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super2State = "Super2";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString Super2MirrorState = "Super2_Mirror";

	// === Mirror ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	bool bUseMirrorParam = true;

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString MirrorParam = "Mirror";

	// === Timing & Damage ===
	UPROPERTY(EditAnywhere, Category = "Super")
	float LockFallbackSeconds = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Super")
	float DamageMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Super")
	float OverlapRadius = 100.f;

	// === FX ===
	UPROPERTY(EditAnywhere, Category = "FX")
	TSubclassOf<AActor> SuperBurstFX;

	UPROPERTY(EditAnywhere, Category = "FX")
	FVector FXOffset = FVector(0, 0, 120.f);

private:
	bool bIsSuper = false;
	float SuperElapsed = 0.f;
	float SuperTimeout = 0.f;

	int32 ActiveSuperIndex = 0;
	TArray<int32> OriginalDamages;

	void StartSuper(int32 Index);
	void EndSuper();

	void SpawnSuperFX();
	void ProcessSuperHits();
	bool IsInSuperState() const;
};
