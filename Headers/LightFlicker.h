#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LightFlicker_Prototype.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ULightFlicker_Prototype : public UActorComponent
{
	GENERATED_BODY()

public:
	ULightFlicker_Prototype();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction
	) override;

	// === Flicker Settings ===
	UPROPERTY(EditAnywhere, Category = "Flicker")
	float MinIntensity = 6.f;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	float MaxIntensity = 8.f;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	float FlickerSpeed = 0.1f;

	UPROPERTY(EditAnywhere, Category = "Flicker")
	float Smoothness = 0.2f;

private:
	UPROPERTY()
	class ULightComponent* LightComp = nullptr;

	float TargetIntensity = 0.f;
	float Timer = 0.f;
};
