#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitDetector_Prototype.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitLanded, int32, TotalHits);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UHitDetector_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	UHitDetector_Prototype();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	// === Static hit tracking ===
	static int32 SuccessfulHits;
	static FOnHitLanded OnHitLanded;

	// === Hit Settings ===
	UPROPERTY(EditAnywhere, Category = "Hit")
	int32 Damage = 10;

	UPROPERTY(EditAnywhere, Category = "Hit")
	bool bIsPlayerAttack = true;

	// === Animator-driven activation ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	TArray<FString> AttackStateNames;

	UPROPERTY(EditAnywhere, Category = "Animation")
	int32 AnimatorLayerIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float ActiveStartNormalized = 0.f;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float ActiveEndNormalized = 1.f;

private:
	AActor* OwnerRoot = nullptr;

	UPROPERTY()
	class UShapeComponent* Hitbox = nullptr;

	UPROPERTY()
	class USkeletalMeshComponent* MeshRef = nullptr;

	UPROPERTY()
	class UAnimInstance* AnimInstance = nullptr;

	TArray<int32> AttackStateHashes;

	void RebuildStateHashes();
	bool IsInAttackWindow() const;
	bool MatchesAttackStateAndTime(const FAnimInstanceProxy& Proxy) const;

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
