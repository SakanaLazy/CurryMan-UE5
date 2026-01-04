#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCCombat_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNPCCombat_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCCombat_Prototype();

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
	AActor* PlayerActor;

	// === Attack Settings ===
	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MinAttackDelay = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxAttackDelay = 2.0f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	bool bUseRandomDelay = true;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float FixedDelay = 1.2f;

	// === Animation Parameters ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	FString PunchTriggerName = "Punch";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString PunchTypeName = "PunchType";

	// === Safety ===
	UPROPERTY(EditAnywhere, Category = "Safety")
	float TriggerFailSafeTime = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Safety")
	float FaceSpeed = 10.f;

private:
	bool bIsAttacking = false;
	FTimerHandle AttackTimerHandle;

	void TryStartAttack();
	void PerformAttack();
	void FacePlayer(float DeltaTime);
};
