#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthSystem_Prototype.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnHealthChanged,
	float, Current,
	float, Max
);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHealthSystem_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthSystem_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	// === Health Settings ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int32 MaxHealth = 100;

	UPROPERTY(BlueprintReadOnly, Category = "Health")
	int32 CurrentHealth = 0;

	// === References ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	USkeletalMeshComponent* MeshRef;

	UPROPERTY()
	UAnimInstance* AnimInstance;

	// === Status Flags ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bIsPlayer = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool bEnableHitReactions = true;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

	// === Animation Triggers ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	FString DeathTrigger = "KnockOut";

	UPROPERTY(EditAnywhere, Category = "Animation")
	FString HitTrigger = "HitReact";

	// === UI Event ===
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	// === Public API ===
	UFUNCTION()
	void TakeDamage(int32 Damage);

private:
	bool bKOSequenceStarted = false;

	void PlayHitReaction();
	void BeginKO();
	void Die();

	template<typename T>
	void DisableIfExists();
};
