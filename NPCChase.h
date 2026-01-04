#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NPCChase_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNPCChase_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCChase_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	// === Chase Settings ===
	UPROPERTY(EditAnywhere, Category = "Chase")
	float ChaseRange = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float StopRange = 220.f;

	UPROPERTY(EditAnywhere, Category = "Chase")
	float UpdateRate = 0.2f;

	// === References ===
	UPROPERTY(EditAnywhere, Category = "References")
	AActor* PlayerActor;

	UPROPERTY()
	class UNPCCombat_Prototype* CombatComp;

	UPROPERTY()
	class UAnimInstance* AnimInstance;

	UPROPERTY()
	class UHealthSystem_Prototype* HealthComp;

private:
	FTimerHandle ChaseTimerHandle;

	void UpdateChase();
};
