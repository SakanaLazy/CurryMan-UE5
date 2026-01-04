#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SuperMeterSystem_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class USuperMeterSystem_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	USuperMeterSystem_Prototype();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// === Settings ===
	UPROPERTY(EditAnywhere, Category = "Settings")
	int32 HitsToFill = 30;

	// === References ===
	UPROPERTY(EditAnywhere, Category = "References")
	class UUserWidget* UIWidget;

	UPROPERTY(EditAnywhere, Category = "References")
	class USuperController_Prototype* SuperController;

	// === API ===
	void ResetMeter();

private:
	int32 CurrentHits = 0;

	void AddHit(int32 TotalHits);
	void UpdateUI();
	void UnlockSuper();
};
