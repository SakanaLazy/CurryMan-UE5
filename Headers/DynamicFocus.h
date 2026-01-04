#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicFocus_Prototype.generated.h"

UCLASS()
class ADynamicFocus_Prototype : public AActor
{
	GENERATED_BODY()

public:
	ADynamicFocus_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// === References ===
	UPROPERTY(EditAnywhere, Category = "References")
	AActor* PlayerActor;

	UPROPERTY(EditAnywhere, Category = "References")
	class APostProcessVolume* PostProcessVolume;

private:
	bool bHasDOF = false;
};
