#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_HealthBar_Prototype.generated.h"

UCLASS()
class UUI_HealthBar_Prototype : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(
		const FGeometry& MyGeometry,
		float InDeltaTime
	) override;

	// === References ===
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UProgressBar* FillBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	class UHealthSystem_Prototype* Target;

	// === Animation Settings ===
	UPROPERTY(EditAnywhere, Category = "Animation")
	float SmoothSpeed = 5.f;

	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor FullHealthColor = FLinearColor::Green;

	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor MidHealthColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere, Category = "Color")
	FLinearColor LowHealthColor = FLinearColor::Red;

private:
	float CurrentFill = 1.f;
	float TargetFill = 1.f;

	UFUNCTION()
	void OnHealthChanged(float Current, float Max);

	FLinearColor EvaluateHealthColor(float Alpha) const;
};
